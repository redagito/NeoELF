
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
    data->changed = GFX_FALSE;
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
    data->changed = GFX_FALSE;
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
    data->changed = GFX_FALSE;
}

gfxVertexArray* gfxCreateVertexArray(unsigned char gpuData)
{
    gfxVertexArray* vertexArray = NULL;

    vertexArray = (gfxVertexArray*)malloc(sizeof(gfxVertexArray));
    memset(vertexArray, 0x0, sizeof(gfxVertexArray));
    vertexArray->objType = GFX_VERTEX_ARRAY;
    vertexArray->objDestr = gfxDestroyVertexArray;

    vertexArray->gpuData = !gpuData == GFX_FALSE;

    gfxIncObj(GFX_VERTEX_ARRAY);

    return vertexArray;
}

void gfxDestroyVertexArray(void* data)
{
    int i;
    gfxVertexArray* vertexArray = (gfxVertexArray*)data;

    for (i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
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
    gfxVarr* varr;
    int i;
    int varrCount;

    if (!(target >= GFX_VERTEX && target < GFX_MAX_VERTEX_ARRAYS))
    {
        printf("error: invalid target when setting vertex array data\n");
        return;
    }

    varr = &vertexArray->varrs[target];

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
            varrCount = 0;
            for (i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
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
        varr->data = NULL;
    }
}

void gfxSetVertexArray(gfxVertexArray* vertexArray)
{
    int i;

    if (driver->version < 200)
    {
        if (vertexArray->varrs[GFX_VERTEX].data)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, driver->formats[vertexArray->varrs[GFX_VERTEX].data->format], 0,
                            vertexArray->varrs[GFX_VERTEX].data->data);
        }
        else
        {
            glDisableClientState(GL_VERTEX_ARRAY);
        }

        if (vertexArray->varrs[GFX_NORMAL].data)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(driver->formats[vertexArray->varrs[GFX_NORMAL].data->format], 0,
                            vertexArray->varrs[GFX_NORMAL].data->data);
        }
        else
        {
            glDisableClientState(GL_NORMAL_ARRAY);
        }

        if (vertexArray->varrs[GFX_COLOR].data)
        {
            // input something that doesn't make sense so that the driver will know something has changed in the color
            gfxSetColor(&driver->shaderParams.materialParams.diffuseColor, 10.3f, 10.056f, 10.230f, 1.0f);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, driver->formats[vertexArray->varrs[GFX_COLOR].data->format], 0,
                           vertexArray->varrs[GFX_COLOR].data->data);
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
        }

        if (vertexArray->varrs[GFX_TEX_COORD].data)
        {
            for (i = 0; i < GFX_MAX_TEXTURES - 1; i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glClientActiveTexture(GL_TEXTURE0 + i);

                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2, driver->formats[vertexArray->varrs[GFX_TEX_COORD].data->format], 0,
                                  vertexArray->varrs[GFX_TEX_COORD].data->data);
            }
        }
        else
        {
            for (i = 0; i < GFX_MAX_TEXTURES - 1; i++)
            {
                glActiveTexture(GL_TEXTURE0_ARB + i);
                glClientActiveTexture(GL_TEXTURE0_ARB + i);

                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
        }
    }
    else
    {
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

            driver->dirtyVertexArrays = GFX_TRUE;
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

            driver->dirtyVertexArrays = GFX_FALSE;
        }
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

gfxVertexIndex* gfxCreateVertexIndex(unsigned char gpuData, gfxVertexData* data)
{
    gfxVertexIndex* vertexIndex = NULL;

    vertexIndex = (gfxVertexIndex*)malloc(sizeof(gfxVertexIndex));
    memset(vertexIndex, 0x0, sizeof(gfxVertexIndex));
    vertexIndex->objType = GFX_VERTEX_INDEX;
    vertexIndex->objDestr = gfxDestroyVertexIndex;

    vertexIndex->data = data;
    gfxIncRef((gfxObject*)vertexIndex->data);
    vertexIndex->indiceCount = gfxGetVertexDataCount(data);

    vertexIndex->gpuData = !gpuData == GFX_FALSE;

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
    if (vertexIndex->gpuData && driver->version >= 200)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndex->data->vbo);
        glDrawElements(driver->drawModes[drawMode], vertexIndex->indiceCount,
                       driver->formats[vertexIndex->data->format], 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        glDrawElements(driver->drawModes[drawMode], vertexIndex->indiceCount,
                       driver->formats[vertexIndex->data->format], vertexIndex->data->data);
    }

    driver->verticesDrawn[drawMode] += vertexIndex->indiceCount;
}
