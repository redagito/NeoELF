#pragma once

#include "nelf/gfx/gfxAttributeType.h"
#include "nelf/gfx/gfxObjectHeader.h"

struct gfxVertexData;

struct gfxVarr
{
    gfxVertexData* data = nullptr;
    int vertexCount = 0;
    int elementCount = 0;
    int vertexSizeBytes = 0;
};

struct gfxVertexArray
{
    GFX_OBJECT_HEADER;
    int vertexCount = 0;
    gfxVarr varrs[GFX_MAX_VERTEX_ARRAYS];
    bool gpuData = false;
};

gfxVertexArray* gfxCreateVertexArray(unsigned char gpuData);
void gfxDestroyVertexArray(void* data);

int gfxGetVertexArrayVertexCount(gfxVertexArray* vertexArray);

void gfxSetVertexArrayData(gfxVertexArray* vertexArray, int target, gfxVertexData* data);
void gfxSetVertexArray(gfxVertexArray* vertexArray);

void gfxDrawVertexArray(gfxVertexArray* vertexArray, int count, int drawMode);
