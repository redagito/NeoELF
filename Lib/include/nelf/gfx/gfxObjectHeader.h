#pragma once

#include "nelf/gfx/gfxObjectType.h"

#define GFX_OBJECT_HEADER                    \
    GfxObjectType objType = GFX_OBJECT_NONE; \
    int objRefCount = 0;                     \
    void (*objDestr)(void*) = nullptr