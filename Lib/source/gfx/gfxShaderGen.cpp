#include "nelf/gfx/gfxShaderGen.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

#include "gfx/gfxLightType.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxMaxTextures.h"
#include "nelf/gfx/gfxShaderConfig.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTextureMapType.h"

enum GBufferMode
{
    GFX_GBUFFER_DEPTH,
    GFX_GBUFFER_FILL,
    GFX_GBUFFER_LIGHTING
};

char* gfxCreateString(const char* str)
{
    char* nstr;

    if (!str || !strlen(str))
        return NULL;

    nstr = (char*)malloc(strlen(str) + 1);
    memset(nstr, 0x0, sizeof(char) * (strlen(str) + 1));

    memcpy(nstr, str, sizeof(char) * strlen(str));

    return nstr;
}

void gfxDestroyString(char* str) { free(str); }

gfxLine* gfxCreateLine()
{
    gfxLine* line;

    line = (gfxLine*)malloc(sizeof(gfxLine));
    memset(line, 0x0, sizeof(gfxLine));

    return line;
}

void gfxDestroyLine(gfxLine* line)
{
    if (line->text)
        gfxDestroyString(line->text);

    free(line);
}

void gfxDestroyLines(gfxLine* line)
{
    if (line->next)
        gfxDestroyLines(line->next);

    gfxDestroyLine(line);
}

gfxDocument* gfxCreateDocument()
{
    gfxDocument* document;

    document = (gfxDocument*)malloc(sizeof(gfxDocument));
    memset(document, 0x0, sizeof(gfxDocument));

    return document;
}

void gfxDestroyDocument(gfxDocument* document)
{
    if (document->lines)
        gfxDestroyLines(document->lines);

    free(document);
}

void gfxClearDocument(gfxDocument* document)
{
    if (document->lines)
        gfxDestroyLines(document->lines);

    memset(document, 0x0, sizeof(gfxDocument));
}

void gfxAddDocumentLine(gfxDocument* document, const char* str)
{
    gfxLine* line;
    gfxLine* lline;

    if (!str || !strlen(str))
        return;

    line = gfxCreateLine();

    line->text = gfxCreateString(str);

    if (!document->lines)
        document->lines = line;
    else
    {
        lline = document->lines;
        while (lline->next) lline = lline->next;
        lline->next = line;
    }

    document->numLines++;
    document->numChars += strlen(str) + 1;
}

unsigned int gfxGetDocumentLines(gfxDocument* document) { return document->numLines; }

unsigned int gfxGetDocumentChars(gfxDocument* document) { return document->numChars; }

void gfxDocumentToBuffer(gfxDocument* document, char* buf)
{
    int offset;
    gfxLine* line;

    offset = 0;
    line = document->lines;

    while (line)
    {
        memcpy(&buf[offset], line->text, strlen(line->text));
        offset += strlen(line->text);
        buf[offset] = '\n';
        offset++;
        line = line->next;
    }
}

void gfxGetShaderProgramConfig(gfxShaderParams* shaderParams, gfxShaderConfig* shaderConfig)
{
    int i;

    memset(shaderConfig, 0x0, sizeof(gfxShaderConfig));

    for (i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        if (shaderParams->textureParams[i].texture)
        {
            shaderConfig->textures |= shaderParams->textureParams[i].type;
        }
    }

    shaderConfig->light = shaderParams->lightParams.type;
    shaderConfig->vertexColor = shaderParams->renderParams.vertexColor;
    shaderConfig->specular = false;
    if ((shaderParams->materialParams.specularColor.r > 0.0001f ||
         shaderParams->materialParams.specularColor.g > 0.0001f ||
         shaderParams->materialParams.specularColor.b > 0.0001f) &&
        shaderParams->materialParams.shininess > 0.0001f)
    {
        shaderConfig->specular = true;
    }

    shaderConfig->gbuffer = shaderParams->gbufferMode;
    shaderConfig->fog = shaderParams->fogParams.mode;
    shaderConfig->blend = shaderParams->renderParams.blendMode;
}

void gfxAddVertexAttributes(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "attribute vec3 elf_VertexAttr;");
    if (config->light || config->textures & GFX_HEIGHT_MAP || config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "attribute vec3 elf_NormalAttr;");
    if (config->textures)
        gfxAddDocumentLine(document, "attribute vec2 elf_TexCoordAttr;");
    if ((config->light && config->textures & GFX_NORMAL_MAP) || config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "attribute vec3 elf_TangentAttr;");
    if (config->vertexColor)
        gfxAddDocumentLine(document, "attribute vec4 elf_ColorAttr;");
}

void gfxAddVertexUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "uniform mat4 elf_ProjectionMatrix;");
    gfxAddDocumentLine(document, "uniform mat4 elf_ModelviewMatrix;");
    if (config->light || config->textures & GFX_HEIGHT_MAP || config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "uniform mat3 elf_NormalMatrix;");
    if (config->light)
        gfxAddDocumentLine(document, "uniform vec3 elf_LightPosition;");
    if (config->light == GFX_SPOT_LIGHT || config->light == GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "uniform vec3 elf_LightSpotDirection;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "uniform mat4 elf_ShadowProjectionMatrix;");
    if (config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "uniform vec3 elf_CameraPosition;");
}

void gfxAddVertexVaryings(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light || config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_EyeVector;");
    if (config->light && config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_LightTSDirection;");
    if (config->light && !(config->textures & GFX_NORMAL_MAP))
        gfxAddDocumentLine(document, "varying vec3 elf_Normal;");
    if (config->light)
        gfxAddDocumentLine(document, "varying vec3 elf_LightDirection;");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "varying float elf_Distance;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "varying vec4 elf_ShadowCoord;");
    if (config->textures && config->textures != GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
    if (config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_CubeMapCoord;");
    if (config->vertexColor)
        gfxAddDocumentLine(document, "varying vec4 elf_VertexColor;");
}

void gfxAddVertexInit(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "void main()");
    gfxAddDocumentLine(document, "{");
    gfxAddDocumentLine(document, "\tvec4 vertex = elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);");
}

void gfxAddVertexTextureCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures && config->textures != GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "\telf_TexCoord = elf_TexCoordAttr;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "\telf_ShadowCoord = elf_ShadowProjectionMatrix*vertex;");
}

void gfxAddVertexLightingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light && !(config->textures & GFX_HEIGHT_MAP))
        gfxAddDocumentLine(document, "\telf_EyeVector = -vertex.xyz;");
    if ((config->light && config->textures & GFX_NORMAL_MAP) || config->textures & GFX_HEIGHT_MAP)
    {
        if (config->light && !(config->textures & GFX_NORMAL_MAP) && config->textures & GFX_HEIGHT_MAP)
            gfxAddDocumentLine(document, "\telf_Normal = vec3(elf_ModelviewMatrix*vec4(elf_NormalAttr, 0.0));");
        else
            gfxAddDocumentLine(document, "\tvec3 elf_Normal = elf_NormalMatrix*elf_NormalAttr;");

        gfxAddDocumentLine(document, "\tvec3 elf_Tangent = elf_NormalMatrix*elf_TangentAttr;");
        gfxAddDocumentLine(document, "\tvec3 elf_BiNormal = cross(elf_Normal, elf_Tangent);");
        gfxAddDocumentLine(document, "\tvec3 tmpvec = -vertex.xyz;");
        gfxAddDocumentLine(document, "\telf_EyeVector.x = dot(tmpvec, elf_Tangent);");
        gfxAddDocumentLine(document, "\telf_EyeVector.y = dot(tmpvec, elf_BiNormal);");
        gfxAddDocumentLine(document, "\telf_EyeVector.z = dot(tmpvec, elf_Normal);");
    }
    if (config->light)
    {
        if (config->textures & GFX_NORMAL_MAP)
        {
            if (config->light != GFX_SUN_LIGHT)
                gfxAddDocumentLine(document, "\ttmpvec = elf_LightPosition-vertex.xyz;");
            if (config->light == GFX_SUN_LIGHT)
                gfxAddDocumentLine(document, "\ttmpvec = -elf_LightSpotDirection;");
            gfxAddDocumentLine(document, "\telf_LightDirection = tmpvec;");
            gfxAddDocumentLine(document, "\telf_LightTSDirection.x = dot(tmpvec, elf_Tangent);");
            gfxAddDocumentLine(document, "\telf_LightTSDirection.y = dot(tmpvec, elf_BiNormal);");
            gfxAddDocumentLine(document, "\telf_LightTSDirection.z = dot(tmpvec, elf_Normal);");
        }
        else
        {
            if (!(config->textures & GFX_HEIGHT_MAP))
                gfxAddDocumentLine(document, "\telf_Normal = elf_NormalMatrix*elf_NormalAttr;");
            if (config->light != GFX_SUN_LIGHT)
                gfxAddDocumentLine(document, "\telf_LightDirection = elf_LightPosition-vertex.xyz;");
            if (config->light == GFX_SUN_LIGHT)
                gfxAddDocumentLine(document, "\telf_LightDirection = -elf_LightSpotDirection;");
        }
    }
    if (config->textures & GFX_CUBE_MAP)
    {
        if (!config->light && !(config->textures & GFX_HEIGHT_MAP))
        {
            gfxAddDocumentLine(document, "\tvec3 elf_Normal = elf_NormalMatrix*elf_NormalAttr;");
        }
        gfxAddDocumentLine(document, "\telf_CubeMapCoord = reflect(-vertex.xyz, elf_Normal);");
    }
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "\telf_Distance = length(elf_LightDirection);");
}

void gfxAddVertexEnd(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->vertexColor)
        gfxAddDocumentLine(document, "\telf_VertexColor = elf_ColorAttr;");
    gfxAddDocumentLine(document, "\tgl_Position = elf_ProjectionMatrix*vertex;");
    gfxAddDocumentLine(document, "}");
}

void gfxAddFragmentTextureUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_COLOR_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_ColorMap;");
    if (config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_NormalMap;");
    if (config->textures & GFX_SPECULAR_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_SpecularMap;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_HeightMap;");
    if (config->textures & GFX_COLOR_RAMP_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_ColorRampMap;");
    if (config->textures & GFX_LIGHT_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_LightMap;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "uniform sampler2DShadow elf_ShadowMap;");
    if (config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "uniform samplerCube elf_CubeMap;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "uniform float elf_ParallaxScale;");
}

void gfxAddFragmentFogUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->fog)
    {
        gfxAddDocumentLine(document, "uniform float elf_FogStart;");
        gfxAddDocumentLine(document, "uniform float elf_FogEnd;");
        gfxAddDocumentLine(document, "uniform vec3 elf_FogColor;");
    }
}

void gfxAddFragmentMaterialUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "uniform vec4 elf_DiffuseColor;");
    if (config->light)
    {
        gfxAddDocumentLine(document, "uniform vec3 elf_SpecularColor;");
        gfxAddDocumentLine(document, "uniform float elf_Shininess;");
    }
}

void gfxAddFragmentLightingUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light)
        gfxAddDocumentLine(document, "uniform vec3 elf_LightColor;");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "uniform float elf_LightRange;");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "uniform float elf_LightFadeRange;");
    if (config->light == GFX_SPOT_LIGHT)
        gfxAddDocumentLine(document, "uniform vec3 elf_LightSpotDirection;");
    if (config->light == GFX_SPOT_LIGHT)
        gfxAddDocumentLine(document, "uniform float elf_LightInnerConeCos;");
    if (config->light == GFX_SPOT_LIGHT)
        gfxAddDocumentLine(document, "uniform float elf_LightOuterConeCos;");
}

void gfxAddFragmentVaryings(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures)
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "varying vec4 elf_ShadowCoord;");
    if (config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_CubeMapCoord;");
    if (config->light || config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_EyeVector;");
    if (config->light && config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_LightTSDirection;");
    if (config->light && !(config->textures & GFX_NORMAL_MAP))
        gfxAddDocumentLine(document, "varying vec3 elf_Normal;");
    if (config->light)
        gfxAddDocumentLine(document, "varying vec3 elf_LightDirection;");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "varying float elf_Distance;");
    if (config->vertexColor)
        gfxAddDocumentLine(document, "varying vec4 elf_VertexColor;");
}

void gfxAddFragmentInit(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "void main()");
    gfxAddDocumentLine(document, "{");
    gfxAddDocumentLine(document, "\tvec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);");
    if (config->light)
    {
        gfxAddDocumentLine(document, "\tvec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);");
        gfxAddDocumentLine(document, "\tvec3 specular = vec3(0.0, 0.0, 0.0);");
    }
}

void gfxAddFragmentShadowCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light && config->textures & GFX_SHADOW_MAP)
    {
        gfxAddDocumentLine(document, "\tfloat shadow = max(shadow2DProj(elf_ShadowMap, elf_ShadowCoord).r, 0.0);");
        gfxAddDocumentLine(document, "\tif(shadow < 0.001) discard;");
    }
}

void gfxAddFragmentParallaxMappingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light || config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "\tvec3 E = normalize(elf_EyeVector);");
    if (config->textures & GFX_HEIGHT_MAP)
    {
        gfxAddDocumentLine(document, "\tfloat depth = texture2D(elf_HeightMap, elf_TexCoord).r;");
        gfxAddDocumentLine(document, "\tvec2 elf_HeightTexCoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document,
                           "\tdepth = (depth+texture2D(elf_HeightMap, elf_TexCoord+elf_HeightTexCoord).r)*0.5;");
        gfxAddDocumentLine(document, "\telf_HeightTexCoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document,
                           "\tdepth = (depth+texture2D(elf_HeightMap, elf_TexCoord+elf_HeightTexCoord).r)*0.5;");
        gfxAddDocumentLine(document, "\telf_HeightTexCoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document, "\telf_HeightTexCoord = elf_TexCoord+elf_HeightTexCoord;");
    }
}

void gfxAddFragmentPreLightingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light && config->textures & GFX_NORMAL_MAP && config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "\tvec3 N = normalize(texture2D(elf_NormalMap, elf_HeightTexCoord).rgb*2.0-1.0);");
    if (config->light && config->textures & GFX_NORMAL_MAP && !(config->textures & GFX_HEIGHT_MAP))
        gfxAddDocumentLine(document, "\tvec3 N = normalize(texture2D(elf_NormalMap, elf_TexCoord).rgb*2.0-1.0);");
    if (config->light && config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(
            document, "\tvec3 L = elf_LightTSDirection*inversesqrt(dot(elf_LightTSDirection, elf_LightTSDirection));");
    if (config->light && !(config->textures & GFX_NORMAL_MAP))
        gfxAddDocumentLine(document, "\tvec3 N = normalize(elf_Normal);");
    if (config->light && !(config->textures & GFX_NORMAL_MAP))
        gfxAddDocumentLine(document, "\tvec3 L = normalize(elf_LightDirection);");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(
            document, "\tfloat att = clamp(1.0-max(elf_Distance-elf_LightRange, 0.0)/elf_LightFadeRange, 0.0, 1.0);");
    if (config->light == GFX_SPOT_LIGHT)
    {
        gfxAddDocumentLine(document, "\tvec3 D = normalize(elf_LightSpotDirection);");
        if (config->textures & GFX_NORMAL_MAP)
            gfxAddDocumentLine(document, "\tfloat cosCurAngle = dot(-normalize(elf_LightDirection), D);");
        else
            gfxAddDocumentLine(document, "\tfloat cosCurAngle = dot(-L, D);");
        gfxAddDocumentLine(document, "\tfloat cosInnerConeAngle = elf_LightInnerConeCos;");
        gfxAddDocumentLine(document, "\tfloat cosOuterConeAngle = elf_LightOuterConeCos;");
        gfxAddDocumentLine(document, "\tfloat cosInnerMinusOuterConeAngle = cosInnerConeAngle-cosOuterConeAngle;");
        gfxAddDocumentLine(document, "\tfloat spot = 0.0;");
        gfxAddDocumentLine(document,
                           "\tspot = clamp((cosCurAngle-cosOuterConeAngle) / cosInnerMinusOuterConeAngle, 0.0, 1.0);");
    }
}

void gfxAddFragmentLightingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light)
    {
        gfxAddDocumentLine(document, "\tfloat lambertTerm = max(dot(N, L), 0.0);");
        gfxAddDocumentLine(document, "\tif(lambertTerm > 0.0)");
        gfxAddDocumentLine(document, "\t{");
        gfxAddDocumentLine(document, "\t\tdiffuse = elf_DiffuseColor*vec4(elf_LightColor*lambertTerm, 1.0);");
        if (config->specular)
        {
            gfxAddDocumentLine(document, "\t\tvec3 R = reflect(-L, N);");
            gfxAddDocumentLine(document,
                               "\t\tfloat specStrength = clamp(pow(max(dot(R, E), 0.0), elf_Shininess), 0.0, 1.0);");
            gfxAddDocumentLine(document, "\t\tspecular = elf_SpecularColor*elf_LightColor*specStrength;");
        }
        gfxAddDocumentLine(document, "\t}");
    }
    if (!config->light)
        gfxAddDocumentLine(document, "\tfinalColor *= elf_DiffuseColor;");
}

void gfxAddFragmentTextureCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_HEIGHT_MAP)
    {
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "\tfinalColor *= texture2D(elf_ColorMap, elf_HeightTexCoord);");
        if (config->light && config->textures & GFX_COLOR_RAMP_MAP)
            gfxAddDocumentLine(document,
                               "\tfinalColor.rgb *= texture2D(elf_ColorRampMap, vec2(clamp(lambertTerm, 0.0, "
                               "1.0))).rgb*elf_DiffuseColor*elf_LightColor;");
        if (config->textures & GFX_LIGHT_MAP)
            gfxAddDocumentLine(document, "\tfinalColor.rgb *= texture2D(elf_LightMap, elf_HeightTexCoord).rgb;");
    }
    else
    {
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "\tfinalColor *= texture2D(elf_ColorMap, elf_TexCoord);");
        if (config->light && config->textures & GFX_COLOR_RAMP_MAP)
            gfxAddDocumentLine(document,
                               "\tfinalColor.rgb *= texture2D(elf_ColorRampMap, vec2(clamp(lambertTerm, 0.0, "
                               "1.0))).rgb*elf_DiffuseColor*elf_LightColor;");
        if (config->textures & GFX_LIGHT_MAP)
            gfxAddDocumentLine(document, "\tfinalColor.rgb *= texture2D(elf_LightMap, elf_TexCoord).rgb;");
    }
    if (config->textures & GFX_CUBE_MAP)
        gfxAddDocumentLine(document, "\tfinalColor *= textureCube(elf_CubeMap, elf_CubeMapCoord);");
}

void gfxAddFragmentPostLightingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->light && !(config->textures & GFX_COLOR_RAMP_MAP))
        gfxAddDocumentLine(document, "\tfinalColor.rgb *= diffuse.rgb;");
    if (config->light && config->specular && config->textures & GFX_SPECULAR_MAP &&
        !(config->textures & GFX_HEIGHT_MAP))
        gfxAddDocumentLine(document, "\tfinalColor.rgb += specular*texture2D(elf_SpecularMap, elf_TexCoord).rgb;");
    if (config->light && config->specular && config->textures & GFX_SPECULAR_MAP && config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document,
                           "\tfinalColor.rgb += specular*texture2D(elf_SpecularMap, elf_HeightTexCoord).rgb;");
    if (config->light && config->specular && !(config->textures & GFX_SPECULAR_MAP))
        gfxAddDocumentLine(document, "\tfinalColor.rgb += specular;");
    if (config->light && config->light != GFX_SUN_LIGHT)
        gfxAddDocumentLine(document, "\tfinalColor.rgb *= att;");
    if (config->light && config->textures & GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "\tfinalColor.rgb *= shadow;");
    if (config->light == GFX_SPOT_LIGHT)
        gfxAddDocumentLine(document, "\tfinalColor.rgb *= spot;");
}

void gfxAddFragmentEnd(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->fog)
    {
        if (config->blend == GFX_ADD)
            gfxAddDocumentLine(
                document,
                "\tfinalColor.rgb = mix(elf_FogColor*(finalColor.r*0.3+finalColor.g*0.59+finalColor.b*0.11), "
                "finalColor.rgb, clamp((elf_FogEnd-gl_FragCoord.z/gl_FragCoord.w)/(elf_FogEnd-elf_FogStart), 0.0, "
                "1.0));");
        else
            gfxAddDocumentLine(
                document,
                "\tfinalColor.rgb = mix(elf_FogColor, finalColor.rgb, "
                "clamp((elf_FogEnd-gl_FragCoord.z/gl_FragCoord.w)/(elf_FogEnd-elf_FogStart), 0.0, 1.0));");
    }
    if (config->vertexColor)
        gfxAddDocumentLine(document, "\tfinalColor *= elf_VertexColor;");
    gfxAddDocumentLine(document, "\tgl_FragColor = finalColor;");
    gfxAddDocumentLine(document, "}");
}

gfxShaderProgram* gfxGetShaderProgram(gfxShaderConfig* config)
{
    assert(driver != nullptr);

    gfxDocument* document;
    gfxShaderProgram* shaderProgram;
    gfxShaderProgram* lshpr;
    char* vertShdr;
    char* fragShdr;

    shaderProgram = driver->shaderPrograms;
    while (shaderProgram)
    {
        if (!memcmp(&shaderProgram->config, config, sizeof(gfxShaderConfig)))
            return shaderProgram;
        shaderProgram = shaderProgram->next;
    }

    document = gfxCreateDocument();

    // --------------------- VERTEX SHADER --------------------- //

    gfxAddDocumentLine(document, "#version 120");
    gfxAddVertexAttributes(document, config);
    gfxAddVertexUniforms(document, config);
    gfxAddVertexVaryings(document, config);
    gfxAddVertexInit(document, config);
    gfxAddVertexTextureCalcs(document, config);
    gfxAddVertexLightingCalcs(document, config);
    gfxAddVertexEnd(document, config);

    vertShdr = (char*)malloc(sizeof(char) * gfxGetDocumentChars(document) + 1);
    gfxDocumentToBuffer(document, vertShdr);
    vertShdr[gfxGetDocumentChars(document)] = '\0';
    // elfLogWrite("------------ VERTEX SHADER ------------\n");
    // elfLogWrite(vertShdr);

    gfxClearDocument(document);

    // --------------------- FRAGMENT SHADER --------------------- //

    gfxAddDocumentLine(document, "#version 120");
    gfxAddFragmentTextureUniforms(document, config);
    gfxAddFragmentFogUniforms(document, config);
    gfxAddFragmentMaterialUniforms(document, config);
    gfxAddFragmentLightingUniforms(document, config);
    gfxAddFragmentVaryings(document, config);
    gfxAddFragmentInit(document, config);
    gfxAddFragmentShadowCalcs(document, config);
    gfxAddFragmentParallaxMappingCalcs(document, config);
    gfxAddFragmentPreLightingCalcs(document, config);
    gfxAddFragmentLightingCalcs(document, config);
    gfxAddFragmentTextureCalcs(document, config);
    gfxAddFragmentPostLightingCalcs(document, config);
    gfxAddFragmentEnd(document, config);

    fragShdr = (char*)malloc(sizeof(char) * (gfxGetDocumentChars(document) + 1));
    gfxDocumentToBuffer(document, fragShdr);
    fragShdr[gfxGetDocumentChars(document)] = '\0';
    // elfLogWrite("----------- FRAGMENT SHADER -----------\n");
    // elfLogWrite(fragShdr);

    // ----------------------------------------------------------- //

    gfxDestroyDocument(document);

    shaderProgram = gfxCreateShaderProgram(vertShdr, fragShdr);

    free(vertShdr);
    free(fragShdr);

    if (shaderProgram)
    {
        memcpy(&shaderProgram->config, config, sizeof(gfxShaderConfig));

        if (!driver->shaderPrograms)
            driver->shaderPrograms = shaderProgram;
        else
        {
            lshpr = driver->shaderPrograms;
            while (lshpr->next) lshpr = lshpr->next;
            lshpr->next = shaderProgram;
        }
    }

    return shaderProgram;
}

void gfxAddGbufVertexAttributes(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "attribute vec3 elf_VertexAttr;");
    gfxAddDocumentLine(document, "attribute vec3 elf_NormalAttr;");
    if (config->textures)
        gfxAddDocumentLine(document, "attribute vec2 elf_TexCoordAttr;");
    if (config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(document, "attribute vec3 elf_TangentAttr;");
    if (config->vertexColor)
        gfxAddDocumentLine(document, "attribute vec4 elf_ColorAttr;");
}

void gfxAddGbufVertexUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "uniform mat4 elf_ProjectionMatrix;");
    gfxAddDocumentLine(document, "uniform mat4 elf_ModelviewMatrix;");
    if (config->light || config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "uniform mat3 elf_NormalMatrix;");
}

void gfxAddGbufVertexVaryings(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "varying vec3 elf_Normal;");
    if (config->textures)
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_EyeVector;");
    if (config->textures & GFX_NORMAL_MAP || config->textures & GFX_HEIGHT_MAP)
    {
        gfxAddDocumentLine(document, "varying vec3 elf_BiNormal;");
        gfxAddDocumentLine(document, "varying vec3 elf_Tangent;");
    }
    if (config->vertexColor)
        gfxAddDocumentLine(document, "varying vec4 elf_VertexColor;");
}

void gfxAddGbufVertexInit(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "void main()");
    gfxAddDocumentLine(document, "{");
    gfxAddDocumentLine(document, "\tvec4 vertex = elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);");
}

void gfxAddGbufVertexTextureCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures && config->textures != GFX_SHADOW_MAP)
        gfxAddDocumentLine(document, "\telf_TexCoord = elf_TexCoordAttr;");
}

void gfxAddGbufVertexLightingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_NORMAL_MAP || config->textures & GFX_HEIGHT_MAP)
    {
        gfxAddDocumentLine(document, "\telf_Normal = elf_NormalMatrix*elf_NormalAttr;");
        gfxAddDocumentLine(document, "\telf_Tangent = elf_NormalMatrix*elf_TangentAttr;");
        gfxAddDocumentLine(document, "\telf_BiNormal = cross(elf_Normal, elf_Tangent);");
        if (config->textures & GFX_HEIGHT_MAP)
        {
            gfxAddDocumentLine(document, "\tvec3 tmpvec = -vertex.xyz;");
            gfxAddDocumentLine(document, "\telf_EyeVector.x = dot(tmpvec, elf_Tangent);");
            gfxAddDocumentLine(document, "\telf_EyeVector.y = dot(tmpvec, elf_BiNormal);");
            gfxAddDocumentLine(document, "\telf_EyeVector.z = dot(tmpvec, elf_Normal);");
        }
    }
    else
    {
        gfxAddDocumentLine(document, "\telf_Normal = elf_NormalMatrix*elf_NormalAttr;");
    }
}

void gfxAddGbufVertexEnd(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->vertexColor)
        gfxAddDocumentLine(document, "\telf_VertexColor = elf_ColorAttr;");
    gfxAddDocumentLine(document, "\tgl_Position = elf_ProjectionMatrix*vertex;");
    gfxAddDocumentLine(document, "}");
}

void gfxAddGbufFragmentTextureUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_COLOR_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_ColorMap;");
    if (config->textures & GFX_NORMAL_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_NormalMap;");
    if (config->textures & GFX_SPECULAR_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_SpecularMap;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_HeightMap;");
    if (config->textures & GFX_LIGHT_MAP)
        gfxAddDocumentLine(document, "uniform sampler2D elf_LightMap;");
}

void gfxAddGbufFragmentMaterialUniforms(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "uniform vec4 elf_DiffuseColor;");
    gfxAddDocumentLine(document, "uniform vec3 elf_SpecularColor;");
    gfxAddDocumentLine(document, "uniform float elf_Shininess;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "uniform float elf_ParallaxScale;");
}

void gfxAddGbufFragmentVaryings(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "varying vec3 elf_Normal;");
    if (config->textures)
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
    if (config->textures & GFX_HEIGHT_MAP)
        gfxAddDocumentLine(document, "varying vec3 elf_EyeVector;");
    if (config->textures & GFX_NORMAL_MAP || config->textures & GFX_HEIGHT_MAP)
    {
        gfxAddDocumentLine(document, "varying vec3 elf_BiNormal;");
        gfxAddDocumentLine(document, "varying vec3 elf_Tangent;");
    }
    if (config->vertexColor)
        gfxAddDocumentLine(document, "varying vec4 elf_VertexColor;");
}

void gfxAddGbufFragmentInit(gfxDocument* document, gfxShaderConfig* config)
{
    gfxAddDocumentLine(document, "void main()");
    gfxAddDocumentLine(document, "{");
}

void gfxAddGbufFragmentParallaxMappingCalcs(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_HEIGHT_MAP)
    {
        gfxAddDocumentLine(document, "\tvec3 E = normalize(elf_EyeVector);");
        gfxAddDocumentLine(document, "\tfloat depth = texture2D(elf_HeightMap, elf_TexCoord).r;");
        gfxAddDocumentLine(document, "\tvec2 texcoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document, "\tdepth = (depth+texture2D(elf_HeightMap, elf_TexCoord+texcoord).r)*0.5;");
        gfxAddDocumentLine(document, "\ttexcoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document, "\tdepth = (depth+texture2D(elf_HeightMap, elf_TexCoord+texcoord).r)*0.5;");
        gfxAddDocumentLine(document, "\ttexcoord = E.xy*depth*elf_ParallaxScale;");
        gfxAddDocumentLine(document, "\ttexcoord = elf_TexCoord+texcoord;");
    }
    else if (config->textures)
    {
        gfxAddDocumentLine(document, "\tvec2 texcoord = elf_TexCoord;");
    }
}

void gfxAddGbufFragmentEnd(gfxDocument* document, gfxShaderConfig* config)
{
    if (config->textures & GFX_NORMAL_MAP)
    {
        gfxAddDocumentLine(document, "\tvec3 normal = texture2D(elf_NormalMap, texcoord).rgb;");
        gfxAddDocumentLine(document, "\tnormal -= 0.5;");
        gfxAddDocumentLine(document,
                           "\tnormal = normalize(normal.x*elf_Tangent+normal.y*elf_BiNormal+normal.z*elf_Normal);");
        gfxAddDocumentLine(document, "\tgl_FragData[0].rg = normal.xy;");
    }
    else
    {
        gfxAddDocumentLine(document, "\tgl_FragData[0].rg = normalize(elf_Normal).rg;");
    }
    gfxAddDocumentLine(document, "\tvec4 color = elf_DiffuseColor;");
    if (config->textures & GFX_COLOR_MAP)
        gfxAddDocumentLine(document, "\tcolor *= texture2D(elf_ColorMap, texcoord);");
    if (config->textures & GFX_LIGHT_MAP)
        gfxAddDocumentLine(document, "\tcolor *= texture2D(elf_LightMap, texcoord);");
    gfxAddDocumentLine(document, "\tgl_FragData[1] = vec4(color.rgb, 1.0);");
    gfxAddDocumentLine(document, "\tgl_FragData[2] = vec4(elf_SpecularColor, elf_Shininess/255.0);");
    gfxAddDocumentLine(document, "}");
}

gfxShaderProgram* gfxGetGbufShaderProgram(gfxShaderConfig* config)
{
    assert(driver != nullptr);
    gfxDocument* document;
    gfxShaderProgram* shaderProgram;
    gfxShaderProgram* lshpr;
    char* vertShdr;
    char* fragShdr;

    shaderProgram = driver->shaderPrograms;
    while (shaderProgram)
    {
        if (!memcmp(&shaderProgram->config, config, sizeof(gfxShaderConfig)))
            return shaderProgram;
        shaderProgram = shaderProgram->next;
    }

    document = gfxCreateDocument();

    // --------------------- VERTEX SHADER --------------------- //

    if (config->gbuffer == GFX_GBUFFER_DEPTH)
    {
        gfxAddDocumentLine(document, "attribute vec3 elf_VertexAttr;");
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "attribute vec2 elf_TexCoordAttr;");
        gfxAddDocumentLine(document, "uniform mat4 elf_ProjectionMatrix;");
        gfxAddDocumentLine(document, "uniform mat4 elf_ModelviewMatrix;");
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
        gfxAddDocumentLine(document, "void main()");
        gfxAddDocumentLine(document, "{");
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "\telf_TexCoord = elf_TexCoordAttr;");
        gfxAddDocumentLine(document,
                           "\tgl_Position = elf_ProjectionMatrix*(elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0));");
        gfxAddDocumentLine(document, "}");
    }
    else if (config->gbuffer == GFX_GBUFFER_FILL)
    {
        gfxAddGbufVertexAttributes(document, config);
        gfxAddGbufVertexUniforms(document, config);
        gfxAddGbufVertexVaryings(document, config);
        gfxAddGbufVertexInit(document, config);
        gfxAddGbufVertexTextureCalcs(document, config);
        gfxAddGbufVertexLightingCalcs(document, config);
        gfxAddGbufVertexEnd(document, config);
    }
    else if (config->gbuffer == GFX_GBUFFER_LIGHTING)
    {
        gfxAddDocumentLine(document, "attribute vec3 elf_VertexAttr;");
        gfxAddDocumentLine(document, "attribute vec2 elf_TexCoordAttr;");
        gfxAddDocumentLine(document, "uniform mat4 elf_ModelviewMatrix;");
        gfxAddDocumentLine(document, "uniform mat4 elf_ProjectionMatrix;");
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
        gfxAddDocumentLine(document, "void main()");
        gfxAddDocumentLine(document, "{");
        gfxAddDocumentLine(document, "\telf_TexCoord = elf_TexCoordAttr;");
        gfxAddDocumentLine(document,
                           "\tgl_Position = elf_ProjectionMatrix*(elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0));");
        gfxAddDocumentLine(document, "}");
    }

    vertShdr = (char*)malloc(sizeof(char) * gfxGetDocumentChars(document) + 1);
    gfxDocumentToBuffer(document, vertShdr);
    vertShdr[gfxGetDocumentChars(document)] = '\0';
    // elfLogWrite("------------ VERTEX SHADER ------------\n");
    // elfLogWrite(vertShdr);

    gfxClearDocument(document);

    // --------------------- FRAGMENT SHADER --------------------- //

    if (config->gbuffer == GFX_GBUFFER_DEPTH)
    {
        if (config->textures & GFX_COLOR_MAP)
        {
            gfxAddDocumentLine(document, "uniform sampler2D elf_ColorMap;");
            gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
        }
        gfxAddDocumentLine(document, "void main()");
        gfxAddDocumentLine(document, "{");
        if (config->textures & GFX_COLOR_MAP)
            gfxAddDocumentLine(document, "\tgl_FragData[0] = texture2D(elf_ColorMap, elf_TexCoord);");
        gfxAddDocumentLine(document, "}");
    }
    else if (config->gbuffer == GFX_GBUFFER_FILL)
    {
        gfxAddGbufFragmentTextureUniforms(document, config);
        gfxAddGbufFragmentMaterialUniforms(document, config);
        gfxAddGbufFragmentVaryings(document, config);
        gfxAddGbufFragmentInit(document, config);
        gfxAddGbufFragmentParallaxMappingCalcs(document, config);
        gfxAddGbufFragmentEnd(document, config);
    }
    else if (config->gbuffer == GFX_GBUFFER_LIGHTING)
    {
        gfxAddDocumentLine(document, "uniform sampler2D elf_Texture0;");
        gfxAddDocumentLine(document, "uniform sampler2D elf_Texture1;");
        gfxAddDocumentLine(document, "uniform sampler2D elf_Texture2;");
        gfxAddDocumentLine(document, "uniform sampler2D elf_Texture3;");
        gfxAddDocumentLine(document, "uniform vec3 elf_LightPosition;");
        gfxAddDocumentLine(document, "uniform vec3 elf_LightColor;");
        gfxAddDocumentLine(document, "uniform float elf_LightRange;");
        gfxAddDocumentLine(document, "uniform float elf_LightFadeRange;");
        if (config->light == GFX_SPOT_LIGHT)
        {
            gfxAddDocumentLine(document, "uniform vec3 elf_LightSpotDirection;");
            gfxAddDocumentLine(document, "uniform float elf_LightInnerConeCos;");
            gfxAddDocumentLine(document, "uniform float elf_LightOuterConeCos;");
            if (config->textures & GFX_SHADOW_MAP)
            {
                gfxAddDocumentLine(document, "uniform sampler2DShadow elf_ShadowMap;");
                gfxAddDocumentLine(document, "uniform mat4 elf_ShadowProjectionMatrix;");
            }
        }
        else if (config->light == GFX_SUN_LIGHT)
        {
            gfxAddDocumentLine(document, "uniform vec3 elf_LightSpotDirection;");
        }
        gfxAddDocumentLine(document, "uniform mat4 elf_InvProjectionMatrix;");
        gfxAddDocumentLine(document, "varying vec2 elf_TexCoord;");
        gfxAddDocumentLine(document, "void main()");
        gfxAddDocumentLine(document, "{\n");
        gfxAddDocumentLine(document, "\tvec3 diffuse = vec3(0.0, 0.0, 0.0);");
        gfxAddDocumentLine(document, "\tvec3 specular = vec3(0.0, 0.0, 0.0);");
        gfxAddDocumentLine(document, "\tfloat depth = texture2D(elf_Texture0, elf_TexCoord).r*2.0-1.0;");
        gfxAddDocumentLine(document,
                           "\tvec4 vertex = elf_InvProjectionMatrix*vec4(elf_TexCoord.x*2.0-1.0, "
                           "elf_TexCoord.y*2.0-1.0, depth, 1.0);");
        gfxAddDocumentLine(document, "\tvertex = vec4(vertex.xyz/vertex.w, 1.0);");
        if (config->light == GFX_SPOT_LIGHT && config->textures & GFX_SHADOW_MAP)
        {
            gfxAddDocumentLine(document, "\tvec4 elf_ShadowCoord = elf_ShadowProjectionMatrix*vec4(vertex.xyz, 1.0);");
            gfxAddDocumentLine(document, "\tfloat shadow = shadow2DProj(elf_ShadowMap, elf_ShadowCoord).r;");
            gfxAddDocumentLine(document, "\tif(shadow < 0.001f) discard;");
        }
        if (config->light != GFX_SUN_LIGHT)
        {
            gfxAddDocumentLine(document, "\tvec3 elf_LightDirection = elf_LightPosition-vertex.xyz;");
        }
        else
        {
            gfxAddDocumentLine(document, "\tvec3 elf_LightDirection = -elf_LightSpotDirection;");
        }
        gfxAddDocumentLine(document, "\tfloat elf_Distance = length(elf_LightDirection);");
        gfxAddDocumentLine(document, "\tvec3 N = vec3(texture2D(elf_Texture1, elf_TexCoord).rg, 0.0);");
        gfxAddDocumentLine(document, "\tN = normalize(vec3(N.x, N.y, sqrt(1.0-N.x*N.x-N.y*N.y)));");
        gfxAddDocumentLine(document, "\tvec3 L = normalize(elf_LightDirection);");
        if (config->light == GFX_SPOT_LIGHT)
        {
            gfxAddDocumentLine(document, "\tvec3 D = normalize(elf_LightSpotDirection);");
            gfxAddDocumentLine(document, "\tfloat cosCurAngle = dot(-L, D);");
            gfxAddDocumentLine(document, "\tfloat cosInnerConeAngle = elf_LightInnerConeCos;");
            gfxAddDocumentLine(document, "\tfloat cosOuterConeAngle = elf_LightOuterConeCos;");
            gfxAddDocumentLine(document, "\tfloat cosInnerMinusOuterConeAngle = cosInnerConeAngle-cosOuterConeAngle;");
            gfxAddDocumentLine(document, "\tfloat spot = 0.0;");
            gfxAddDocumentLine(
                document, "\tspot = clamp((cosCurAngle-cosOuterConeAngle) / cosInnerMinusOuterConeAngle, 0.0, 1.0);");
        }
        gfxAddDocumentLine(
            document, "\tfloat att = clamp(1.0-max(elf_Distance-elf_LightRange, 0.0)/elf_LightFadeRange, 0.0, 1.0);");
        gfxAddDocumentLine(document, "\tfloat lambertTerm = max(dot(N, L), 0.0);");
        gfxAddDocumentLine(document, "\tif(lambertTerm > 0.0)");
        gfxAddDocumentLine(document, "\t{");
        gfxAddDocumentLine(document, "\t\tvec3 E = normalize(-vertex.xyz);");
        gfxAddDocumentLine(document, "\t\tvec3 R = reflect(-L, N);");
        gfxAddDocumentLine(document, "\t\tvec4 spec = texture2D(elf_Texture2, elf_TexCoord);");
        gfxAddDocumentLine(document, "\t\tspec.a = spec.a*255.0;");
        gfxAddDocumentLine(document, "\t\tif(spec.a > 0.0)");
        gfxAddDocumentLine(document, "\t\t{");
        gfxAddDocumentLine(document, "\t\t\tfloat specStrength = clamp(pow(max(dot(R, E), 0.0), spec.a), 0.0, 1.0);");
        gfxAddDocumentLine(document, "\t\t\tspecular = spec.rgb*elf_LightColor*specStrength;");
        gfxAddDocumentLine(document, "\t\t}");
        gfxAddDocumentLine(document, "\t\tdiffuse = elf_LightColor*lambertTerm;");
        gfxAddDocumentLine(document, "\t}");
        if (config->light == GFX_POINT_LIGHT || config->light == GFX_SPOT_LIGHT)
        {
            if (config->light == GFX_POINT_LIGHT)
                gfxAddDocumentLine(document, "\tfloat str = att;");
            if (config->light == GFX_SPOT_LIGHT)
                gfxAddDocumentLine(document, "\tfloat str = att*spot;");
            if (config->textures & GFX_SHADOW_MAP)
                gfxAddDocumentLine(document, "\tstr *= shadow;");
        }
        if (config->light != GFX_SUN_LIGHT)
        {
            gfxAddDocumentLine(document, "\tgl_FragData[0] = vec4(diffuse*str, 1.0);");
            gfxAddDocumentLine(document, "\tgl_FragData[1] = vec4(specular*str, 1.0);");
        }
        else
        {
            gfxAddDocumentLine(document, "\tgl_FragData[0] = vec4(diffuse, 1.0);");
            gfxAddDocumentLine(document, "\tgl_FragData[1] = vec4(specular, 1.0);");
        }
        gfxAddDocumentLine(document, "}");
    }

    fragShdr = (char*)malloc(sizeof(char) * (gfxGetDocumentChars(document) + 1));
    gfxDocumentToBuffer(document, fragShdr);
    fragShdr[gfxGetDocumentChars(document)] = '\0';
    // elfLogWrite("----------- FRAGMENT SHADER -----------\n");
    // elfLogWrite(fragShdr);

    // ----------------------------------------------------------- //

    gfxDestroyDocument(document);

    shaderProgram = gfxCreateShaderProgram(vertShdr, fragShdr);

    free(vertShdr);
    free(fragShdr);

    if (shaderProgram)
    {
        memcpy(&shaderProgram->config, config, sizeof(gfxShaderConfig));

        if (!driver->shaderPrograms)
            driver->shaderPrograms = shaderProgram;
        else
        {
            lshpr = driver->shaderPrograms;
            while (lshpr->next) lshpr = lshpr->next;
            lshpr->next = shaderProgram;
        }
    }

    return shaderProgram;
}