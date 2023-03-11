#include "nelf/General.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Log.h"

elfGeneral* gen = nullptr;

void elfInitGeneral()
{
    if (gen != nullptr)
        return;

    gen = (elfGeneral*)malloc(sizeof(elfGeneral));
    memset(gen, 0x0, sizeof(elfGeneral));
    gen->objType = ELF_GENERAL;
}

void elfDeinitGeneral()
{
    if (!gen)
        return;

    if (gen->refCount > 0)
    {
        elfLogWrite("error: possible memory leak in ELF, [%d] references not dereferenced\n", elfGetGlobalRefCount());
        elfDumpRefTable();
    }

    if (gen->refCount < 0)
    {
        elfLogWrite("error: possible double free in ELF, [%d] negative reference count\n", elfGetGlobalRefCount());
        elfDumpRefTable();
    }

    if (gen->objCount > 0)
    {
        elfLogWrite("error: possible memory leak in ELF, [%d] objects not destroyed\n", elfGetGlobalObjCount());
        elfDumpObjTable();
    }

    if (gen->objCount < 0)
    {
        elfLogWrite("error: possible double free in ELF, [%d] negative object count\n", elfGetGlobalObjCount());
        elfDumpObjTable();
    }

    free(gen);
}

void elfIncObj(int type)
{
    if (gen == nullptr)
        return;
    gen->objCount++;
    gen->objTable[type]++;
}

void elfDecObj(int type)
{
    if (gen == nullptr)
        return;
    gen->objCount--;
    gen->objTable[type]--;
}

void elfDumpRefTable()
{
    if (gen == nullptr)
        return;
    int i;

    elfLogWrite("---------- REF COUNT TABLE ----------\n");

    for (i = 0; i < ELF_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gen->refTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

void elfDumpObjTable()
{
    if (gen == nullptr)
        return;
    int i;

    elfLogWrite("---------- OBJ COUNT TABLE ----------\n");

    for (i = 0; i < ELF_OBJECT_TYPE_COUNT; i++)
    {
        elfLogWrite("%d : %d\n", i, gen->objTable[i]);
    }

    elfLogWrite("-------------------------------------\n");
}

int elfGetGlobalRefCount()
{
    if (gen == nullptr)
        return 0;
    return gen->refCount;
}

int elfGetGlobalObjCount()
{
    if (gen == nullptr)
        return 0;
    return gen->objCount;
}
