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
    // TODO What is this meant to do? Its weird
    bool gpuData = false;
};

gfxVertexIndex* gfxCreateVertexIndex(bool gpuData, gfxVertexData* data);
void gfxDestroyVertexIndex(void* data);

int gfxGetVertexIndexIndiceCount(gfxVertexIndex* vertexIndex);

void gfxDrawVertexIndex(gfxVertexIndex* vertexIndex, unsigned int drawMode);