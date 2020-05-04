
const char* vertShader =
    "uniform mat4 elf_ProjectionMatrix;\n"
    "uniform mat4 elf_ModelviewMatrix;\n"
    "varying vec2 elf_TexCoord;\n"
    "attribute vec2 elf_TexCoordAttr;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\telf_TexCoord = elf_TexCoordAttr;\n"
    "\tgl_Position = elf_ProjectionMatrix*elf_ModelviewMatrix*gl_Vertex;\n"
    "}\n";

const char* hipassShader =
    "uniform sampler2D elf_Texture0;\n"
    "varying vec2 elf_TexCoord;\n"
    "uniform float threshold;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tvec4 tex = texture2D(elf_Texture0, elf_TexCoord);\n"
    "\tgl_FragColor = max((tex-threshold)/(1.0-threshold), 0.0);\n"
    "}\n";

const char* blurShader =
    "uniform sampler2D elf_Texture0;\n"
    "varying vec2 elf_TexCoord;\n"
    "uniform vec2 offset;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tvec4 tex;\n"
    "\ttex = texture2D(elf_Texture0, elf_TexCoord)*0.2;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord+offset)*0.16;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*2.0)*0.12;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*3.0)*0.07;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*4.0)*0.05;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord-offset)*0.16;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*2.0)*0.12;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*3.0)*0.07;\n"
    "\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*4.0)*0.05;\n"
    "\tgl_FragColor = tex;\n"
    "}\n";

const char* fogCombineShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform sampler2D elf_Texture1;\n"
    "uniform sampler2D elf_Texture2;\n"
    "varying vec2 elf_TexCoord;\n"
    "uniform mat4 elf_InvProjectionMatrix;"
    "uniform float elf_FogStart;\n"
    "uniform float elf_FogEnd;\n"
    "uniform vec3 elf_FogColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tfloat mask = texture2D(elf_Texture2, elf_TexCoord).a;\n"
    "\tvec4 col = texture2D(elf_Texture1, elf_TexCoord);\n"
    "\tif(mask < 0.99) {gl_FragColor = col; return;}\n"
    "\tfloat depth = texture2D(elf_Texture0, elf_TexCoord).r*2.0-1.0;\n"
    "\tvec4 vertex = elf_InvProjectionMatrix*vec4(elf_TexCoord.x*2.0-1.0, elf_TexCoord.y*2.0-1.0, depth, 1.0);\n"
    "\tvertex = vec4(vertex.xyz/vertex.w, 1.0);\n"
    "\tfloat fogFactor = clamp((elf_FogEnd+vertex.z)/(elf_FogEnd-elf_FogStart), 0.0, 1.0);\n"
    "\tgl_FragColor = vec4(col*fogFactor+elf_FogColor*(1.0-fogFactor), 1.0);\n"
    "}\n";

const char* bloomCombineShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform sampler2D elf_Texture1;\n"
    "uniform sampler2D elf_Texture2;\n"
    "uniform sampler2D elf_Texture3;\n"
    "varying vec2 elf_TexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tvec4 col = texture2D(elf_Texture0, elf_TexCoord);\n"
    "\t//col += texture2D(elf_Texture1, elf_TexCoord);\n"
    "\tcol += texture2D(elf_Texture2, elf_TexCoord);\n"
    "\tcol += texture2D(elf_Texture3, elf_TexCoord);\n"
    "\tgl_FragColor = col;\n"
    "}\n";

const char* dofCombineShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform sampler2D elf_Texture1;\n"
    "uniform sampler2D elf_Texture2;\n"
    "varying vec2 elf_TexCoord;\n"
    "uniform mat4 elf_InvProjectionMatrix;"
    "uniform float elf_FocalRange;\n"
    "uniform float elf_FocalDistance;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tfloat depth = texture2D(elf_Texture0, elf_TexCoord).r*2.0-1.0;\n"
    "\tvec4 col = texture2D(elf_Texture1, elf_TexCoord);\n"
    "\tvec4 blur = texture2D(elf_Texture2, elf_TexCoord);\n"
    "\tvec4 vertex = elf_InvProjectionMatrix*vec4(elf_TexCoord.x*2.0-1.0, elf_TexCoord.y*2.0-1.0, depth, 1.0);\n"
    "\tvertex = vec4(vertex.xyz/vertex.w, 1.0);\n"
    "\tfloat ratio = clamp(abs(-vertex.z-elf_FocalDistance)/elf_FocalRange, 0.0, 1.0);\n"
    "\tgl_FragColor = vec4(col.rgb, 0.0)+ratio*vec4(blur.rgb-col.rgb, 1.0);\n"
    "}\n";

const char* ssaoShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform sampler2D elf_Texture1;\n"
    "uniform int elf_ViewportWidth;\n"
    "uniform int elf_ViewportHeight;\n"
    "uniform float elf_ClipStart;\n"
    "uniform float elf_ClipEnd;\n"
    "uniform float amount;\n"
    "varying vec2 elf_TexCoord;\n"
    "#define PI 3.14159265\n"
    "float width = float(elf_ViewportWidth);\n"
    "float height = float(elf_ViewportHeight);\n"
    "float near = elf_ClipStart;\n"
    "float far = elf_ClipEnd;\n"
    "int samples = 7;\n"
    "int rings = 3;\n"
    "vec2 rand(in vec2 coord)\n"
    "{\n"
    "\tfloat noiseX = (fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453));\n"
    "\tfloat noiseY = (fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453));\n"
    "\treturn vec2(noiseX,noiseY)*0.004;\n"
    "}\n"
    "float readDepth(in vec2 coord)\n"
    "{\n"
    "\treturn (2.0 * near) / (far + near - texture2D(elf_Texture1, coord).x * (far-near));\n"
    "}\n"
    "float compareDepths(in float depth1, in float depth2)\n"
    "{\n"
    "\tfloat aoCap = 1.0;\n"
    "\tfloat aoMultiplier = 100.0;\n"
    "\tfloat depthTolerance = 0.0000;\n"
    "\tfloat aorange = 60.0;\n"
    "\tfloat diff = sqrt(clamp(1.0-(depth1-depth2) / (aorange/(far-near)),0.0,1.0));\n"
    "\tfloat ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;\n"
    "\treturn ao;\n"
    "}\n"
    "void main(void)\n"
    "{\n"
    "\tfloat depth = readDepth(elf_TexCoord);\n"
    "\tfloat d;\n"
    "\tfloat aspect = width/height;\n"
    "\tvec2 noise = rand(elf_TexCoord);\n"
    "\tfloat w = (1.0 / width)/clamp(depth,0.05,1.0)+(noise.x*(1.0-noise.x));\n"
    "\tfloat h = (1.0 / height)/clamp(depth,0.05,1.0)+(noise.y*(1.0-noise.y));\n"
    "\tfloat pw;\n"
    "\tfloat ph;\n"
    "\tfloat ao;\n"
    "\tfloat s;\n"
    "\tfloat fade = 1.0;\n"
    "\tfor (int i = 0 ; i < rings; i += 1)\n"
    "\t{\n"
    "\t\tfade *= 0.5;\n"
    "\t\tfor (int j = 0 ; j < samples*i; j += 1)\n"
    "\t\t{\n"
    "\t\t\tfloat step = PI*2.0 / (float(samples*i));\n"
    "\t\t\tpw = (cos(float(j)*step)*float(i));\n"
    "\t\t\tph = (sin(float(j)*step)*float(i))*aspect;\n"
    "\t\t\td = readDepth( vec2(elf_TexCoord.s+pw*w,elf_TexCoord.t+ph*h));\n"
    "\t\t\tao += compareDepths(depth,d)*fade;\n"
    "\t\t\ts += 1.0*fade;\n"
    "\t\t}\n"
    "\t}\n"
    "\tao /= s;\n"
    "\tao = 1.0-ao;\n"
    "\tvec3 color = texture2D(elf_Texture0,elf_TexCoord).rgb;\n"
    "\tvec3 luminance = vec3(color.r*0.3+color.g*0.59+color.b*0.11)-(amount-1.0);\n"
    "\tvec3 black = vec3(0.0,0.0,0.0);\n"
    "\tvec3 treshold = vec3(0.2,0.2,0.2);\n"
    "\tluminance = "
    "clamp(max(black,luminance-treshold)+max(black,luminance-treshold)+max(black,luminance-treshold),0.0,1.0);\n"
    "\tgl_FragColor = vec4(color.rgb*mix(vec3(ao,ao,ao).rgb,vec3(1.0,1.0,1.0),luminance),1.0);\n"
    "}\n";

const char* ssaoCombineShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform sampler2D elf_Texture1;\n"
    "varying vec2 elf_TexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "\tvec4 ssao = texture2D(elf_Texture1, elf_TexCoord);\n"
    "\tvec4 col = texture2D(elf_Texture0, elf_TexCoord);\n"
    "\tgl_FragColor = vec4(col.rgb*ssao.rgb, col.a);\n"
    "}\n";

const char* lightShaftShader =
    "uniform sampler2D elf_Texture0;\n"
    "uniform float exposure;\n"
    "uniform float decay;\n"
    "uniform float density;\n"
    "uniform float weight;\n"
    "uniform vec2 lightPosition;\n"
    "varying vec2 elf_TexCoord;\n"
    "const int NUM_SAMPLES = 100;\n"
    "void main()\n"
    "{\n"
    "\tvec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "\tvec2 deltaTextCoord = vec2(elf_TexCoord.st - lightPosition.xy);\n"
    "\tvec2 textCoord = elf_TexCoord.st;\n"
    "\tdeltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;\n"
    "\tfloat illuminationDecay = 1.0;\n"
    "\tfor(int i=0; i < NUM_SAMPLES ; i++)\n"
    "\t{\n"
    "\t\ttextCoord -= deltaTextCoord;\n"
    "\t\tvec4 sample = texture2D(elf_Texture0, textCoord);\n"
    "\t\tsample *= illuminationDecay * weight;\n"
    "\t\tfinalColor += sample;\n"
    "\t\tilluminationDecay *= decay;\n"
    "\t}\n"
    "\tgl_FragColor = finalColor*exposure;\n"
    "}\n";

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

void elfRunPostProcess(elfPostProcess* postProcess, elfScene* scene)
{
    unsigned int sourceRt = 0;
    elfCamera* cam;
    elfEntity* ent;
    elfSprite* spr;
    elfLight* light;
    int i;
    elfVec3f lightPos;
    elfVec3f lightScreenPos;
    elfVec3f camPos;
    elfVec4f camOrient;
    int viewport[4];
    unsigned char firstShaft;

    gfxDisableRenderTarget();

    gfxSetShaderParamsDefault(&postProcess->shaderParams);
    postProcess->shaderParams.renderParams.depthTest = GFX_FALSE;
    postProcess->shaderParams.renderParams.depthWrite = GFX_FALSE;
    postProcess->shaderParams.renderParams.alphaTest = GFX_FALSE;

    // SSAO
    if (postProcess->ssao && scene->curCamera)
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
        gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f,
                                           1.0f, postProcess->shaderParams.projectionMatrix);

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

        sourceRt = !sourceRt;

        postProcess->shaderParams.textureParams[0].texture = NULL;
        postProcess->shaderParams.textureParams[1].texture = NULL;
    }

    // DOF
    if (postProcess->dof)
    {
        gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);
        gfxGetOrthographicProjectionMatrix(0, postProcess->bufferWidth * 2, 0, postProcess->bufferHeight * 2, -1.0f,
                                           1.0f, postProcess->shaderParams.projectionMatrix);

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
        gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f,
                                           1.0f, postProcess->shaderParams.projectionMatrix);

        gfxMatrix4GetInverse(scene->curCamera->projectionMatrix, postProcess->shaderParams.invProjectionMatrix);

        postProcess->shaderParams.shaderProgram = postProcess->dofCombineShdr;
        postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtDepth;
        postProcess->shaderParams.textureParams[1].texture = postProcess->mainRtColor[0];
        postProcess->shaderParams.textureParams[2].texture = postProcess->rtTexHigh_1;

        gfxSetShaderParams(&postProcess->shaderParams);

        gfxSetShaderProgramUniform1f("elf_FocalRange", postProcess->dofFocalRange);
        gfxSetShaderProgramUniform1f("elf_FocalDistance", postProcess->dofFocalDistance);

        elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

        sourceRt = !sourceRt;

        postProcess->shaderParams.textureParams[0].texture = NULL;
        postProcess->shaderParams.textureParams[1].texture = NULL;
        postProcess->shaderParams.textureParams[2].texture = NULL;
    }

    // BLOOM
    if (postProcess->bloom)
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
        gfxGetOrthographicProjectionMatrix(0.0f, postProcess->bufferWidth / 2, 0.0f, postProcess->bufferHeight / 2,
                                           -1.0f, 1.0f, postProcess->shaderParams.projectionMatrix);

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
        gfxGetOrthographicProjectionMatrix(0.0f, postProcess->bufferWidth / 4, 0.0f, postProcess->bufferHeight / 4,
                                           -1.0f, 1.0f, postProcess->shaderParams.projectionMatrix);

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
        gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f,
                                           1.0f, postProcess->shaderParams.projectionMatrix);

        postProcess->shaderParams.shaderProgram = postProcess->bloomCombineShdr;
        postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
        postProcess->shaderParams.textureParams[1].texture = postProcess->rtTexMed_3;
        postProcess->shaderParams.textureParams[2].texture = postProcess->rtTexLow_2;
        postProcess->shaderParams.textureParams[3].texture = postProcess->rtTexTiny_2;
        gfxSetShaderParams(&postProcess->shaderParams);

        elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

        postProcess->shaderParams.textureParams[0].texture = NULL;
        postProcess->shaderParams.textureParams[1].texture = NULL;
        postProcess->shaderParams.textureParams[2].texture = NULL;
        postProcess->shaderParams.textureParams[3].texture = NULL;
    }

    if (!postProcess->bloom && !postProcess->dof && !postProcess->ssao)
    {
        gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
        gfxGetOrthographicProjectionMatrix(0.0f, (float)elfGetWindowWidth(), 0.0f, (float)elfGetWindowHeight(), -1.0f,
                                           1.0f, postProcess->shaderParams.projectionMatrix);

        postProcess->shaderParams.textureParams[0].texture = postProcess->mainRtColor[sourceRt];
        gfxSetShaderParams(&postProcess->shaderParams);

        elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

        postProcess->shaderParams.textureParams[0].texture = NULL;
    }

    // LIGHT SHAFTS
    firstShaft = ELF_TRUE;

    if (postProcess->lightShafts && scene->curCamera)
    {
        for (light = (elfLight*)elfBeginList(scene->lights); light; light = (elfLight*)elfGetListNext(scene->lights))
        {
            lightPos = elfGetActorPosition((elfActor*)light);
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

                    scene->shaderParams.renderParams.colorWrite = ELF_FALSE;
                    scene->shaderParams.renderParams.alphaWrite = ELF_FALSE;

                    for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue);
                         i < scene->entityQueueCount && ent != NULL;
                         i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
                    {
                        elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                    }

                    for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue);
                         i < scene->spriteQueueCount && spr != NULL;
                         i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
                    {
                        elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                    }

                    firstShaft = ELF_FALSE;
                }

                camPos = elfGetActorPosition((elfActor*)scene->curCamera);
                camOrient = elfGetActorOrientation((elfActor*)scene->curCamera);
                viewport[0] = 0;
                viewport[1] = 0;
                viewport[2] = elfGetWindowWidth();
                viewport[3] = elfGetWindowHeight();
                gfxProject(lightPos.x, lightPos.y, lightPos.z, elfGetCameraModelviewMatrix(scene->curCamera),
                           elfGetCameraProjectionMatrix(scene->curCamera), viewport, &lightScreenPos.x);

                lightPos = elfSubVec3fVec3f(lightPos, camPos);
                camOrient = elfGetQuaInverted(camOrient);
                lightPos = elfMulQuaVec3f(camOrient, lightPos);

                // render the light beacon
                gfxSetShaderParamsDefault(&scene->shaderParams);

                elfSetCamera(scene->curCamera, &scene->shaderParams);
                gfxSetViewport(0, 0, postProcess->bufferWidth * 2, postProcess->bufferHeight * 2);

                scene->shaderParams.renderParams.depthWrite = GFX_FALSE;

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

                scene->shaderParams.renderParams.depthTest = ELF_FALSE;
                scene->shaderParams.renderParams.depthWrite = ELF_FALSE;
                scene->shaderParams.shaderProgram = postProcess->lightShaftShdr;
                scene->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_1;

                gfxSetShaderParams(&scene->shaderParams);
                gfxSetShaderProgramUniform1f("exposure", 0.0034f);
                gfxSetShaderProgramUniform1f("decay", 1.0f);
                gfxSetShaderProgramUniform1f("density", 1.0f - light->shaftFadeOff);
                gfxSetShaderProgramUniform1f("weight",
                                             light->shaftIntensity * 5.0f * postProcess->lightShaftsIntensity);
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

                scene->shaderParams.renderParams.depthTest = GFX_FALSE;
                scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
                scene->shaderParams.renderParams.blendMode = GFX_ADD;
                scene->shaderParams.textureParams[0].texture = postProcess->rtTexHigh_2;

                gfxSetShaderParams(&scene->shaderParams);

                elfDrawTextured2dQuad(0.0f, 0.0f, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

                scene->shaderParams.textureParams[0].texture = NULL;
            }
        }
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&postProcess->shaderParams);
    gfxSetShaderParams(&postProcess->shaderParams);

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
    postProcess->bloom = ELF_TRUE;
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

void elfDisablePostProcessBloom(elfPostProcess* postProcess) { postProcess->bloom = ELF_FALSE; }

float elfGetPostProcessBloomThreshold(elfPostProcess* postProcess) { return postProcess->bloomThreshold; }

void elfSetPostProcessDof(elfPostProcess* postProcess, float focalRange, float focalDistance)
{
    postProcess->dof = ELF_TRUE;
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

void elfDisablePostProcessDof(elfPostProcess* postProcess) { postProcess->dof = ELF_FALSE; }

float elfGetPostProcessDofFocalRange(elfPostProcess* postProcess) { return postProcess->dofFocalRange; }

float elfGetPostProcessDofFocalDistance(elfPostProcess* postProcess) { return postProcess->dofFocalDistance; }

void elfSetPostProcessSsao(elfPostProcess* postProcess, float amount)
{
    postProcess->ssao = ELF_TRUE;
    postProcess->ssaoAmount = amount;

    if (!postProcess->mainRtColor[1] && (int)postProcess->bloom + (int)postProcess->dof + (int)postProcess->ssao > 1)
    {
        postProcess->mainRtColor[1] = gfxCreate2dTexture(elfGetWindowWidth(), elfGetWindowHeight(), 0.0f, GFX_CLAMP,
                                                         GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
        gfxIncRef((gfxObject*)postProcess->mainRtColor[1]);
    }
}

void elfDisablePostProcessSsao(elfPostProcess* postProcess) { postProcess->ssao = ELF_FALSE; }

float elfGetPostProcessSsaoAmount(elfPostProcess* postProcess) { return postProcess->ssaoAmount; }

void elfSetPostProcessLightShafts(elfPostProcess* postProcess, float intensity)
{
    postProcess->lightShafts = ELF_TRUE;
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

void elfDisablePostProcessLightShafts(elfPostProcess* postProcess) { postProcess->lightShafts = ELF_FALSE; }

float elfGetPostProcessLightShaftsIntensity(elfPostProcess* postProcess) { return postProcess->lightShaftsIntensity; }

unsigned char elfIsPostProcessBloom(elfPostProcess* postProcess) { return postProcess->bloom; }

unsigned char elfIsPostProcessSsao(elfPostProcess* postProcess) { return postProcess->ssao; }

unsigned char elfIsPostProcessDof(elfPostProcess* postProcess) { return postProcess->dof; }

unsigned char elfIsPostProcessLightShafts(elfPostProcess* postProcess) { return postProcess->lightShafts; }
