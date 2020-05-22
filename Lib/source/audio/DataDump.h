#pragma once

#include <list>
#include <vector>

struct elfDataChunk;

struct elfDataDump
{
    elfDataChunk* first = nullptr;
    int offset = 0;
};

elfDataDump* elfCreateDataDump();
void elfDestroyDataDump(elfDataDump* dump);

int elfGetDataDumpLength(elfDataDump* dump);
void elfAddChunkToDataDump(elfDataDump* dump, void* data, int length);
void elfDataDumpToBuffer(elfDataDump* dump, void* buf);

namespace nelf
{
class Buffer
{
   public:
    // Add data to buffer
    void add(const unsigned char* data, size_t size);
    void add(const std::vector<unsigned char>& data);

    // Size of data in buffer in bytes
    size_t getSize() const;

    // Return data as single array
    std::vector<unsigned char> getData() const;
    // Write to provided array
    void dumpData(unsigned char* arr, size_t size) const;

   private:
    std::list<std::vector<unsigned char>> chunks;
};

}  // namespace nelf