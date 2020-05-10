#pragma once

#include "nelf/Vector.h"
#include "nelf/resource/resourceHeader.h"

struct elfMeshData;
struct elfPhysicsTriMesh;
struct elfList;
struct gfxVertexArray;
struct gfxVertexData;
struct gfxVertexIndex;
struct gfxShaderParams;

struct elfModelArea
{
    int indiceCount = 0;
    gfxVertexData* index = nullptr;
    gfxVertexIndex* vertexIndex = nullptr;
    unsigned int materialNumber = 0;
};

struct elfModel
{
    ELF_RESOURCE_HEADER;
    char* filePath = nullptr;
    int verticeCount = 0;
    int frameCount = 0;
    int areaCount = 0;
    int indiceCount = 0;

    gfxVertexArray* vertexArray = nullptr;
    gfxVertexData* vertices = nullptr;
    gfxVertexData* normals = nullptr;
    gfxVertexData* texCoords = nullptr;
    gfxVertexData* tangents = nullptr;
    unsigned int* index = nullptr;
    float* weights = nullptr;
    int* boneids = nullptr;
    elfPhysicsTriMesh* triMesh = nullptr;
    elfModelArea* areas = nullptr;
    elfVec3f bbMin;
    elfVec3f bbMax;
};

elfModel* elfCreateModel(const char* name);
elfModel* elfCreateModelFromMeshData(elfMeshData* data);

void elfGenerateModelTangents(elfModel* model);

void elfDestroyModel(void* data);

void elfSetModelName(elfModel* model, const char* name);

const char* elfGetModelName(elfModel* model);
const char* elfGetModelFilePath(elfModel* model);

int elfGetModelVertexCount(elfModel* model);
int elfGetModelIndiceCount(elfModel* model);

elfVec3f elfGetModelBoundingBoxMin(elfModel* model);
elfVec3f elfGetModelBoundingBoxMax(elfModel* model);

float* elfGetModelVertices(elfModel* model);
float* elfGetModelNormals(elfModel* model);
float* elfGetModelTexCoords(elfModel* model);
float* elfGetModelTangents(elfModel* model);
unsigned int* elfGetModelIndices(elfModel* model);

void elfDrawModel(elfList* materials, elfModel* model, int mode, gfxShaderParams* shaderParams);
void elfDrawModelBoundingBox(elfModel* model, gfxShaderParams* shaderParams);