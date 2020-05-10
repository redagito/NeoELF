#pragma once

#include "nelf/objectHeader.h"

struct elfList;

// TODO Use std::filesystem instead

// Single item in directory
struct elfDirectoryItem
{
    ELF_OBJECT_HEADER;
    char* name = nullptr;
    unsigned char itemType = 0;
};

elfDirectoryItem* elfCreateDirectoryItem();
void elfDestroyDirectoryItem(void* data);

// Directory
struct elfDirectory
{
    ELF_OBJECT_HEADER;
    char* path = nullptr;
    elfList* items = nullptr;
};

elfDirectory* elfCreateDirectory();
void elfDestroyDirectory(void* data);

void elfAppendDirectoryItemListFolder(elfList* items, elfDirectoryItem* nitem);

elfDirectory* elfReadDirectory(const char* path);

const char* elfGetDirectoryPath(elfDirectory* directory);
int elfGetDirectoryItemCount(elfDirectory* directory);
elfDirectoryItem* elfGetDirectoryItem(elfDirectory* directory, int idx);
const char* elfGetDirectoryItemName(elfDirectoryItem* dirItem);
int elfGetDirectoryItemType(elfDirectoryItem* dirItem);

// Utility
char* elfGetFileFromPath(const char* filePath);
char* elfGetDirectoryFromPath(const char* filePath);