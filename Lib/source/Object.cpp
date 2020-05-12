#include "nelf/Object.h"

#include "nelf/General.h"
#include "nelf/Log.h"

void elfIncRef(elfObject* obj)
{
    if (gen != nullptr)
    {
        gen->refCount++;
        gen->refTable[obj->objType]++;
    }
    obj->objRefCount++;
}

void elfDecRef(elfObject* obj)
{
    if (gen != nullptr)
    {
        gen->refCount--;
        gen->refTable[obj->objType]--;
    }
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

int elfGetObjectType(elfObject* obj) { return obj->objType; }

int elfGetObjectRefCount(elfObject* obj) { return obj->objRefCount; }

bool elfIsActor(elfObject* obj)
{
    if (obj->objType == ELF_CAMERA || obj->objType == ELF_ENTITY || obj->objType == ELF_LIGHT ||
        obj->objType == ELF_PARTICLES || obj->objType == ELF_SPRITE)
        return true;
    return false;
}

bool elfIsGuiObject(elfObject* obj)
{
    if (obj->objType == ELF_GUI || obj->objType == ELF_LABEL || obj->objType == ELF_BUTTON ||
        obj->objType == ELF_PICTURE || obj->objType == ELF_TEXT_FIELD || obj->objType == ELF_SLIDER ||
        obj->objType == ELF_SCREEN || obj->objType == ELF_TEXT_LIST || obj->objType == ELF_CHECK_BOX)
        return true;
    return false;
}