#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxVertexData
{
    GFX_OBJECT_HEADER;
    unsigned int vbo = 0;
    int count = 0;
    int format = 0;
    int sizeBytes = 0;
    int dataType = 0;
    void* data = nullptr;
    bool changed = false;
};

gfxVertexData* gfxCreateVertexData(int count, int format, int dataType);
void gfxDestroyVertexData(void* data);

int gfxGetVertexDataCount(gfxVertexData* data);
int gfxGetVertexDataFormat(gfxVertexData* data);
void* gfxGetVertexDataBuffer(gfxVertexData* data);
int gfxGetVertexDataSizeBytes(gfxVertexData* data);

void gfxUpdateVertexData(gfxVertexData* data);
void gfxUpdateVertexDataSubData(gfxVertexData* data, int start, int length);

void gfxInitVertexDataVbo(gfxVertexData* data);