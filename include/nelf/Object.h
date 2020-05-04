#pragma once

#include "nelf/objectHeader.h"

struct elfObject
{
    ELF_OBJECT_HEADER;
};

void elfIncRef(elfObject* obj);

void elfDecRef(elfObject* obj);

int elfGetObjectType(elfObject* obj);

int elfGetObjectRefCount(elfObject* obj);

bool elfIsActor(elfObject* obj);

bool elfIsGuiObject(elfObject* obj);