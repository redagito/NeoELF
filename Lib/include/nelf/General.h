#pragma once

#include "nelf/objectHeader.h"
#include "nelf/objectType.h"

// Global object lifetime tracking
// and some other settings that probably should not be here
// TODO Maybe just call this "Objects"?
struct elfGeneral
{
    ELF_OBJECT_HEADER;

    int refCount = 0;

    // Total object count during runtime
    int objCount = 0;
    int refTable[ELF_OBJECT_TYPE_COUNT];
    // Total counts for each object type
    int objTable[ELF_OBJECT_TYPE_COUNT];
};

// Globally accessible
extern elfGeneral* gen;

void elfInitGeneral();
void elfDeinitGeneral();

void elfIncObj(int type);
void elfDecObj(int type);

void elfDumpRefTable();
void elfDumpObjTable();

int elfGetGlobalRefCount();
int elfGetGlobalObjCount();
