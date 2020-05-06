#pragma once

#include "nelf/Face.h"
#include "nelf/List.h"
#include "nelf/Vertex.h"
#include "nelf/objectHeader.h"

// TODO Maybe just call it a mesh?
struct elfMeshData
{
    ELF_OBJECT_HEADER;
    elfList* vertices;
    elfList* faces;
};

elfMeshData* elfCreateMeshData();
void elfDestroyMeshData(void* data);

int elfGetMeshDataVertexCount(elfMeshData* meshData);
int elfGetMeshDataFaceCount(elfMeshData* meshData);

void elfAddMeshDataVertex(elfMeshData* meshData, elfVertex* vertice);
void elfAddMeshDataFace(elfMeshData* meshData, int v1, int v2, int v3);

elfVertex* elfGetVertexFromMeshData(elfMeshData* meshData, int idx);
elfFace* elfGetFaceFromMeshData(elfMeshData* meshData, int idx);