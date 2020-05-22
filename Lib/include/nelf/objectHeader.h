#pragma once

#include "nelf/objectType.h"

// Provides data fields for
// - type id, see objectTypes.h
// - reference counting
// - destructor callback
// TODO The functionality of this can be replaced by shared_ptr and the normal C++ dtor
//      Only issue might be the objType but this can be solved with better design
#define ELF_OBJECT_HEADER   \
    ObjectType objType = 0; \
    int objRefCount = 0;    \
    void (*objDestr)(void*) = nullptr
