#pragma once

// Provides data fields for
// - type id, see objectTypes.h
// - reference counting
// - destructor callback
#define ELF_OBJECT_HEADER \
    int objType = 0;      \
    int objRefCount = 0;  \
    void (*objDestr)(void*) = nullptr
