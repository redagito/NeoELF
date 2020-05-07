#pragma once

#include "nelf/objectHeader.h"

struct btTriangleMesh;

struct elfPhysicsTriMesh
{
    ELF_OBJECT_HEADER;
    btTriangleMesh* triMesh = nullptr;
};

elfPhysicsTriMesh* elfCreatePhysicsTriMesh(float* verts, unsigned int* idx, int indiceCount);
void elfDestroyPhysicsTriMesh(void* data);