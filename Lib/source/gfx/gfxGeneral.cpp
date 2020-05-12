#include "nelf/gfx/gfxGeneral.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Log.h"

gfxGeneral* gfxGen = nullptr;

gfxGeneral* gfxCreateGeneral()
{
    gfxGeneral* general;

    general = (gfxGeneral*)malloc(sizeof(gfxGeneral));
    memset(general, 0x0, sizeof(gfxGeneral));

    return general;
}

void gfxDestroyGeneral(gfxGeneral* general) { free(general); }

void gfxDumpRefTable()
{
    int i;

    elfLogWrite("---------- REF COUNT TABLE ----------\n");

    for (i = 0; i < GFX_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gfxGen->refTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

void gfxDumpObjTable()
{
    int i;

    elfLogWrite("---------- OBJ COUNT TABLE ----------\n");

    for (i = 0; i < GFX_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gfxGen->objTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

void gfxIncObj(int type)
{
    gfxGen->objCount++;
    gfxGen->objTable[type]++;
}

void gfxDecObj(int type)
{
    gfxGen->objCount--;
    gfxGen->objTable[type]--;
}

int gfxGetGlobalRefCount() { return gfxGen->refCount; }

int gfxGetGlobalObjCount() { return gfxGen->objCount; }
