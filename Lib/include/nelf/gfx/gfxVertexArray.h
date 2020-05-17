#pragma once

#include "nelf/gfx/gfxAttributeType.h"
#include "nelf/gfx/gfxObjectHeader.h"

struct gfxVertexData;

// A single vertex data array
// TODO Not sure why this is required, the data could just be in the gfxvertexData struct?
struct gfxVarr
{
    // Buffer data
    gfxVertexData* data = nullptr;
    // Number of vertices?
    int vertexCount = 0;
    // Number of elements? Maybe for buffers that do not store vertices?
    int elementCount = 0;
    // ???
    int vertexSizeBytes = 0;
};

// Wraps multiple data buffers
// TODO This should manage a VAO
// TODO Why is there no index buffer, why is there a separate gfxVertexIndex type?
struct gfxVertexArray
{
    GFX_OBJECT_HEADER;
    // ???
    int vertexCount = 0;
    // All available types of vertex data arrays
    gfxVarr varrs[GFX_MAX_VERTEX_ARRAYS];
    // ???
    bool gpuData = false;
};

gfxVertexArray* gfxCreateVertexArray(bool gpuData);
void gfxDestroyVertexArray(void* data);

int gfxGetVertexArrayVertexCount(gfxVertexArray* vertexArray);

void gfxSetVertexArrayData(gfxVertexArray* vertexArray, int target, gfxVertexData* data);
void gfxSetVertexArray(gfxVertexArray* vertexArray);

void gfxDrawVertexArray(gfxVertexArray* vertexArray, int count, int drawMode);
