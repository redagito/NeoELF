#pragma once

#include "objectHeader.h"

#define ELF_RESOURCE_HEADER \
    ELF_OBJECT_HEADER;      \
    int id;                 \
    char* name