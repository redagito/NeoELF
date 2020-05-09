#pragma once

#include "nelf/objectHeader.h"

// Common data for all resource types, provides
// - id, a resource id
// - name, string identifier
#define ELF_RESOURCE_HEADER \
    ELF_OBJECT_HEADER;      \
    int id = 0;             \
    char* name = nullptr