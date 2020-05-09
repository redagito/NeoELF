#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxTexture;

struct gfxRenderTarget
{
    GFX_OBJECT_HEADER;
    unsigned int fb;
    unsigned int rb;
    unsigned int width, height;
    unsigned char targets[16];
};

unsigned char gfxCheckRenderTarget();

gfxRenderTarget* gfxCreateRenderTarget(unsigned int width, unsigned int height);
void gfxDestroyRenderTarget(void* data);

gfxRenderTarget* gfxGetCurRenderTarget();

void gfxSetRenderTargetColorTexture(gfxRenderTarget* renderTarget, unsigned int n, gfxTexture* color);
void gfxSetRenderTargetDepthTexture(gfxRenderTarget* renderTarget, gfxTexture* depth);

unsigned char gfxSetRenderTarget(gfxRenderTarget* renderTarget);

void gfxDisableRenderTarget();