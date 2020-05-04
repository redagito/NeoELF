#include "nelf/MeshData.h"

elfMeshData* elfCreateMeshData()
{
    elfMeshData* meshData;

    meshData = (elfMeshData*)malloc(sizeof(elfMeshData));
    memset(meshData, 0x0, sizeof(elfMeshData));
    meshData->objType = ELF_MESH_DATA;
    meshData->objDestr = elfDestroyMeshData;

    meshData->vertices = elfCreateList();
    meshData->faces = elfCreateList();

    elfIncRef((elfObject*)meshData->vertices);
    elfIncRef((elfObject*)meshData->faces);

    elfIncObj(ELF_MESH_DATA);

    return meshData;
}

void elfDestroyMeshData(void* data)
{
    elfMeshData* meshData = (elfMeshData*)data;

    elfDecRef((elfObject*)meshData->vertices);
    elfDecRef((elfObject*)meshData->faces);

    free(meshData);

    elfDecObj(ELF_MESH_DATA);
}

int elfGetMeshDataVertexCount(elfMeshData* meshData) { return elfGetListLength(meshData->vertices); }

int elfGetMeshDataFaceCount(elfMeshData* meshData) { return elfGetListLength(meshData->faces); }

void elfAddMeshDataVertex(elfMeshData* meshData, elfVertex* vertice)
{
    elfAppendListObject(meshData->vertices, (elfObject*)vertice);
}

void elfAddMeshDataFace(elfMeshData* meshData, int v1, int v2, int v3)
{
    elfFace* face;

    if (v1 < 0 || v2 < 0 || v3 < 0)
        return;
    if (v1 > elfGetListLength(meshData->vertices) || v2 > elfGetListLength(meshData->vertices) ||
        v3 > elfGetListLength(meshData->vertices))
        return;

    face = elfCreateFace();

    face->v1 = v1;
    face->v2 = v2;
    face->v3 = v3;

    elfAppendListObject(meshData->faces, (elfObject*)face);
}

elfVertex* elfGetVertexFromMeshData(elfMeshData* meshData, int idx)
{
    return (elfVertex*)elfGetListObject(meshData->vertices, idx);
}

elfFace* elfGetFaceFromMeshData(elfMeshData* meshData, int idx)
{
    return (elfFace*)elfGetListObject(meshData->faces, idx);
}
