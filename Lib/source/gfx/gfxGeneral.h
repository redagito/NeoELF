#pragma once

#include "nelf/gfx/gfxObjectType.h"

// Runtime statistics for gfx objecs
struct gfxGeneral
{
    // Total reference ad object counts
    int refCount = 0;
    int objCount = 0;

    // Reference and object counts per object type
    int refTable[GFX_OBJECT_TYPE_COUNT];
    int objTable[GFX_OBJECT_TYPE_COUNT];
};

extern gfxGeneral* gfxGen;

gfxGeneral* gfxCreateGeneral();
void gfxDestroyGeneral(gfxGeneral* general);

// Writes to log, dependency on nelf/Log.h
void gfxDumpRefTable();
void gfxDumpObjTable();

void gfxIncObj(GfxObjectType type);
void gfxDecObj(GfxObjectType type);

int gfxGetGlobalRefCount();
int gfxGetGlobalObjCount();
