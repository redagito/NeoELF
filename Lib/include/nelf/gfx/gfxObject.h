#pragma once

#include "nelf/gfx/gfxObjectHeader.h"

struct gfxObject
{
    GFX_OBJECT_HEADER;
};

void gfxIncRef(gfxObject* obj);
void gfxDecRef(gfxObject* obj);

int gfxGetObjectType(gfxObject* obj);
int gfxGetObjectRefCount(gfxObject* obj);