#include "nelf/gfx/gfxVertexIndex.h"

#include <glad/glad.h>

#include <cstdlib>
#include <cstring>

#include "gfx/gfxGeneral.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxObjectType.h"
#include "nelf/gfx/gfxVertexData.h"

gfxVertexIndex* gfxCreateVertexIndex(bool gpuData, gfxVertexData* data)
{
    gfxVertexIndex* vertexIndex = NULL;

    vertexIndex = (gfxVertexIndex*)malloc(sizeof(gfxVertexIndex));
    memset(vertexIndex, 0x0, sizeof(gfxVertexIndex));
    vertexIndex->objType = GFX_VERTEX_INDEX;
    vertexIndex->objDestr = gfxDestroyVertexIndex;

    vertexIndex->data = data;
    gfxIncRef((gfxObject*)vertexIndex->data);
    vertexIndex->indiceCount = gfxGetVertexDataCount(data);

    vertexIndex->gpuData = gpuData;

    if (vertexIndex->gpuData && driver->version >= 200)
        gfxInitVertexDataVbo(data);

    gfxIncObj(GFX_VERTEX_INDEX);

    return vertexIndex;
}

void gfxDestroyVertexIndex(void* data)
{
    gfxVertexIndex* vertexIndex = (gfxVertexIndex*)data;

    if (vertexIndex->data)
        gfxDecRef((gfxObject*)vertexIndex->data);

    free(vertexIndex);

    gfxDecObj(GFX_VERTEX_INDEX);
}

int gfxGetVertexIndexIndiceCount(gfxVertexIndex* vertexIndex) { return vertexIndex->indiceCount; }

void gfxDrawVertexIndex(gfxVertexIndex* vertexIndex, unsigned int drawMode)
{
    // Should use VAO for drawing with full state setup
    if (vertexIndex->gpuData)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndex->data->vbo);
        glDrawElements(driver->drawModes[drawMode], vertexIndex->indiceCount,
                       driver->formats[vertexIndex->data->format], 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        // For drawing without an VBO, uploads data directly
        glDrawElements(driver->drawModes[drawMode], vertexIndex->indiceCount,
                       driver->formats[vertexIndex->data->format], vertexIndex->data->data);
    }

    driver->verticesDrawn[drawMode] += vertexIndex->indiceCount;
}
