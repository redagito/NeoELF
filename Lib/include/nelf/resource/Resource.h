#pragma once

#include "nelf/resource/resourceHeader.h"

struct elfList;

struct elfResource
{
    ELF_RESOURCE_HEADER;
};

elfResource* elfGetResource(elfList* resources, const char* name);
elfResource* elfGetResourceById(elfList* resources, int id);

// Sets the resource object name to a value that is unique for all objects
// in the provided collection
// NOTE: This does NOT add the object to the collection
void elfSetResourceUniqueName(elfList* namedObjects, elfResource* object);