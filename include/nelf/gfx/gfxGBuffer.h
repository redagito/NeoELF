#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxTexture;
struct gfxRenderTarget;
struct gfxShaderParams;

struct gfxGbuffer
{
    GFX_OBJECT_HEADER;

    int width = 0;
    int height = 0;

    gfxRenderTarget* bufRt = nullptr;
    gfxRenderTarget* lightRt = nullptr;
    gfxRenderTarget* mainRt = nullptr;

    gfxTexture* depthTex = nullptr;
    gfxTexture* buf1Tex = nullptr;
    gfxTexture* buf2Tex = nullptr;
    gfxTexture* buf3Tex = nullptr;
    gfxTexture* buf4Tex = nullptr;
    gfxTexture* diffuseTex = nullptr;
    gfxTexture* specularTex = nullptr;

    gfxTexture* mainTex = nullptr;
};

gfxGbuffer* gfxCreateGbuffer(int width, int height);

void gfxDestroyGbuffer(void* data);

void gfxBindGbuffer(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);
void gfxBindGbufferLight(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);
void gfxBindGbufferMain(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);

gfxTexture* gfxGetGbufferDepth(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferDiffuse(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferSpecular(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferMain(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf1(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf2(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf3(gfxGbuffer* gbuffer);
