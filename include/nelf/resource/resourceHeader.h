#pragma once

#include "objectHeader.h"

// Common data for all resource types, provides
// - id, a resource id
// - name, string identifier
#define ELF_RESOURCE_HEADER \
    ELF_OBJECT_HEADER;      \
    int id;                 \
    char* name