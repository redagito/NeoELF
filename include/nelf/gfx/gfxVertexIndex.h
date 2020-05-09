#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxVertexData;

struct gfxVertexIndex
{
    GFX_OBJECT_HEADER;
    int indiceCount = 0;
    gfxVertexData* data = nullptr;
    unsigned char gpuData;
};

gfxVertexIndex* gfxCreateVertexIndex(bool gpuData, gfxVertexData* data);
void gfxDestroyVertexIndex(void* data);

int gfxGetVertexIndexIndiceCount(gfxVertexIndex* vertexIndex);

void gfxDrawVertexIndex(gfxVertexIndex* vertexIndex, unsigned int drawMode);