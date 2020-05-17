#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

// Base object type for all gfx objects
// TODO Same as elfObject, refactor to use common type
struct gfxObject
{
    GFX_OBJECT_HEADER;
};

// Reference counting mechanism control
void gfxIncRef(gfxObject* obj);
// This actually also destroys the object
// Which means all objects MUST be allocated
// TODO Add flag to differentiate stack / heap allocation?
void gfxDecRef(gfxObject* obj);

// Object type
GfxObjectType gfxGetObjectType(gfxObject* obj);
// Current reference count
int gfxGetObjectRefCount(gfxObject* obj);