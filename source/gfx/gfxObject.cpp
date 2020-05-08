#include "nelf/gfx/gfxObject.h"

#include "nelf/Log.h"
#include "nelf/gfx/gfxGeneral.h"

void gfxIncRef(gfxObject* obj)
{
    gfxGen->refCount++;
    gfxGen->refTable[obj->objType]++;
    obj->objRefCount++;
}

void gfxDecRef(gfxObject* obj)
{
    gfxGen->refCount--;
    gfxGen->refTable[obj->objType]--;
    obj->objRefCount--;

    if (obj->objRefCount < 1)
    {
        if (obj->objDestr)
        {
            obj->objDestr(obj);
        }
        else
        {
            elfLogWrite("error: no destructor specified for object\n");
        }
    }
}

int gfxGetObjectType(gfxObject* obj) { return obj->objType; }

int gfxGetObjectRefCount(gfxObject* obj) { return obj->objRefCount; }