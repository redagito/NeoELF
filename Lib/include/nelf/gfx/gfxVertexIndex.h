#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxVertexData;

// Represents a vertex data array and index array
// TODO This requires a VAO to draw
struct gfxVertexIndex
{
    GFX_OBJECT_HEADER;
    int indiceCount = 0;
    gfxVertexData* data = nullptr;
    // Whether the index data is in a VBO and already uploaded to the GPU
    // or the data is provided with the draw call
    // TODO Performance impact if false, should always use VBO
    bool gpuData = false;
};

gfxVertexIndex* gfxCreateVertexIndex(bool gpuData, gfxVertexData* data);
void gfxDestroyVertexIndex(void* data);

int gfxGetVertexIndexIndiceCount(gfxVertexIndex* vertexIndex);

void gfxDrawVertexIndex(gfxVertexIndex* vertexIndex, unsigned int drawMode);