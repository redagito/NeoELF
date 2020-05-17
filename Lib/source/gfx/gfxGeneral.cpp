#include "gfx/gfxGeneral.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Log.h"

gfxGeneral* gfxGen = nullptr;

gfxGeneral* gfxCreateGeneral()
{
    gfxGeneral* general = (gfxGeneral*)malloc(sizeof(gfxGeneral));
    memset(general, 0x0, sizeof(gfxGeneral));

    return general;
}

void gfxDestroyGeneral(gfxGeneral* general) { free(general); }

void gfxDumpRefTable()
{
    elfLogWrite("---------- REF COUNT TABLE ----------\n");

    if (gfxGen == nullptr)
    {
        elfLogWrite("No data\n");
        return;
    }

    for (int i = 0; i < GFX_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gfxGen->refTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

void gfxDumpObjTable()
{
    elfLogWrite("---------- OBJ COUNT TABLE ----------\n");

    if (gfxGen == nullptr)
    {
        elfLogWrite("No data\n");
        return;
    }

    for (int i = 0; i < GFX_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gfxGen->objTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

void gfxIncObj(GfxObjectType type)
{
    if (gfxGen == nullptr)
        return;
    gfxGen->objCount++;
    gfxGen->objTable[type]++;
}

void gfxDecObj(GfxObjectType type)
{
    if (gfxGen == nullptr)
        return;
    gfxGen->objCount--;
    gfxGen->objTable[type]--;
}

int gfxGetGlobalRefCount()
{
    if (gfxGen == nullptr)
        return 0;
    return gfxGen->refCount;
}

int gfxGetGlobalObjCount()
{
    if (gfxGen == nullptr)
        return 0;
    return gfxGen->objCount;
}
