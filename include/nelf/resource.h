#pragma once

#include "nelf/resourceHeader.h"

struct elfList;

struct elfResource
{
    ELF_RESOURCE_HEADER;
};

elfResource* elfGetResource(elfList* resources, const char* name);
elfResource* elfGetResourceById(elfList* resources, int id);

void elfSetResourceUniqueName(elfList* namedObjects, elfResource* object);
