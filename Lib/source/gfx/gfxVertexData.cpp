#include "nelf/gfx/gfxVertexData.h"

#include <glad/glad.h>

#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxGeneral.h"
#include "nelf/gfx/gfxObjectType.h"
#include "nelf/gfx/gfxVertexDataType.h"

gfxVertexData* gfxCreateVertexData(int count, int format, int dataType)
{
    gfxVertexData* data;

    if (count <= 0)
        return NULL;
    if (!(format >= GFX_FLOAT && format < GFX_MAX_FORMATS))
        return NULL;
    if (!(dataType >= GFX_VERTEX_DATA_STATIC && dataType < GFX_MAX_VERTEX_DATA_TYPES))
        return NULL;

    data = (gfxVertexData*)malloc(sizeof(gfxVertexData));
    memset(data, 0x0, sizeof(gfxVertexData));
    data->objType = GFX_VERTEX_DATA;
    data->objDestr = gfxDestroyVertexData;

    data->count = count;
    data->format = format;
    data->sizeBytes = driver->formatSizes[format] * count;
    data->dataType = dataType;
    data->data = malloc(data->sizeBytes);
    memset(data->data, 0x0, data->sizeBytes);

    gfxIncObj(GFX_VERTEX_DATA);

    return data;
}

void gfxDestroyVertexData(void* data)
{
    gfxVertexData* vertexData = (gfxVertexData*)data;

    if (vertexData->vbo)
        glDeleteBuffers(1, &vertexData->vbo);

    free(vertexData->data);
    free(vertexData);

    gfxDecObj(GFX_VERTEX_DATA);
}

int gfxGetVertexDataCount(gfxVertexData* data) { return data->count; }

int gfxGetVertexDataFormat(gfxVertexData* data) { return data->format; }

void* gfxGetVertexDataBuffer(gfxVertexData* data) { return data->data; }

int gfxGetVertexDataSizeBytes(gfxVertexData* data) { return data->sizeBytes; }

void gfxUpdateVertexData(gfxVertexData* data)
{
    if (data->vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data->sizeBytes, data->data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    data->changed = false;
}

void gfxUpdateVertexDataSubData(gfxVertexData* data, int start, int length)
{
    if (start > data->sizeBytes)
        return;
    if (start + length > data->sizeBytes)
        length -= (start + length) - data->sizeBytes;

    if (data->vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, start, length, &((char*)data->data)[start]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    data->changed = false;
}

void gfxInitVertexDataVbo(gfxVertexData* data)
{
    if (!data->vbo)
    {
        glGenBuffers(1, &data->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
        glBufferData(GL_ARRAY_BUFFER, data->sizeBytes, data->data, driver->vertexDataDrawModes[data->dataType]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    data->changed = false;
}