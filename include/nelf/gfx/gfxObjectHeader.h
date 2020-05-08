#pragma once

#define GFX_OBJECT_HEADER \
    int objType = 0;      \
    int objRefCount = 0;  \
    void (*objDestr)(void*) = nullptr