#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxTexture;

struct gfxRenderTarget
{
    GFX_OBJECT_HEADER;
    // Framebuffer
    unsigned int fb = 0;
    // Renderbuffer
    unsigned int rb = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned char targets[16];
};

unsigned char gfxCheckRenderTarget();

gfxRenderTarget* gfxCreateRenderTarget(unsigned int width, unsigned int height);
void gfxDestroyRenderTarget(void* data);

gfxRenderTarget* gfxGetCurRenderTarget();

void gfxSetRenderTargetColorTexture(gfxRenderTarget* renderTarget, unsigned int n, gfxTexture* color);
void gfxSetRenderTargetDepthTexture(gfxRenderTarget* renderTarget, gfxTexture* depth);

bool gfxSetRenderTarget(gfxRenderTarget* renderTarget);

void gfxDisableRenderTarget();