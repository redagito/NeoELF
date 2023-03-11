#include "nelf/PostProcess.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Context.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/RenderStation.h"
#include "nelf/Vector.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/actor/Entity.h"
#include "nelf/actor/Light.h"
#include "nelf/actor/Sprite.h"
#include "nelf/drawMode.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxRenderTarget.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxTextureWrapMode.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/objectType.h"
#include "nelf/resource/Scene.h"

static const char* vertShader = R"#(
uniform mat4 elf_ProjectionMatrix;
uniform mat4 elf_ModelviewMatrix;
varying vec2 elf_TexCoord;
attribute vec2 elf_TexCoordAttr;

void main()
{
    elf_TexCoord = elf_TexCoordAttr;
    gl_Position = elf_ProjectionMatrix * elf_ModelviewMatrix * gl_Vertex;
}
)#";

static const char* hipassShader = R"#(
uniform sampler2D elf_Texture0;
varying vec2 elf_TexCoord;
uniform float threshold;

void main()
{
    vec4 tex = texture2D(elf_Texture0, elf_TexCoord);
    gl_FragColor = max((tex-threshold) / (1.0 - threshold), 0.0);
}
)#";

static const char* blurShader = R"#(
uniform sampler2D elf_Texture0;
varying vec2 elf_TexCoord;
uniform vec2 offset;

void main()
{
    vec4 tex;
    tex = texture2D(elf_Texture0, elf_TexCoord) * 0.2;

    tex += texture2D(elf_Texture0, elf_TexCoord + offset) * 0.16;
    tex += texture2D(elf_Texture0, elf_TexCoord + offset * 2.0) * 0.12;
    tex += texture2D(elf_Texture0, elf_TexCoord + offset * 3.0) * 0.07;
    tex += texture2D(elf_Texture0, elf_TexCoord + offset * 4.0) * 0.05;

    tex += texture2D(elf_Texture0, elf_TexCoord - offset) * 0.16;
    tex += texture2D(elf_Texture0, elf_TexCoord - offset * 2.0) * 0.12;
    tex += texture2D(elf_Texture0, elf_TexCoord - offset * 3.0) * 0.07;
    tex += texture2D(elf_Texture0, elf_TexCoord - offset * 4.0) * 0.05;

    gl_FragColor = tex;
}
)#";

static const char* fogCombineShader = R"#(
uniform sampler2D elf_Texture0;
uniform sampler2D elf_Texture1;
uniform sampler2D elf_Texture2;
varying vec2 elf_TexCoord;
uniform mat4 elf_InvProjectionMatrix;
uniform float elf_FogStart;
uniform float elf_FogEnd;
uniform vec3 elf_FogColor;

void main()
{
    float mask = texture2D(elf_Texture2, elf_TexCoord).a;
    vec4 col = texture2D(elf_Texture1, elf_TexCoord);
    
    if(mask < 0.99) {
        gl_FragColor = col; 
        return;
    }
    
    float depth = texture2D(elf_Texture0, elf_TexCoord).r * 2.0 - 1.0;
    vec4 vertex = elf_InvProjectionMatrix * vec4(elf_TexCoord.x * 2.0 - 1.0, elf_TexCoord.y * 2.0 - 1.0, depth, 1.0);
    vertex = vec4(vertex.xyz / vertex.w, 1.0);
    
    float fogFactor = clamp((elf_FogEnd + vertex.z) / (elf_FogEnd - elf_FogStart), 0.0, 1.0);
    gl_FragColor = vec4(col * fogFactor + elf_FogColor * (1.0 - fogFactor), 1.0);
}
)#";

static const char* bloomCombineShader = R"#(
uniform sampler2D elf_Texture0;
uniform sampler2D elf_Texture1;
uniform sampler2D elf_Texture2;
uniform sampler2D elf_Texture3;
varying vec2 elf_TexCoord;

void main()
{
    vec4 col = texture2D(elf_Texture0, elf_TexCoord);
    // col += texture2D(elf_Texture1, elf_TexCoord);
    col += texture2D(elf_Texture2, elf_TexCoord);
    col += texture2D(elf_Texture3, elf_TexCoord);

    gl_FragColor = col;
}
)#";

static const char* dofCombineShader = R"#(
uniform sampler2D elf_Texture0;
uniform sampler2D elf_Texture1;
uniform sampler2D elf_Texture2;

varying vec2 elf_TexCoord;
uniform mat4 elf_InvProjectionMatrix;
uniform float elf_FocalRange;
uniform float elf_FocalDistance;

void main()
{
    float depth = texture2D(elf_Texture0, elf_TexCoord).r * 2.0 - 1.0;
    vec4 col = texture2D(elf_Texture1, elf_TexCoord);
    vec4 blur = texture2D(elf_Texture2, elf_TexCoord);
    vec4 vertex = elf_InvProjectionMatrix * vec4(elf_TexCoord.x * 2.0 - 1.0, elf_TexCoord.y * 2.0 - 1.0, depth, 1.0);
    
    vertex = vec4(vertex.xyz / vertex.w, 1.0);
    float ratio = clamp(abs(-vertex.z - elf_FocalDistance) / elf_FocalRange, 0.0, 1.0);
    
    gl_FragColor = vec4(col.rgb, 0.0) + ratio * vec4(blur.rgb - col.rgb, 1.0);
}
)#";

static const char* ssaoShader = R"#(
uniform sampler2D elf_Texture0;
uniform sampler2D elf_Texture1;

uniform int elf_ViewportWidth;
uniform int elf_ViewportHeight;

uniform float elf_ClipStart;
uniform float elf_ClipEnd;

uniform float amount;
varying vec2 elf_TexCoord;

#define PI 3.14159265

float width = float(elf_ViewportWidth);
float height = float(elf_ViewportHeight);
float near = elf_ClipStart;
float far = elf_ClipEnd;
int samples = 7;
int rings = 3;

vec2 rand(in vec2 coord)
{
    float noiseX = (fract(sin(dot(coord, vec2(12.9898,78.233))) * 43758.5453));
    float noiseY = (fract(sin(dot(coord, vec2(12.9898,78.233) * 2.0)) * 43758.5453));
    return vec2(noiseX, noiseY) * 0.004;
}

float readDepth(in vec2 coord)
{
    return (2.0 * near) / (far + near - texture2D(elf_Texture1, coord).x * (far-near));
}

float compareDepths(in float depth1, in float depth2)
{
    float aoCap = 1.0;
    float aoMultiplier = 100.0;
    float depthTolerance = 0.0000;
    float aorange = 60.0;
    float diff = sqrt(clamp(1.0 - (depth1 - depth2) / (aorange / (far - near)), 0.0, 1.0));
    float ao = min(aoCap, max(0.0, depth1 - depth2 - depthTolerance) * aoMultiplier) * diff;
    return ao;
}

void main(void)
{
    float depth = readDepth(elf_TexCoord);
    float d;
    float aspect = width / height;
    vec2 noise = rand(elf_TexCoord);
    float w = (1.0 / width) / clamp(depth, 0.05,1.0) + (noise.x * (1.0 - noise.x));
    float h = (1.0 / height) / clamp(depth, 0.05,1.0) + (noise.y * (1.0 - noise.y));

    float pw;
    float ph;
    float ao;
    float s;
    float fade = 1.0;

    for (int i = 0 ; i < rings; i += 1)
    {
        fade *= 0.5;
        for (int j = 0 ; j < samples * i; j += 1)
        {
            float step = PI * 2.0 / (float(samples * i));
            pw = (cos(float(j) * step) * float(i));
            ph = (sin(float(j) * step) * float(i)) * aspect;
            d = readDepth(vec2(elf_TexCoord.s + pw * w, elf_TexCoord.t + ph * h));
            ao += compareDepths(depth,d) * fade;
            s += 1.0 * fade;
        }
    }

    ao /= s;
    ao = 1.0 - ao;

    vec3 color = texture2D(elf_Texture0, elf_TexCoord).rgb;
    vec3 luminance = vec3(color.r * 0.3 + color.g * 0.59 + color.b * 0.11) - (amount - 1.0);
    vec3 black = vec3(0.0, 0.0, 0.0);
    vec3 treshold = vec3(0.2, 0.2, 0.2);

    // luminance = clamp(max(black,luminance - treshold) + max(black, luminance - treshold) + max(black, luminance - treshold), 0.0, 1.0);
    float lum = max(black, luminance - treshold);
    luminance = clamp(3 * lum, 0.0, 1.0);

    gl_FragColor = vec4(color.rgb * mix(vec3(ao, ao, ao).rgb, vec3(1.0, 1.0, 1.0), luminance), 1.0);
}
)#";

static const char* ssaoCombineShader = R"#(
uniform sampler2D elf_Texture0;
uniform sampler2D elf_Texture1;
varying vec2 elf_TexCoord;

void main()
{
    vec4 ssao = texture2D(elf_Texture1, elf_TexCoord);
    vec4 col = texture2D(elf_Texture0, elf_TexCoord);
    gl_FragColor = vec4(col.rgb * ssao.rgb, col.a);
}
)#";

static const char* lightShaftShader = R"#(
uniform sampler2D elf_Texture0;

uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;

uniform vec2 lightPosition;
varying vec2 elf_TexCoord;

const int NUM_SAMPLES = 100;

void main()
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec2 deltaTextCoord = vec2(elf_TexCoord.st - lightPosition.xy);
    vec2 textCoord = elf_TexCoord.st;
    deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;

    for(int i=0; i < NUM_SAMPLES ; i++)
    {
        textCoord -= deltaTextCoord;
        vec4 sample = texture2D(elf_Texture0, textCoord);
        sample *= illuminationDecay * weight;
        finalColor += sample;
        illuminationDecay *= decay;
    }
    gl_FragColor = finalColor*exposure;
}
)#";

elfPostProcess* elfCreatePostProcess()
{
    elfPostProcess* postProcess;

    postProcess = (elfPostProcess*)malloc(sizeof(elfPostProcess));
    memset(postProcess, 0x0, sizeof(elfPostProcess));
    postProcess->objType = ELF_POST_PROCESS;
    postProcess->objDestr = elfDestroyPostProcess;

    elfInitPostProcessBuffers(postProcess);

    postProcess->hipassShdr = gfxCreateShaderProgram(vertShader, hipassShader);
    postProcess->blurShdr = gfxCreateShaderProgram(vertShader, blurShader);
    postProcess->bloomCombineShdr = gfxCreateShaderProgram(vertShader, bloomCombineShader);
    postProcess->dofCombineShdr = gfxCreateShaderProgram(vertShader, dofCombineShader);
    postProcess->ssaoShdr = gfxCreateShaderProgram(vertShader, ssaoShader);
    postProcess->ssaoCombineShdr = gfxCreateShaderProgram(vertShader, ssaoCombineShader);
    postProcess->lightShaftShdr = gfxCreateShaderProgram(vertShader, lightShaftShader);

    postProcess->lightShaftTransform = gfxCreateObjectTransform();

    gfxSetShaderParamsDefault(&postProcess->shaderParams);

    elfIncObj(ELF_POST_PROCESS);

    return postProcess;
}

void elfDestroyPostProcess(void* data)
{
    elfPostProcess* postProcess = (elfPostProcess*)data;

    gfxDecRef((gfxObject*)postProcess->mainRt);
    gfxDecRef((gfxObject*)postProcess->mainRtColor[0]);
    if (postProcess->mainRtColor[1])
        gfxDecRef((gfxObject*)postProcess->mainRtColor[1]);
    gfxDecRef((gfxObject*)postProcess->mainRtDepth);

    gfxDecRef((gfxObject*)postProcess->rtHigh);
    gfxDecRef((gfxObject*)postProcess->rtMed);
    gfxDecRef((gfxObject*)postProcess->rtLow);
    gfxDecRef((gfxObject*)postProcess->rtTiny);

    gfxDecRef((gfxObject*)postProcess->rtTexHigh_1);
    gfxDecRef((gfxObject*)postProcess->rtTexHigh_2);
    gfxDecRef((gfxObject*)postProcess->rtTexHighDepth);
    gfxDecRef((gfxObject*)postProcess->rtTexMed_1);
    gfxDecRef((gfxObject*)postProcess->rtTexMed_2);
    gfxDecRef((gfxObject*)postProcess->rtTexMed_3);
    gfxDecRef((gfxObject*)postProcess->rtTexLow_1);
    gfxDecRef((gfxObject*)postProcess->rtTexLow_2);
    gfxDecRef((gfxObject*)postProcess->rtTexTiny_1);
    gfxDecRef((gfxObject*)postProcess->rtTexTiny_2);

    if (postProcess->hipassShdr)
        gfxDestroyShaderProgram(postProcess->hipassShdr);
    if (postProcess->blurShdr)
        gfxDestroyShaderProgram(postProcess->blurShdr);
    if (postProcess->bloomCombineShdr)
        gfxDestroyShaderProgram(postProcess->bloomCombineShdr);
    if (postProcess->dofCombineShdr)
        gfxDestroyShaderProgram(postProcess->dofCombineShdr);
    if (postProcess->ssaoShdr)
        gfxDestroyShaderProgram(postProcess->ssaoShdr);
    if (postProcess->ssaoCombineShdr)
        gfxDestroyShaderProgram(postProcess->ssaoCombineShdr);
    if (postProcess->lightShaftShdr)
        gfxDestroyShaderProgram(postProcess->lightShaftShdr);

    gfxDestroyTransform(postProcess->lightShaftTransform);

    free(postProcess);

    elfDecObj(ELF_POST_PROCESS);
}

void elfInitPostProcessBuffers(elfPostProcess* postProcess)
{
    if (postProcess->mainRt)
        gfxDecRef((gfxObject*)postProcess->mainRt);
    if (postProcess->mainRtColor[0])
        gfxDecRef((gfxObject*)postProcess->mainRtColor[0]);
    if (postProcess->mainRtColor[1])
        gfxDecRef((gfxObject*)postProcess->mainRtColor[1]);

    if (postProcess->rtHigh)
        gfxDecRef((gfxObject*)postProcess->rtHigh);
    if (postProcess->rtMed)
        gfxDecRef((gfxObject*)postProcess->rtMed);
    if (postProcess->rtLow)
        gfxDecRef((gfxObject*)postProcess->rtLow);
    if (postProcess->rtTiny)
        gfxDecRef((gfxObject*)postProcess->rtTiny);

    if (postProcess->rtTexHigh_1)
        gfxDecRef((gfxObject*)postProcess->rtTexHigh_1);
    if (postProcess->rtTexHigh_2)
        gfxDecRef((gfxObject*)postProcess->rtTexHigh_2);
    if (postProcess->rtTexHighDepth)
        gfxDecRef((gfxObject*)postProcess->rtTexHighDepth);
    if (postProcess->rtTexMed_1)
        gfxDecRef((gfxObject*)postProcess->rtTexMed_1);
    if (postProcess->rtTexMed_2)
        gfxDecRef((gfxObject*)postProcess->rtTexMed_2);
    if (postProcess->rtTexMed_3)
        gfxDecRef((gfxObject*)postProcess->rtTexMed_3);
    if (postProcess->rtTexLow_1)
        gfxDecRef((gfxObject*)postProcess->rtTexLow_1);
    if (postProcess->rtTexLow_2)
        gfxDecRef((gfxObject*)postProcess->rtTexLow_2);
    if (postProcess->rtTexTiny_1)
        gfxDecRef((gfxObject*)postProcess->rtTexTiny_1);
    if (postProcess->rtTexTiny_2)
        gfxDecRef((gfxObject*)postProcess->rtTexTiny_2);

    postProcess->bufferWidth = elfGetWindowWidth() / 4;
    postProcess->bufferHeight = elfGetWindowHeight() / 4;

    postProcess->mainRtColor[0] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                     GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
    postProcess->mainRtDepth =
        gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_DEPTH_COMPONENT,
                           GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);

    postProcess->mainRt = gfxCreateRenderTarget(elfGetWindowWidth(), elfGetWindowHeight());

    gfxSetRenderTargetColorTexture(postProcess->mainRt, 0, postProcess->mainRtColor[0]);
    gfxSetRenderTargetDepthTexture(postProcess->mainRt, postProcess->mainRtDepth);

    postProcess->rtTexHigh_1 = gfxCreate2dTexture(postProcess->bufferWidth * 2, postProcess->bufferHeight * 2, 0.0f,
                                                  GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexHigh_2 = gfxCreate2dTexture(postProcess->bufferWidth * 2, postProcess->bufferHeight * 2, 0.0f,
                                                  GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexHighDepth =
        gfxCreate2dTexture(postProcess->bufferWidth * 2, postProcess->bufferHeight * 2, 0.0f, GFX_CLAMP, GFX_LINEAR,
                           GFX_DEPTH_COMPONENT, GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
    postProcess->rtTexMed_1 = gfxCreate2dTexture(postProcess->bufferWidth, postProcess->bufferHeight, 0.0f, GFX_CLAMP,
                                                 GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexMed_2 = gfxCreate2dTexture(postProcess->bufferWidth, postProcess->bufferHeight, 0.0f, GFX_CLAMP,
                                                 GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexMed_3 = gfxCreate2dTexture(postProcess->bufferWidth, postProcess->bufferHeight, 0.0f, GFX_CLAMP,
                                                 GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexLow_1 = gfxCreate2dTexture(postProcess->bufferWidth / 2, postProcess->bufferHeight / 2, 0.0f,
                                                 GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexLow_2 = gfxCreate2dTexture(postProcess->bufferWidth / 2, postProcess->bufferHeight / 2, 0.0f,
                                                 GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexTiny_1 = gfxCreate2dTexture(postProcess->bufferWidth / 4, postProcess->bufferHeight / 4, 0.0f,
                                                  GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
    postProcess->rtTexTiny_2 = gfxCreate2dTexture(postProcess->bufferWidth / 4, postProcess->bufferHeight / 4, 0.0f,
                                                  GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);

    postProcess->rtHigh = gfxCreateRenderTarget(postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);
    postProcess->rtMed = gfxCreateRenderTarget(postProcess->bufferWidth, postProcess->bufferHeight);
    postProcess->rtLow = gfxCreateRenderTarget(postProcess->bufferWidth / 2, postProcess->bufferHeight / 2);
    postProcess->rtTiny = gfxCreateRenderTarget(postProcess->bufferWidth / 4, postProcess->bufferHeight / 4);

    gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_1);
    gfxSetRenderTargetDepthTexture(postProcess->rtHigh, postProcess->rtTexHighDepth);
    gfxSetRenderTargetColorTexture(postProcess->rtMed, 0, postProcess->rtTexMed_1);
    gfxSetRenderTargetColorTexture(postProcess->rtLow, 0, postProcess->rtTexLow_1);
    gfxSetRenderTargetColorTexture(postProcess->rtTiny, 0, postProcess->rtTexTiny_1);

    gfxIncRef((gfxObject*)postProcess->mainRtColor[0]);
    gfxIncRef((gfxObject*)postProcess->mainRtDepth);
    gfxIncRef((gfxObject*)postProcess->mainRt);

    gfxIncRef((gfxObject*)postProcess->rtHigh);
    gfxIncRef((gfxObject*)postProcess->rtMed);
    gfxIncRef((gfxObject*)postProcess->rtLow);
    gfxIncRef((gfxObject*)postProcess->rtTiny);

    gfxIncRef((gfxObject*)postProcess->rtTexHigh_1);
    gfxIncRef((gfxObject*)postProcess->rtTexHigh_2);
    gfxIncRef((gfxObject*)postProcess->rtTexHighDepth);
    gfxIncRef((gfxObject*)postProcess->rtTexMed_1);
    gfxIncRef((gfxObject*)postProcess->rtTexMed_2);
    gfxIncRef((gfxObject*)postProcess->rtTexMed_3);
    gfxIncRef((gfxObject*)postProcess->rtTexLow_1);
    gfxIncRef((gfxObject*)postProcess->rtTexLow_2);
    gfxIncRef((gfxObject*)postProcess->rtTexTiny_1);
    gfxIncRef((gfxObject*)postProcess->rtTexTiny_2);

    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

static void runPostProcessSSAO(elfPostProcess* postProcess, elfScene* scene, bool sourceRt)
{
    if (postProcess->dof || postProcess->bloom)
    {
        gfxSetRenderTarget(postProcess->mainRt);
        gfxSetRenderTargetColorTexture(postProcess->mainRt, 0, postProcess->mainRtColor[!sourceRt]);
    }
    else
    {
        gfxDisableRenderTarget();
    }

    gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    elfCamera* cam;
    if ((cam = elfGetSceneActiveCamera(scene)))
    {
        postProcess->shaderParams.clipStart = elfGetCameraClip(cam).x;
        postProcess->shaderParams.clipEnd = elfGetCameraClip(cam).y;
        postProcess->shaderParams.viewportWidth = elfGetWindowWidth() * 2;
        postProcess->shaderParams.viewportHeight = elfGetWindowHeight() * 2;
    }

    postProcess->shaderParams.shaderProgram = postProcess->ssaoShdr;

    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniform1f("amount", postProcess->ssaoAmount);

    elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

    postProcess->shaderParams.textureParams[0].texture = nullptr;
    postProcess->shaderParams.textureParams[1].texture = nullptr;
}

static void runPostProcessDOF(elfPostProcess* postProcess, elfScene* scene, bool sourceRt)
{
    gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);
    gfxGetOrthographicProjectionMatrix(0, postProcess->bufferWidth * 2, 0, postProcess->bufferHeight * 2, -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    gfxSetRenderTarget(postProcess->rtHigh);
    gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_1);

    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
    gfxSetShaderParams(&postProcess->shaderParams);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);

    gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_2);

    postProcess->shaderParams.shaderProgram = postProcess->blurShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_1;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 1.0f / ((float)(postProcess->bufferWidth * 2)), 0.0f);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);

    gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_1);

    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_2;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 0.0f, 1.0f / ((float)(postProcess->bufferHeight * 2)));
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);

    if (postProcess->bloom)
    {
        gfxSetRenderTarget(postProcess->mainRt);
        gfxSetRenderTargetColorTexture(postProcess->mainRt, 0, postProcess->mainRtColor[!sourceRt]);
    }
    else
    {
        gfxDisableRenderTarget();
    }

    gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    gfxMatrix4GetInverse(scene->curCamera->projectionMatrix, postProcess->shaderParams.invProjectionMatrix);

    postProcess->shaderParams.shaderProgram = postProcess->dofCombineShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtDepth;
    postProcess->shaderParams.textureParams[1].texture = postProcess->mainRtColor[0];
    postProcess->shaderParams.textureParams[2].texture = postProcess->rtTexHigh_1;

    gfxSetShaderParams(&postProcess->shaderParams);

    gfxSetShaderProgramUniform1f("elf_FocalRange", postProcess->dofFocalRange);
    gfxSetShaderProgramUniform1f("elf_FocalDistance", postProcess->dofFocalDistance);

    elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

    postProcess->shaderParams.textureParams[0].texture = nullptr;
    postProcess->shaderParams.textureParams[1].texture = nullptr;
    postProcess->shaderParams.textureParams[2].texture = nullptr;
}

static void runPostProcessBloom(elfPostProcess* postProcess, elfScene* scene, bool sourceRt)
{
    gfxSetViewport(0, 0, postProcess->bufferWidth, postProcess->bufferHeight);
    gfxGetOrthographicProjectionMatrix(0, postProcess->bufferWidth, 0, postProcess->bufferHeight, -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    gfxSetRenderTarget(postProcess->rtMed);
    gfxSetRenderTargetColorTexture(postProcess->rtMed, 0, postProcess->rtTexMed_1);

    postProcess->shaderParams.shaderProgram = postProcess->hipassShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniform1f("threshold", postProcess->bloomThreshold);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth, postProcess->bufferHeight);

    gfxSetRenderTargetColorTexture(postProcess->rtMed, 0, postProcess->rtTexMed_2);

    postProcess->shaderParams.shaderProgram = postProcess->blurShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexMed_1;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 1.0f / ((float)postProcess->bufferWidth), 0.0);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth, postProcess->bufferHeight);

    gfxSetRenderTargetColorTexture(postProcess->rtMed, 0, postProcess->rtTexMed_3);

    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexMed_2;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 0.0f, 1.0f / ((float)postProcess->bufferHeight));
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth, postProcess->bufferHeight);

    gfxSetViewport(0, 0, postProcess->bufferWidth / 2, postProcess->bufferHeight / 2);
    gfxGetOrthographicProjectionMatrix(0.0f, postProcess->bufferWidth / 2, 0.0f, postProcess->bufferHeight / 2, -1.0f,
                                       1.0f, postProcess->shaderParams.projectionMatrix);

    gfxSetRenderTarget(postProcess->rtLow);
    gfxSetRenderTargetColorTexture(postProcess->rtLow, 0, postProcess->rtTexLow_1);

    postProcess->shaderParams.shaderProgram = postProcess->blurShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexMed_3;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 1.0f / ((float)postProcess->bufferWidth / 2), 0.0f);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth / 2, postProcess->bufferHeight / 2);

    gfxSetRenderTargetColorTexture(postProcess->rtLow, 0, postProcess->rtTexLow_2);

    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexLow_1;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 0.0f, 1.0f / ((float)postProcess->bufferHeight / 2));
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth / 2, postProcess->bufferHeight / 2);

    gfxSetViewport(0, 0, postProcess->bufferWidth / 4, postProcess->bufferHeight / 4);
    gfxGetOrthographicProjectionMatrix(0.0f, postProcess->bufferWidth / 4, 0.0f, postProcess->bufferHeight / 4, -1.0f,
                                       1.0f, postProcess->shaderParams.projectionMatrix);

    gfxSetRenderTarget(postProcess->rtTiny);
    gfxSetRenderTargetColorTexture(postProcess->rtTiny, 0, postProcess->rtTexTiny_1);

    postProcess->shaderParams.shaderProgram = postProcess->blurShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexLow_2;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 1.0f / ((float)postProcess->bufferWidth / 4), 0.0f);
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth / 4, postProcess->bufferHeight / 4);

    gfxSetRenderTargetColorTexture(postProcess->rtTiny, 0, postProcess->rtTexTiny_2);

    postProcess->shaderParams.textureParams[0].texture = postProcess->rtTexTiny_1;
    gfxSetShaderParams(&postProcess->shaderParams);
    gfxSetShaderProgramUniformVec2("offset", 0.0f, 1.0f / ((float)postProcess->bufferHeight / 4));
    elfDrawTextured2dQuad(0.0f, 0.0f, postProcess->bufferWidth / 4, postProcess->bufferHeight / 4);

    gfxDisableRenderTarget();

    gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    postProcess->shaderParams.shaderProgram = postProcess->bloomCombineShdr;
    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
    postProcess->shaderParams.textureParams[1].texture = postProcess->rtTexMed_3;
    postProcess->shaderParams.textureParams[2].texture = postProcess->rtTexLow_2;
    postProcess->shaderParams.textureParams[3].texture = postProcess->rtTexTiny_2;
    gfxSetShaderParams(&postProcess->shaderParams);

    elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

    postProcess->shaderParams.textureParams[0].texture = nullptr;
    postProcess->shaderParams.textureParams[1].texture = nullptr;
    postProcess->shaderParams.textureParams[2].texture = nullptr;
    postProcess->shaderParams.textureParams[3].texture = nullptr;
}

static void runPostProcessDefault(elfPostProcess* postProcess, bool sourceRt)
{
    gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f, 1.0f,
                                       postProcess->shaderParams.projectionMatrix);

    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
    gfxSetShaderParams(&postProcess->shaderParams);

    elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

    postProcess->shaderParams.textureParams[0].texture = nullptr;
}

static void runPostProcessLightShafts(elfPostProcess* postProcess, elfScene* scene)
{
    bool firstShaft = true;
    for (elfLight* light = (elfLight*)elfBeginList(scene->lights); light;
         light = (elfLight*)elfGetListNext(scene->lights))
    {
        elfVec3f lightPos = elfGetActorPosition((elfActor*)light);
        if (light->shaft && elfSphereInsideFrustum(scene->curCamera, &lightPos.x, light->shaftSize))
        {
            if (firstShaft)
            {
                gfxSetShaderParamsDefault(&scene->shaderParams);
                elfSetCamera(scene->curCamera, &scene->shaderParams);
                gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);
                gfxSetShaderParams(&scene->shaderParams);

                gfxSetRenderTarget(postProcess->rtHigh);
                gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_1);
                gfxClearBuffers(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

                scene->shaderParams.renderParams.colorWrite = false;
                scene->shaderParams.renderParams.alphaWrite = false;

                elfEntity* ent;
                int i;
                for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue);
                     i < scene->entityQueueCount && ent != nullptr;
                     i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                }

                elfSprite* spr;
                for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue);
                     i < scene->spriteQueueCount && spr != nullptr;
                     i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
                {
                    elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                }

                firstShaft = false;
            }

            elfVec3f camPos = elfGetActorPosition((elfActor*)scene->curCamera);
            elfVec4f camOrient = elfGetActorOrientation((elfActor*)scene->curCamera);

            int viewport[4];
            viewport[0] = 0;
            viewport[1] = 0;
            viewport[2] = elfGetWindowWidth();
            viewport[3] = elfGetWindowHeight();

            elfVec3f lightScreenPos;
            gfxProject(lightPos.x, lightPos.y, lightPos.z, elfGetCameraModelviewMatrix(scene->curCamera),
                       elfGetCameraProjectionMatrix(scene->curCamera), viewport, &lightScreenPos.x);

            lightPos = elfSubVec3fVec3f(lightPos, camPos);
            camOrient = elfGetQuaInverted(camOrient);
            lightPos = elfMulQuaVec3f(camOrient, lightPos);

            // render the light beacon
            gfxSetShaderParamsDefault(&scene->shaderParams);

            elfSetCamera(scene->curCamera, &scene->shaderParams);
            gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);

            scene->shaderParams.renderParams.depthWrite = false;

            gfxSetTransformPosition(postProcess->lightShaftTransform, lightPos.x, lightPos.y, lightPos.z);
            memcpy(scene->shaderParams.modelviewMatrix, gfxGetTransformMatrix(postProcess->lightShaftTransform),
                   sizeof(float) * 16);
            gfxSetColor(&scene->shaderParams.materialParams.diffuseColor, light->color.r, light->color.g,
                        light->color.b, 1.0);

            gfxSetShaderParams(&scene->shaderParams);

            gfxSetRenderTarget(postProcess->rtHigh);
            gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_1);

            elfDrawCircle(0, 0, 32, light->shaftSize);

            // shaft the light...
            gfxSetRenderTargetColorTexture(postProcess->rtHigh, 0, postProcess->rtTexHigh_2);

            gfxSetShaderParamsDefault(&scene->shaderParams);
            gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);
            gfxGetOrthographicProjectionMatrix(0.0f, (float)postProcess->bufferWidth * 2, 0.0f,
                                               (float)postProcess->bufferHeight * 2, -1.0f, 1.0f,
                                               scene->shaderParams.projectionMatrix);

            scene->shaderParams.renderParams.depthTest = false;
            scene->shaderParams.renderParams.depthWrite = false;
            scene->shaderParams.shaderProgram = postProcess->lightShaftShdr;
            scene->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_1;

            gfxSetShaderParams(&scene->shaderParams);
            gfxSetShaderProgramUniform1f("exposure", 0.0034f);
            gfxSetShaderProgramUniform1f("decay", 1.0f);
            gfxSetShaderProgramUniform1f("density", 1.0f - light->shaftFadeOff);
            gfxSetShaderProgramUniform1f("weight", light->shaftIntensity * 5.0f * postProcess->lightShaftsIntensity);
            gfxSetShaderProgramUniformVec2("lightPosition", lightScreenPos.x / (float)elfGetWindowWidth(),
                                           lightScreenPos.y / (float)elfGetWindowHeight());

            elfDrawTextured2dQuad(0.0f, 0.0f, (float)postProcess->bufferWidth * 2,
                                  (float)postProcess->bufferHeight * 2);

            gfxDisableRenderTarget();

            // add light shaft to scene
            gfxSetShaderParamsDefault(&scene->shaderParams);
            gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
            gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(),
                                               -1.0f, 1.0f, scene->shaderParams.projectionMatrix);

            scene->shaderParams.renderParams.depthTest = false;
            scene->shaderParams.renderParams.depthWrite = false;
            scene->shaderParams.renderParams.blendMode = GFX_ADD;
            scene->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_2;

            gfxSetShaderParams(&scene->shaderParams);

            elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

            scene->shaderParams.textureParams[0].texture = nullptr;
        }
    }
}

void elfRunPostProcess(elfPostProcess* postProcess, elfScene* scene)
{
    gfxDisableRenderTarget();

    gfxSetShaderParamsDefault(&postProcess->shaderParams);
    postProcess->shaderParams.renderParams.depthTest = false;
    postProcess->shaderParams.renderParams.depthWrite = false;
    postProcess->shaderParams.renderParams.alphaTest = false;

    // Use rendertarget as source
    bool sourceRt = false;

    // SSAO
    if (postProcess->ssao && scene->curCamera)
    {
        runPostProcessSSAO(postProcess, scene, sourceRt);
        sourceRt = !sourceRt;
    }

    // DOF
    if (postProcess->dof)
    {
        runPostProcessDOF(postProcess, scene, sourceRt);
        sourceRt = !sourceRt;
    }

    // BLOOM
    if (postProcess->bloom)
    {
        runPostProcessBloom(postProcess, scene, sourceRt);
    }

    // Default
    // TODO ssao only active if currentCam != null, BUG?
    if (!postProcess->bloom && !postProcess->dof && !postProcess->ssao)
    {
        runPostProcessDefault(postProcess, sourceRt);
    }

    // LIGHT SHAFTS
    if (postProcess->lightShafts && scene->curCamera)
    {
        runPostProcessLightShafts(postProcess, scene);
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&postProcess->shaderParams);
    gfxSetShaderParams(&postProcess->shaderParams);

    // TODO Can be removed?
    /*gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0, (float)elfGetWindowWidth(),
            0.0, (float)elfGetWindowHeight(), -1.0, 1.0,
            postProcess->shaderParams.projectionMatrix);

    postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[curMainRt];
    gfxSetShaderParams(&postProcess->shaderParams);

    elfDrawTextured2dQuad(0.0, 0.0, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());*/
}

void elfSetPostProcessBloom(elfPostProcess* postProcess, float threshold)
{
    postProcess->bloom = true;
    postProcess->bloomThreshold = threshold;
    if (postProcess->bloomThreshold < 0.0001f)
        postProcess->bloomThreshold = 0.0001f;
    if (postProcess->bloomThreshold > 0.9999f)
        postProcess->bloomThreshold = 0.9999f;
    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

void elfDisablePostProcessBloom(elfPostProcess* postProcess) { postProcess->bloom = false; }

float elfGetPostProcessBloomThreshold(elfPostProcess* postProcess) { return postProcess->bloomThreshold; }

void elfSetPostProcessDof(elfPostProcess* postProcess, float focalRange, float focalDistance)
{
    postProcess->dof = true;
    postProcess->dofFocalRange = focalRange;
    if (postProcess->dofFocalRange < 0.0f)
        postProcess->dofFocalRange = 0.0f;
    postProcess->dofFocalDistance = focalDistance;
    if (postProcess->dofFocalDistance < 0.0f)
        postProcess->dofFocalDistance = 0.0f;
    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

void elfDisablePostProcessDof(elfPostProcess* postProcess) { postProcess->dof = false; }

float elfGetPostProcessDofFocalRange(elfPostProcess* postProcess) { return postProcess->dofFocalRange; }

float elfGetPostProcessDofFocalDistance(elfPostProcess* postProcess) { return postProcess->dofFocalDistance; }

void elfSetPostProcessSsao(elfPostProcess* postProcess, float amount)
{
    postProcess->ssao = true;
    postProcess->ssaoAmount = amount;

    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

void elfDisablePostProcessSsao(elfPostProcess* postProcess) { postProcess->ssao = false; }

float elfGetPostProcessSsaoAmount(elfPostProcess* postProcess) { return postProcess->ssaoAmount; }

void elfSetPostProcessLightShafts(elfPostProcess* postProcess, float intensity)
{
    postProcess->lightShafts = true;
    postProcess->lightShaftsIntensity = intensity;
    if (postProcess->lightShaftsIntensity < 0.0f)
        postProcess->lightShaftsIntensity = 0.0f;

    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

void elfDisablePostProcessLightShafts(elfPostProcess* postProcess) { postProcess->lightShafts = false; }

float elfGetPostProcessLightShaftsIntensity(elfPostProcess* postProcess) { return postProcess->lightShaftsIntensity; }

bool elfIsPostProcessBloom(elfPostProcess* postProcess) { return postProcess->bloom; }

bool elfIsPostProcessSsao(elfPostProcess* postProcess) { return postProcess->ssao; }

bool elfIsPostProcessDof(elfPostProcess* postProcess) { return postProcess->dof; }

bool elfIsPostProcessLightShafts(elfPostProcess* postProcess) { return postProcess->lightShafts; }
