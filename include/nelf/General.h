#pragma once

#include "nelf/objectHeader.h"
#include "nelf/objectType.h"

// Global object lifetime tracking
// and some other settings that probably should not be here
// TODO Maybe just call this "Objects" and move unrelated stuff out
struct elfGeneral
{
    ELF_OBJECT_HEADER;
    // Path to log file
    // TODO Why is this here and not managed by log.h
    char* log = nullptr;

    // Last error that happened
    // TODO Why is this here, should be managed by log.h
    char* errStr = nullptr;
    // Last error code
    // TODO Together with errStr put into separate struct Error?
    int errCode = 0;

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

// Why is this here?
// TODO Move to log and have it manage this
void elfSetLogFilePath(const char* filePath);
