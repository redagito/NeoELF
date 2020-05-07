#include "nelf/pak/PakIndex.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/String.h"
#include "nelf/objectType.h"

elfPakIndex* elfCreatePakIndex()
{
    elfPakIndex* index;

    index = (elfPakIndex*)malloc(sizeof(elfPakIndex));
    memset(index, 0x0, sizeof(elfPakIndex));
    index->objType = ELF_PAK_INDEX;
    index->objDestr = elfDestroyPakIndex;

    elfIncObj(ELF_PAK_INDEX);

    return index;
}

void elfDestroyPakIndex(void* data)
{
    elfPakIndex* index = (elfPakIndex*)data;

    if (index->name)
        elfDestroyString(index->name);

    free(index);

    elfDecObj(ELF_PAK_INDEX);
}

unsigned char elfGetPakIndexType(elfPakIndex* index) { return index->objType; }

const char* elfGetPakIndexName(elfPakIndex* index) { return index->name; }

int elfGetPakIndexOffset(elfPakIndex* index) { return index->offset; }