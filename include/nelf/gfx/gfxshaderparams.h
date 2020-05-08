#pragma once

#include "nelf/gfx/gfxMaxTextures.h"

struct gfxTexture;
struct gfxGbuffer;
struct gfxShaderProgram;

struct gfxColor
{
    float r, g, b, a;
};

struct gfxPosition
{
    float x, y, z;
};

struct gfxNormal
{
    float x, y, z;
};

struct gfxRenderParams
{
    bool depthTest;
    bool depthWrite;
    unsigned char depthFunc;
    bool colorWrite;
    bool alphaWrite;
    bool alphaTest;

    float alphaThreshold;

    unsigned char cullFace;
    unsigned char blendMode;

    float offsetScale;
    float offsetBias;
    float lineWidth;

    unsigned char lineSmooth;
    unsigned char cullFaceMode;
    unsigned char frontFace;
    unsigned char wireframe;
    unsigned char vertexColor;
    bool multisample;
};

struct gfxFogParams
{
    unsigned char mode;
    float start;
    float end;
    gfxColor color;
};

struct gfxMaterialParams
{
    gfxColor ambientColor;
    gfxColor diffuseColor;
    gfxColor specularColor;
    float shininess;
};

struct gfxTextureParams
{
    int type;
    gfxTexture* texture = nullptr;
    int projectionMode;
    float parallaxScale;
    float matrix[16];
};

struct gfxLightParams
{
    unsigned char type;
    unsigned char mode;

    gfxPosition position;
    gfxNormal direction;
    gfxColor color;

    float range;
    float fadeRange;
    float innerCone;
    float outerCone;
};

struct gfxShaderParams
{
    gfxRenderParams renderParams;
    gfxFogParams fogParams;
    gfxMaterialParams materialParams;
    gfxTextureParams textureParams[GFX_MAX_TEXTURES];
    gfxLightParams lightParams;
    gfxPosition cameraPosition;

    float cameraMatrix[16];
    float clipStart;
    float clipEnd;

    int viewportWidth;
    int viewportHeight;

    float projectionMatrix[16];
    float invProjectionMatrix[16];
    float modelviewMatrix[16];
    float normalMatrix[9];

    gfxGbuffer* gbuffer;
    unsigned char gbufferMode;
    gfxShaderProgram* shaderProgram;
};

void gfxSetColor(gfxColor* color, float r, float g, float b, float a);

void gfxSetShaderParamsDefault(gfxShaderParams* shaderParams);
void gfxSetMaterialParamsDefault(gfxShaderParams* shaderParams);
void gfxSetTextureParamsDefault(gfxShaderParams* shaderParams);

void gfxSetShaderParams(gfxShaderParams* shaderParams);