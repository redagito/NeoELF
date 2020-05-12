#pragma once

#include "nelf/objectHeader.h"

struct elfPakIndex
{
    ELF_OBJECT_HEADER;
    unsigned char indexType = 0;
    char* name = nullptr;
    unsigned int offset = 0;
};

elfPakIndex* elfCreatePakIndex();
void elfDestroyPakIndex(void* data);

unsigned char elfGetPakIndexType(elfPakIndex* index);
const char* elfGetPakIndexName(elfPakIndex* index);
int elfGetPakIndexOffset(elfPakIndex* index);