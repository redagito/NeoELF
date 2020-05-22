#include "audio/DataDump.h"

#include <cstdlib>
#include <cstring>

struct elfDataChunk
{
    elfDataChunk* next = nullptr;
    int length = 0;
    void* data = nullptr;
};

static elfDataChunk* elfCreateDataChunk()
{
    elfDataChunk* chunk;

    chunk = (elfDataChunk*)malloc(sizeof(elfDataChunk));
    memset(chunk, 0x0, sizeof(elfDataChunk));

    return chunk;
}

static void elfDestroyDataChunk(elfDataChunk* chunk)
{
    if (chunk->next)
        elfDestroyDataChunk(chunk->next);
    if (chunk->data)
        free(chunk->data);
    free(chunk);
}

elfDataDump* elfCreateDataDump()
{
    elfDataDump* dump;

    dump = (elfDataDump*)malloc(sizeof(elfDataDump));
    memset(dump, 0x0, sizeof(elfDataDump));

    return dump;
}

void elfDestroyDataDump(elfDataDump* dump)
{
    if (dump->first)
        elfDestroyDataChunk(dump->first);
    free(dump);
}

int elfGetDataDumpLength(elfDataDump* dump)
{
    elfDataChunk* chk;
    int length;

    length = 0;

    chk = dump->first;
    while (chk)
    {
        length += chk->length;
        chk = chk->next;
    }

    return length;
}

void elfAddChunkToDataDump(elfDataDump* dump, void* data, int length)
{
    elfDataChunk* chunk;
    elfDataChunk* chk;

    chunk = elfCreateDataChunk();
    chunk->length = length;
    chunk->data = malloc(length);
    memcpy(chunk->data, data, length);

    if (!dump->first)
    {
        dump->first = chunk;
    }
    else
    {
        chk = dump->first;
        while (chk->next) chk = chk->next;
        chk->next = chunk;
    }
}

void elfDataDumpToBuffer(elfDataDump* dump, void* buf)
{
    int offset;
    elfDataChunk* chk;

    offset = 0;

    chk = dump->first;
    while (chk)
    {
        memcpy(&((char*)buf)[offset], chk->data, chk->length);
        offset += chk->length;
        chk = chk->next;
    }
}

namespace nelf
{
void Buffer::add(const unsigned char* data, size_t size) {}

void Buffer::add(const std::vector<unsigned char>& data) {}

size_t Buffer::getSize() const { return 0; }

std::vector<unsigned char> Buffer::getData() const { return {}; }

void Buffer::dumpData(unsigned char* arr, size_t size) const {}

}  // namespace nelf
