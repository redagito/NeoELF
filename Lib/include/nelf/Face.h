#pragma once

#include "nelf/objectHeader.h"

// TODO Basically same question as for elfVertex
struct elfFace
{
    ELF_OBJECT_HEADER;
    // Indices into a list of vertices
    int v1 = 0;
    int v2 = 0;
    int v3 = 0;
};

elfFace* elfCreateFace();
void elfDestroyFace(void* data);

int elfGetFaceV1(elfFace* face);
int elfGetFaceV2(elfFace* face);
int elfGetFaceV3(elfFace* face);