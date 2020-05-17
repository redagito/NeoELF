#pragma once

#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxObjectHeader.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxTextureWrapMode.h"

struct gfxTexture
{
    GFX_OBJECT_HEADER;
    unsigned int id = 0;
    int type = 0;
    int width = 0;
    int height = 0;
    int format = 0;
    int dataFormat = 0;
};

int gfxGetMaxTextureSize();

gfxTexture* gfxCreateTexture();
gfxTexture* gfxCreate2dTexture(unsigned int width, unsigned int height, float anisotropy, GfxTextureWrapMode mode,
                               GfxTextureFilterType filter, GfxTextureFormat format, GfxTextureFormat internalFormat,
                               GfxFormatType dataFormat, void* data);
gfxTexture* gfxCreateCubeMap(unsigned int width, unsigned int height, float anisotropy, GfxTextureWrapMode mode,
                             GfxTextureFilterType filter, GfxTextureFormat format, GfxTextureFormat internalFormat,
                             GfxFormatType dataFormat, void* xpos, void* xneg, void* ypos, void* yneg, void* zpos,
                             void* zneg);

void gfxDestroyTexture(void* data);

int gfxGetTextureType(gfxTexture* texture);
int gfxGetTextureWidth(gfxTexture* texture);
int gfxGetTextureHeight(gfxTexture* texture);
int gfxGetTextureFormat(gfxTexture* texture);
int gfxGetTextureDataFormat(gfxTexture* texture);

void gfxSetTexture(gfxTexture* texture, int slot);

void elfDisableTexture(int slot);
