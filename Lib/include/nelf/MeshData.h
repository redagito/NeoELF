#pragma once

#include "nelf/Face.h"
#include "nelf/List.h"
#include "nelf/Vertex.h"
#include "nelf/objectHeader.h"

/**
 * Represents a single mesh
 */
struct elfMeshData
{
    ELF_OBJECT_HEADER;
    // List of vertex data
    // A single vertex has position, normals and texture coordinates
    elfList* vertices;
    // List of faces in the mesh
    // A face consists of indices into the vertices list
    elfList* faces;
};

elfMeshData* elfCreateMeshData();
void elfDestroyMeshData(void* data);

int elfGetMeshDataVertexCount(elfMeshData* meshData);
int elfGetMeshDataFaceCount(elfMeshData* meshData);

void elfAddMeshDataVertex(elfMeshData* meshData, elfVertex* vertice);
void elfAddMeshDataFace(elfMeshData* meshData, int v1, int v2, int v3);

// Fetch by index
elfVertex* elfGetVertexFromMeshData(elfMeshData* meshData, int idx);
elfFace* elfGetFaceFromMeshData(elfMeshData* meshData, int idx);