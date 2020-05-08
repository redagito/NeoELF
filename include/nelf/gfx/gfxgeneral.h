#pragma once

#include "nelf/gfx/gfxObjectType.h"

struct gfxGeneral
{
    int refCount = 0;
    int objCount = 0;
    int refTable[GFX_OBJECT_TYPE_COUNT];
    int objTable[GFX_OBJECT_TYPE_COUNT];
};

extern gfxGeneral* gfxGen;

gfxGeneral* gfxCreateGeneral();
void gfxDestroyGeneral(gfxGeneral* general);

void gfxDumpRefTable();
void gfxDumpObjTable();

void gfxIncObj(int type);
void gfxDecObj(int type);

int gfxGetGlobalRefCount();
int gfxGetGlobalObjCount();
