#include "nelf/gfx/gfxVertexArray.h"

#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "gfx/gfxGeneral.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxObjectType.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxVertexData.h"

gfxVertexArray* gfxCreateVertexArray(bool gpuData)
{
    gfxVertexArray* vertexArray = nullptr;

    vertexArray = (gfxVertexArray*)malloc(sizeof(gfxVertexArray));
    memset(vertexArray, 0x0, sizeof(gfxVertexArray));
    vertexArray->objType = GFX_VERTEX_ARRAY;
    vertexArray->objDestr = gfxDestroyVertexArray;

    vertexArray->gpuData = gpuData;

    gfxIncObj(GFX_VERTEX_ARRAY);

    return vertexArray;
}

void gfxDestroyVertexArray(void* data)
{
    gfxVertexArray* vertexArray = (gfxVertexArray*)data;

    for (int i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
    {
        if (vertexArray->varrs[i].data)
        {
            gfxDecRef((gfxObject*)vertexArray->varrs[i].data);
        }
    }

    gfxDecObj(GFX_VERTEX_ARRAY);

    free(vertexArray);
}

int gfxGetVertexArrayVertexCount(gfxVertexArray* vertexArray) { return vertexArray->vertexCount; }

void gfxSetVertexArrayData(gfxVertexArray* vertexArray, int target, gfxVertexData* data)
{
    if (!(target >= GFX_VERTEX && target < GFX_MAX_VERTEX_ARRAYS))
    {
        printf("error: invalid target when setting vertex array data\n");
        return;
    }

    gfxVarr* varr = &vertexArray->varrs[target];

    switch (target)
    {
    case GFX_VERTEX:
        varr->elementCount = 3;
        break;
    case GFX_NORMAL:
        varr->elementCount = 3;
        break;
    case GFX_TEX_COORD:
        varr->elementCount = 2;
        break;
    case GFX_COLOR:
        varr->elementCount = 4;
        break;
    case GFX_TANGENT:
        varr->elementCount = 3;
        break;
    case GFX_WEIGHTS:
        varr->elementCount = 4;
        break;
    case GFX_BONEIDS:
        varr->elementCount = 4;
        break;
    default:
        printf("error: invalid target for vertex array data\n");
        return;
    }

    if (data)
    {
        varr->vertexCount = data->count / varr->elementCount;
        varr->vertexSizeBytes = driver->formatSizes[data->format] * varr->elementCount;

        if (vertexArray->vertexCount == 0)
            vertexArray->vertexCount = varr->vertexCount;
        else if (varr->vertexCount < vertexArray->vertexCount)
            vertexArray->vertexCount = varr->vertexCount;
        else
        {
            int varrCount = 0;
            for (int i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
                if (vertexArray->varrs[i].data)
                    varrCount++;
            if (varrCount < 2)
                vertexArray->vertexCount = varr->vertexCount;
        }

        if (varr->data)
            gfxDecRef((gfxObject*)varr->data);
        varr->data = data;
        gfxIncRef((gfxObject*)varr->data);

        if (vertexArray->gpuData && driver->version >= 200)
            gfxInitVertexDataVbo(varr->data);
    }
    else
    {
        varr->vertexCount = 0;
        varr->vertexSizeBytes = 0;
        if (varr->data)
            gfxDecRef((gfxObject*)varr->data);
        varr->data = nullptr;
    }
}

void gfxSetVertexArray(gfxVertexArray* vertexArray)
{
    int i;

    if (vertexArray->gpuData)
    {
        for (i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
        {
            if (vertexArray->varrs[i].data)
            {
                glEnableVertexAttribArray(i);
                glBindBuffer(GL_ARRAY_BUFFER, vertexArray->varrs[i].data->vbo);
                glVertexAttribPointer(i, vertexArray->varrs[i].elementCount,
                                      driver->formats[vertexArray->varrs[i].data->format], GL_FALSE, 0, 0);
            }
            else
            {
                glDisableVertexAttribArray(i);
            }
        }

        driver->dirtyVertexArrays = true;
    }
    else
    {
        for (i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
        {
            if (vertexArray->varrs[i].data)
            {
                glEnableVertexAttribArray(i);
                if (driver->dirtyVertexArrays)
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                glVertexAttribPointer(i, vertexArray->varrs[i].elementCount,
                                      driver->formats[vertexArray->varrs[i].data->format], GL_FALSE, 0,
                                      vertexArray->varrs[i].data->data);
            }
            else
            {
                glDisableVertexAttribArray(i);
            }
        }

        driver->dirtyVertexArrays = false;
    }
}

void gfxDrawVertexArray(gfxVertexArray* vertexArray, int count, int drawMode)
{
    if (count > vertexArray->vertexCount)
        count -= count - vertexArray->vertexCount;

    gfxSetVertexArray(vertexArray);

    glDrawArrays(driver->drawModes[drawMode], 0, count);

    driver->verticesDrawn[drawMode] += count;
}
