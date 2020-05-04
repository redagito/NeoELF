
elfVertex* elfCreateVertex()
{
    elfVertex* vertice;

    vertice = (elfVertex*)malloc(sizeof(elfVertex));
    memset(vertice, 0x0, sizeof(elfVertex));
    vertice->objType = ELF_VERTEX;
    vertice->objDestr = elfDestroyVertex;

    elfIncObj(ELF_VERTEX);

    return vertice;
}

void elfDestroyVertex(void* data)
{
    elfVertex* vertice = (elfVertex*)data;

    free(vertice);

    elfDecObj(ELF_VERTEX);
}

void elfSetVertexPosition(elfVertex* vertice, float x, float y, float z)
{
    vertice->position.x = x;
    vertice->position.y = y;
    vertice->position.z = z;
}

void elfSetVertexNormal(elfVertex* vertice, float x, float y, float z)
{
    vertice->normal.x = x;
    vertice->normal.y = y;
    vertice->normal.z = z;
}

void elfSetVertexTexCoord(elfVertex* vertice, float u, float v)
{
    vertice->texCoord.x = u;
    vertice->texCoord.y = v;
}

elfVec3f elfGetVertexPosition(elfVertex* vertice) { return vertice->position; }

elfVec3f elfGetVertexNormal(elfVertex* vertice) { return vertice->normal; }

elfVec2f elfGetVertexTexCoord(elfVertex* vertice) { return vertice->texCoord; }

elfFace* elfCreateFace()
{
    elfFace* face;

    face = (elfFace*)malloc(sizeof(elfFace));
    memset(face, 0x0, sizeof(elfFace));
    face->objType = ELF_FACE;
    face->objDestr = elfDestroyFace;

    elfIncObj(ELF_FACE);

    return face;
}

void elfDestroyFace(void* data)
{
    elfFace* face = (elfFace*)data;

    free(face);

    elfDecObj(ELF_FACE);
}

int elfGetFaceV1(elfFace* face) { return face->v1; }

int elfGetFaceV2(elfFace* face) { return face->v2; }

int elfGetFaceV3(elfFace* face) { return face->v3; }

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
