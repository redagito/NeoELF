#pragma once

#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxObjectHeader.h"
#include "nelf/gfx/gfxVertexDataType.h"

// Represents a buffer with data for rendering
// Wraps a vertex buffer object id, VBO
struct gfxVertexData
{
    GFX_OBJECT_HEADER;
    // Vertex buffer object id
    unsigned int vbo = 0;
    // Number of elements?
    int count = 0;
    // Data type of the buffer
    GfxFormatType format = 0;
    // Total size of the data buffer in bytes
    int sizeBytes = 0;
    // Data úsage hint, static or dynamic
    GfxVertexDataType dataType = 0;
    // Cached data
    void* data = nullptr;
    // Data changed, requires reupload to gpu
    // TODO This is actually never set and never checked
    bool changed = false;
};

gfxVertexData* gfxCreateVertexData(int count, int format, int dataType);
void gfxDestroyVertexData(void* data);

int gfxGetVertexDataCount(gfxVertexData* data);
int gfxGetVertexDataFormat(gfxVertexData* data);
void* gfxGetVertexDataBuffer(gfxVertexData* data);
int gfxGetVertexDataSizeBytes(gfxVertexData* data);

// Write data tp gpu memory
void gfxUpdateVertexData(gfxVertexData* data);
// Write subrange of data tp gpu memory
void gfxUpdateVertexDataSubData(gfxVertexData* data, int start, int length);

// Inits vbo, transfers data to gpu
void gfxInitVertexDataVbo(gfxVertexData* data);