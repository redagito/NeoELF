#pragma once

struct elfDataChunk
{
    elfDataChunk* next = nullptr;
    int length = 0;
    void* data = nullptr;
};

struct elfDataDump
{
    elfDataChunk* first = nullptr;
    int offset = 0;
};

elfDataChunk* elfCreateDataChunk();

void elfDestroyDataChunk(elfDataChunk* chunk);

elfDataDump* elfCreateDataDump();

void elfDestroyDataDump(elfDataDump* dump);

int elfGetDataDumpLength(elfDataDump* dump);

void elfAddChunkToDataDump(elfDataDump* dump, void* data, int length);

void elfDataDumpToBuffer(elfDataDump* dump, void* buf);