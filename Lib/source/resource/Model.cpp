#include "nelf/resource/Model.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Face.h"
#include "nelf/General.h"
#include "nelf/MeshData.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/Vertex.h"
#include "nelf/drawMode.h"
#include "nelf/gfx/gfxDrawMode.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxTextureMapType.h"
#include "nelf/gfx/gfxVertexArray.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/gfx/gfxVertexDataType.h"
#include "nelf/gfx/gfxVertexIndex.h"
#include "nelf/objectType.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Resources.h"

elfModel* elfCreateModel(const char* name)
{
    elfModel* model;

    model = (elfModel*)malloc(sizeof(elfModel));
    memset(model, 0x0, sizeof(elfModel));
    model->objType = ELF_MODEL;
    model->objDestr = elfDestroyModel;

    model->id = ++res->modelIdCounter;

    if (name)
        model->name = elfCreateString(name);

    elfIncObj(ELF_MODEL);

    return model;
}

elfModel* elfCreateModelFromMeshData(elfMeshData* data)
{
    elfModel* model;
    float* vertexBuffer;
    float* normalBuffer;
    float* texcoordBuffer;
    unsigned int* indexBuffer;
    int i, j;
    elfFace* face;
    elfVertex* vertex;

    if (elfGetMeshDataVertexCount(data) < 3)
        return NULL;
    if (elfGetMeshDataFaceCount(data) < 1)
        return NULL;

    model = elfCreateModel(NULL);

    model->verticeCount = elfGetListLength(data->vertices);
    model->frameCount = 1;
    model->areaCount = 1;
    model->indiceCount = elfGetListLength(data->faces) * 3;

    model->vertices = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
    model->normals = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
    model->texCoords = gfxCreateVertexData(2 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);

    gfxIncRef((gfxObject*)model->vertices);
    gfxIncRef((gfxObject*)model->normals);
    gfxIncRef((gfxObject*)model->texCoords);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(model->vertices);
    normalBuffer = (float*)gfxGetVertexDataBuffer(model->normals);
    texcoordBuffer = (float*)gfxGetVertexDataBuffer(model->texCoords);

    model->areas = (elfModelArea*)malloc(sizeof(elfModelArea) * model->areaCount);
    memset(model->areas, 0x0, sizeof(elfModelArea) * model->areaCount);

    model->areas[0].indiceCount = model->indiceCount;
    model->areas[0].index = gfxCreateVertexData(model->areas[0].indiceCount, GFX_UINT, GFX_VERTEX_DATA_STATIC);
    gfxIncRef((gfxObject*)model->areas[0].index);

    // model->index should contain ALL of the indexes of the model regardless of the material
    // its used for generating the physics triangle mesh of the model
    model->index = (unsigned int*)malloc(sizeof(unsigned int) * model->areas[0].indiceCount);

    indexBuffer = (unsigned int*)gfxGetVertexDataBuffer(model->areas[0].index);

    for (i = 0, face = (elfFace*)elfBeginList(data->faces); face; face = (elfFace*)elfGetListNext(data->faces), i += 3)
    {
        indexBuffer[i] = face->v1;
        indexBuffer[i + 1] = face->v2;
        indexBuffer[i + 2] = face->v3;
    }

    for (i = 0, j = 0, vertex = (elfVertex*)elfBeginList(data->vertices); vertex;
         vertex = (elfVertex*)elfGetListNext(data->vertices), i += 3, j += 2)
    {
        vertexBuffer[i] = vertex->position.x;
        vertexBuffer[i + 1] = vertex->position.y;
        vertexBuffer[i + 2] = vertex->position.z;

        normalBuffer[i] = vertex->normal.x;
        normalBuffer[i + 1] = vertex->normal.y;
        normalBuffer[i + 2] = vertex->normal.z;

        texcoordBuffer[j] = vertex->texCoord.x;
        texcoordBuffer[j + 1] = vertex->texCoord.y;
    }

    // get bounding box values
    memcpy(&model->bbMin.x, vertexBuffer, sizeof(float) * 3);
    memcpy(&model->bbMax.x, vertexBuffer, sizeof(float) * 3);

    for (j = 3; j < model->verticeCount * 3; j += 3)
    {
        if (vertexBuffer[j] < model->bbMin.x)
            model->bbMin.x = vertexBuffer[j];
        if (vertexBuffer[j + 1] < model->bbMin.y)
            model->bbMin.y = vertexBuffer[j + 1];
        if (vertexBuffer[j + 2] < model->bbMin.z)
            model->bbMin.z = vertexBuffer[j + 2];

        if (vertexBuffer[j] > model->bbMax.x)
            model->bbMax.x = vertexBuffer[j];
        if (vertexBuffer[j + 1] > model->bbMax.y)
            model->bbMax.y = vertexBuffer[j + 1];
        if (vertexBuffer[j + 2] > model->bbMax.z)
            model->bbMax.z = vertexBuffer[j + 2];
    }

    model->vertexArray = gfxCreateVertexArray(true);
    gfxIncRef((gfxObject*)model->vertexArray);

    gfxSetVertexArrayData(model->vertexArray, GFX_VERTEX, model->vertices);
    gfxSetVertexArrayData(model->vertexArray, GFX_NORMAL, model->normals);
    gfxSetVertexArrayData(model->vertexArray, GFX_TEX_COORD, model->texCoords);

    model->areas[0].vertexIndex = gfxCreateVertexIndex(true, model->areas[0].index);
    gfxIncRef((gfxObject*)model->areas[0].vertexIndex);

    memcpy(model->index, indexBuffer, sizeof(unsigned int) * model->areas[0].indiceCount);

    return model;
}

void elfGenerateModelTangents(elfModel* model)
{
    float* vertexBuffer;
    float* texCoordBuffer;
    float* normalBuffer;
    float* tangentBuffer;
    float* vertices;
    float* texCoords;
    float* tangents;
    float edge1[3];
    float edge2[3];
    float edge1uv[2];
    float edge2uv[2];
    float cp;
    float mul;
    float dot;
    int i, j;

    if (!model->vertices || !model->texCoords || !model->index)
        return;

    if (model->tangents)
        gfxDecRef((gfxObject*)model->tangents);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(model->vertices);
    texCoordBuffer = (float*)gfxGetVertexDataBuffer(model->texCoords);

    vertices = (float*)malloc(sizeof(float) * model->indiceCount * 3);
    texCoords = (float*)malloc(sizeof(float) * model->indiceCount * 2);
    tangents = (float*)malloc(sizeof(float) * model->indiceCount * 3);

    // create corresponding vertice and tex coord arrays independent of index
    for (i = 0; i < (int)model->indiceCount / 3; i++)
    {
        vertices[i * 9] = vertexBuffer[model->index[i * 3] * 3];
        vertices[i * 9 + 1] = vertexBuffer[model->index[i * 3] * 3 + 1];
        vertices[i * 9 + 2] = vertexBuffer[model->index[i * 3] * 3 + 2];
        vertices[i * 9 + 3] = vertexBuffer[model->index[i * 3 + 1] * 3];
        vertices[i * 9 + 4] = vertexBuffer[model->index[i * 3 + 1] * 3 + 1];
        vertices[i * 9 + 5] = vertexBuffer[model->index[i * 3 + 1] * 3 + 2];
        vertices[i * 9 + 6] = vertexBuffer[model->index[i * 3 + 2] * 3];
        vertices[i * 9 + 7] = vertexBuffer[model->index[i * 3 + 2] * 3 + 1];
        vertices[i * 9 + 8] = vertexBuffer[model->index[i * 3 + 2] * 3 + 2];

        texCoords[i * 6] = texCoordBuffer[model->index[i * 3] * 2];
        texCoords[i * 6 + 1] = texCoordBuffer[model->index[i * 3] * 2 + 1];
        texCoords[i * 6 + 2] = texCoordBuffer[model->index[i * 3 + 1] * 2];
        texCoords[i * 6 + 3] = texCoordBuffer[model->index[i * 3 + 1] * 2 + 1];
        texCoords[i * 6 + 4] = texCoordBuffer[model->index[i * 3 + 2] * 2];
        texCoords[i * 6 + 5] = texCoordBuffer[model->index[i * 3 + 2] * 2 + 1];
    }

    memset(tangents, 0x0, sizeof(float) * model->indiceCount * 3);

    // calculate tangents
    for (i = 0, j = 0; i < (int)model->indiceCount * 3; i += 9, j += 6)
    {
        edge1[0] = vertices[i + 3] - vertices[i];
        edge1[1] = vertices[i + 4] - vertices[i + 1];
        edge1[2] = vertices[i + 5] - vertices[i + 2];
        edge2[0] = vertices[i + 6] - vertices[i];
        edge2[1] = vertices[i + 7] - vertices[i + 1];
        edge2[2] = vertices[i + 8] - vertices[i + 2];

        edge1uv[0] = texCoords[j + 2] - texCoords[j];
        edge1uv[1] = texCoords[j + 3] - texCoords[j + 1];
        edge2uv[0] = texCoords[j + 4] - texCoords[j];
        edge2uv[1] = texCoords[j + 5] - texCoords[j + 1];

        cp = edge1uv[1] * edge2uv[0] - edge1uv[0] * edge2uv[1];

        if (cp != 0.0f)
        {
            mul = 1.0f / cp;
            tangents[i] = (edge1[0] * (-edge2uv[1]) + edge2[0] * edge1uv[1]) * mul;
            tangents[i + 1] = (edge1[1] * (-edge2uv[1]) + edge2[1] * edge1uv[1]) * mul;
            tangents[i + 2] = (edge1[2] * (-edge2uv[1]) + edge2[2] * edge1uv[1]) * mul;

            gfxVecNormalize(&tangents[i]);

            memcpy(&tangents[i + 3], &tangents[i], sizeof(float) * 3);
            memcpy(&tangents[i + 6], &tangents[i], sizeof(float) * 3);
        }
    }

    model->tangents = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
    gfxIncRef((gfxObject*)model->tangents);

    tangentBuffer = (float*)gfxGetVertexDataBuffer(model->tangents);

    // smooth tangents
    for (i = 0; i < (int)model->indiceCount / 3; i++)
    {
        tangentBuffer[model->index[i * 3] * 3] += tangents[i * 9];
        tangentBuffer[model->index[i * 3] * 3 + 1] += tangents[i * 9 + 1];
        tangentBuffer[model->index[i * 3] * 3 + 2] += tangents[i * 9 + 2];
        tangentBuffer[model->index[i * 3 + 1] * 3] += tangents[i * 9 + 3];
        tangentBuffer[model->index[i * 3 + 1] * 3 + 1] += tangents[i * 9 + 4];
        tangentBuffer[model->index[i * 3 + 1] * 3 + 2] += tangents[i * 9 + 5];
        tangentBuffer[model->index[i * 3 + 2] * 3] += tangents[i * 9 + 6];
        tangentBuffer[model->index[i * 3 + 2] * 3 + 1] += tangents[i * 9 + 7];
        tangentBuffer[model->index[i * 3 + 2] * 3 + 2] += tangents[i * 9 + 8];
    }

    for (i = 0; i < (int)model->verticeCount * 3; i += 3)
    {
        gfxVecNormalize(&tangentBuffer[i]);
    }

    normalBuffer = (float*)gfxGetVertexDataBuffer(model->normals);

    // orthogonize tangents
    for (i = 0; i < (int)model->verticeCount * 3; i += 3)
    {
        gfxVecDotVec(&tangentBuffer[i], &normalBuffer[i], &dot);
        tangentBuffer[i] -= normalBuffer[i] * dot;
        tangentBuffer[i + 1] -= normalBuffer[i + 1] * dot;
        tangentBuffer[i + 2] -= normalBuffer[i + 2] * dot;
        gfxVecNormalize(&tangentBuffer[i]);
    }

    gfxSetVertexArrayData(model->vertexArray, GFX_TANGENT, model->tangents);

    free(vertices);
    free(texCoords);
    free(tangents);
}

void elfDestroyModel(void* data)
{
    int i;
    elfModel* model = (elfModel*)data;

    if (model->name)
        elfDestroyString(model->name);
    if (model->filePath)
        elfDestroyString(model->filePath);

    if (model->vertexArray)
        gfxDecRef((gfxObject*)model->vertexArray);
    if (model->vertices)
        gfxDecRef((gfxObject*)model->vertices);
    if (model->normals)
        gfxDecRef((gfxObject*)model->normals);
    if (model->texCoords)
        gfxDecRef((gfxObject*)model->texCoords);
    if (model->tangents)
        gfxDecRef((gfxObject*)model->tangents);

    if (model->areas)
    {
        for (i = 0; i < model->areaCount; i++)
        {
            if (model->areas[i].indiceCount > 0)
            {
                gfxDecRef((gfxObject*)model->areas[i].index);
                gfxDecRef((gfxObject*)model->areas[i].vertexIndex);
            }
        }
        free(model->areas);
    }

    if (model->index)
        free(model->index);
    if (model->weights)
        free(model->weights);
    if (model->boneids)
        free(model->boneids);
    if (model->triMesh)
        elfDecRef((elfObject*)model->triMesh);

    free(model);

    elfDecObj(ELF_MODEL);
}

void elfSetModelName(elfModel* model, const char* name)
{
    if (model->name)
        elfDestroyString(model->name);
    model->name = elfCreateString(name);
}

const char* elfGetModelName(elfModel* model) { return model->name; }

const char* elfGetModelFilePath(elfModel* model) { return model->filePath; }

int elfGetModelVertexCount(elfModel* model) { return model->verticeCount; }

int elfGetModelIndiceCount(elfModel* model) { return model->indiceCount; }

elfVec3f elfGetModelBoundingBoxMin(elfModel* model) { return model->bbMin; }

elfVec3f elfGetModelBoundingBoxMax(elfModel* model) { return model->bbMax; }

float* elfGetModelVertices(elfModel* model) { return (float*)gfxGetVertexDataBuffer(model->vertices); }

float* elfGetModelNormals(elfModel* model)
{
    if (!model->normals)
        return NULL;
    return (float*)gfxGetVertexDataBuffer(model->normals);
}

float* elfGetModelTexCoords(elfModel* model)
{
    if (!model->texCoords)
        return NULL;
    return (float*)model->texCoords;
}

float* elfGetModelTangents(elfModel* model)
{
    if (!model->tangents)
        return NULL;
    return (float*)gfxGetVertexDataBuffer(model->tangents);
}

unsigned int* elfGetModelIndices(elfModel* model) { return model->index; }

void elfDrawModel(elfList* materials, elfModel* model, int mode, gfxShaderParams* shaderParams)
{
    int i, j;
    elfMaterial* material = nullptr;
    bool found = false;

    if (!model->vertexArray)
        return;

    if (mode == ELF_DRAW_WITHOUT_LIGHTING)
    {
        found = false;

        for (material = (elfMaterial*)elfBeginList(materials); material;
             material = (elfMaterial*)elfGetListNext(materials))
        {
            if (!material->lighting)
            {
                found = true;
                break;
            }
        }

        if (!found)
            return;
    }

    gfxSetVertexArray(model->vertexArray);

    for (i = 0, material = (elfMaterial*)elfBeginList(materials); i < (int)model->areaCount;
         i++, material = (elfMaterial*)elfGetListNext(materials))
    {
        if (model->areas[i].vertexIndex)
        {
            if (material)
            {
                if (mode == ELF_DRAW_WITH_LIGHTING)
                {
                    if (!material->lighting)
                        continue;
                }
                else if (mode == ELF_DRAW_WITHOUT_LIGHTING)
                {
                    if (material->lighting)
                        continue;
                }

                elfSetMaterial(material, mode, shaderParams);

                gfxSetShaderParams(shaderParams);
            }
            else
            {
                gfxSetMaterialParamsDefault(shaderParams);
                for (j = 0; j < GFX_MAX_TEXTURES - 1; j++)
                {
                    if (shaderParams->textureParams[j].type != GFX_SHADOW_MAP)
                        shaderParams->textureParams[j].texture = 0;
                }

                gfxSetShaderParams(shaderParams);
            }

            gfxDrawVertexIndex(model->areas[i].vertexIndex, GFX_TRIANGLES);
        }
    }
}

void elfDrawModelBoundingBox(elfModel* model, gfxShaderParams* shaderParams)
{
    if (!model->vertexArray)
        return;

    gfxSetShaderParams(shaderParams);
    elfDrawBoundingBox(&model->bbMin.x, &model->bbMax.x);
}
