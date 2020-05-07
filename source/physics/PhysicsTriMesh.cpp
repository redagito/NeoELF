#include "nelf/physics/PhysicsTriMesh.h"

#include <bullet/btBulletCollisionCommon.h>

#include <cstdlib>
#include <string>

#include "nelf/General.h"
#include "nelf/objectType.h"

elfPhysicsTriMesh* elfCreatePhysicsTriMesh(float* verts, unsigned int* idx, int indiceCount)
{
    elfPhysicsTriMesh* triMesh;
    int i;

    if (indiceCount < 3)
        return nullptr;

    triMesh = (elfPhysicsTriMesh*)malloc(sizeof(elfPhysicsTriMesh));
    memset(triMesh, 0x0, sizeof(elfPhysicsTriMesh));
    triMesh->objType = ELF_PHYSICS_TRI_MESH;
    triMesh->objDestr = elfDestroyPhysicsTriMesh;

    triMesh->triMesh = new btTriangleMesh();

    for (i = 0; i < indiceCount; i += 3)
    {
        triMesh->triMesh->addTriangle(
            btVector3(verts[idx[i] * 3], verts[idx[i] * 3 + 1], verts[idx[i] * 3 + 2]),
            btVector3(verts[idx[i + 1] * 3], verts[idx[i + 1] * 3 + 1], verts[idx[i + 1] * 3 + 2]),
            btVector3(verts[idx[i + 2] * 3], verts[idx[i + 2] * 3 + 1], verts[idx[i + 2] * 3 + 2]));
    }

    elfIncObj(ELF_PHYSICS_TRI_MESH);

    return triMesh;
}

void elfDestroyPhysicsTriMesh(void* data)
{
    elfPhysicsTriMesh* triMesh = (elfPhysicsTriMesh*)data;

    delete triMesh->triMesh;

    free(triMesh);

    elfDecObj(ELF_PHYSICS_TRI_MESH);
}