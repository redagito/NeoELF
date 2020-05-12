#include "nelf/Face.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/objectType.h"

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