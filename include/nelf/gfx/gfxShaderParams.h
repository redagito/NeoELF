#pragma once

#include "nelf/gfx/gfxMaxTextures.h"

struct gfxTexture;
struct gfxGbuffer;
struct gfxShaderProgram;

struct gfxColor
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};

struct gfxPosition
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct gfxNormal
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct gfxRenderParams
{
    bool depthTest = false;
    bool depthWrite = false;
    unsigned char depthFunc = 0;
    bool colorWrite = false;
    bool alphaWrite = false;
    bool alphaTest = false;

    float alphaThreshold = 0.f;

    unsigned char cullFace = 0;
    unsigned char blendMode = 0;

    float offsetScale = 0.f;
    float offsetBias = 0.f;
    float lineWidth = 0.f;

    unsigned char lineSmooth;
    unsigned char cullFaceMode;
    unsigned char frontFace;
    unsigned char wireframe;
    unsigned char vertexColor;
    bool multisample = false;
};

struct gfxFogParams
{
    unsigned char mode = 0;
    float start = 0.f;
    float end = 0.f;
    gfxColor color;
};

struct gfxMaterialParams
{
    gfxColor ambientColor;
    gfxColor diffuseColor;
    gfxColor specularColor;
    float shininess = 0.f;
};

struct gfxTextureParams
{
    // TextureMapType
    int type = 0;
    gfxTexture* texture = nullptr;
    int projectionMode = 0;
    float parallaxScale = 0.f;
    float matrix[16];
};

struct gfxLightParams
{
    unsigned char type = 0;
    unsigned char mode = 0;

    gfxPosition position;
    gfxNormal direction;
    gfxColor color;

    float range = 0.f;
    float fadeRange = 0.f;
    float innerCone = 0.f;
    float outerCone = 0.f;
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
    float clipStart = 0.f;
    float clipEnd = 0.f;

    int viewportWidth = 0;
    int viewportHeight = 0;

    float projectionMatrix[16];
    float invProjectionMatrix[16];
    float modelviewMatrix[16];
    float normalMatrix[9];

    gfxGbuffer* gbuffer = nullptr;
    unsigned char gbufferMode = 0;
    gfxShaderProgram* shaderProgram = nullptr;
};

void gfxSetColor(gfxColor* color, float r, float g, float b, float a);

void gfxSetShaderParamsDefault(gfxShaderParams* shaderParams);
void gfxSetMaterialParamsDefault(gfxShaderParams* shaderParams);
void gfxSetTextureParamsDefault(gfxShaderParams* shaderParams);

void gfxSetShaderParams(gfxShaderParams* shaderParams);