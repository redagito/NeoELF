#include "nelf/Directory.h"

#include <cstdlib>
#include <cstring>

// 3rd party
#include "dirent/dirent.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/String.h"
#include "nelf/directoryItemType.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

elfDirectoryItem* elfCreateDirectoryItem()
{
    elfDirectoryItem* dirItem;

    dirItem = (elfDirectoryItem*)malloc(sizeof(elfDirectoryItem));
    memset(dirItem, 0x0, sizeof(elfDirectoryItem));
    dirItem->objType = ELF_DIRECTORY_ITEM;
    dirItem->objDestr = elfDestroyDirectoryItem;

    elfIncObj(ELF_DIRECTORY_ITEM);

    return dirItem;
}

void elfDestroyDirectoryItem(void* data)
{
    elfDirectoryItem* directoryItem = (elfDirectoryItem*)data;

    if (directoryItem->name)
        elfDestroyString(directoryItem->name);

    free(directoryItem);

    elfDecObj(ELF_DIRECTORY_ITEM);
}

elfDirectory* elfCreateDirectory()
{
    elfDirectory* directory;

    directory = (elfDirectory*)malloc(sizeof(elfDirectory));
    memset(directory, 0x0, sizeof(elfDirectory));
    directory->objType = ELF_DIRECTORY;
    directory->objDestr = elfDestroyDirectory;

    directory->items = elfCreateList();

    elfIncObj(ELF_DIRECTORY);

    return directory;
}

void elfDestroyDirectory(void* data)
{
    elfDirectory* directory = (elfDirectory*)data;

    if (directory->path)
        elfDestroyString(directory->path);
    elfDestroyList(directory->items);

    free(directory);

    elfDecObj(ELF_DIRECTORY);
}

void elfAppendDirectoryItemListFolder(elfList* items, elfDirectoryItem* nitem)
{
    elfDirectoryItem* dirItem;
    int i;

    for (i = 0, dirItem = (elfDirectoryItem*)elfBeginList(items); dirItem;
         dirItem = (elfDirectoryItem*)elfGetListNext(items), i++)
    {
        if (dirItem->itemType == ELF_FILE)
        {
            elfInsertListObject(items, i, (elfObject*)nitem);
            return;
        }
        else
            continue;
    }

    elfAppendListObject(items, (elfObject*)nitem);
}

typedef struct elfDirItemEmul
{
    char* str;
    int type;
} elfDirItemEmul;

static int alphacmp(const void* a, const void* b)
{
    return strcmp((*((elfDirItemEmul*)a)).str, (*((elfDirItemEmul*)b)).str);
}

elfDirectory* elfReadDirectory(const char* path)
{
    elfDirectory* directory;
    elfDirectoryItem* dirItem;
    DIR* dir;
    struct dirent* dp;
    elfDirItemEmul* names;
    int itemCount;
    int i;

    if (!(dir = opendir(path)))
    {
        elfSetError(ELF_CANT_OPEN_DIRECTORY, "error: could not open directory \"%s\"\n", path);
        return NULL;
    }
    else
    {
        directory = elfCreateDirectory();
        directory->path = elfCreateString(path);

        while ((dp = readdir(dir)))
        {
            dirItem = elfCreateDirectoryItem();
            dirItem->name = elfCreateString(dp->d_name);

#ifdef WIN32
            if (dp->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                dirItem->itemType = ELF_DIR;
            else
                dirItem->itemType = ELF_FILE;
#else
            if (dp->d_type == 4)
                dirItem->itemType = ELF_DIR;
            else
                dirItem->itemType = ELF_FILE;
#endif

            elfAppendListObject(directory->items, (elfObject*)dirItem);
        }

        itemCount = elfGetListLength(directory->items);

        names = (elfDirItemEmul*)malloc(sizeof(elfDirItemEmul) * itemCount);
        memset(names, 0x0, sizeof(elfDirItemEmul) * itemCount);

        for (i = 0, dirItem = (elfDirectoryItem*)elfBeginList(directory->items); dirItem;
             dirItem = (elfDirectoryItem*)elfGetListNext(directory->items), i++)
        {
            names[i].str = (char*)malloc(sizeof(char) * (strlen(dirItem->name) + 1));
            memcpy(names[i].str, dirItem->name, sizeof(char) * (strlen(dirItem->name) + 1));
            names[i].type = dirItem->itemType;
        }

        qsort(names, itemCount, sizeof(elfDirItemEmul), alphacmp);

        elfDestroyDirectory(directory);

        directory = elfCreateDirectory();
        directory->path = elfCreateString(path);

        for (i = 0; i < itemCount; i++)
        {
            dirItem = elfCreateDirectoryItem();
            dirItem->name = elfCreateString(names[i].str);
            dirItem->itemType = names[i].type;

            if (dirItem->itemType == ELF_DIR)
                elfAppendDirectoryItemListFolder(directory->items, dirItem);
            else
                elfAppendListObject(directory->items, (elfObject*)dirItem);

            free(names[i].str);
        }

        free(names);

        return directory;
    }
}

const char* elfGetDirectoryPath(elfDirectory* directory) { return directory->path; }

int elfGetDirectoryItemCount(elfDirectory* directory) { return elfGetListLength(directory->items); }

elfDirectoryItem* elfGetDirectoryItem(elfDirectory* directory, int idx)
{
    int i;
    elfDirectoryItem* dirItem;

    if (idx < 0 || idx > elfGetListLength(directory->items) - 1)
        return NULL;

    for (i = 0, dirItem = (elfDirectoryItem*)elfBeginList(directory->items); dirItem;
         dirItem = (elfDirectoryItem*)elfGetListNext(directory->items), i++)
    {
        if (i == idx)
            return dirItem;
    }

    return NULL;
}

const char* elfGetDirectoryItemName(elfDirectoryItem* dirItem) { return dirItem->name; }

int elfGetDirectoryItemType(elfDirectoryItem* dirItem) { return dirItem->itemType; }

char* elfGetFileFromPath(const char* filePath)
{
    int idx;

    if (strlen(filePath) < 1)
        return elfCreateString("");

    idx = elfRFindCharsFromString("/\\", filePath);
    if (idx < 1 || idx == strlen(filePath) - 1)
    {
        return elfCreateString("");
    }
    else
    {
        return elfSubString((char*)filePath, idx + 1, strlen(filePath) - (idx + 1));
    }
}

char* elfGetDirectoryFromPath(const char* filePath)
{
    int idx;

    if (strlen(filePath) < 1)
        return elfCreateString("");

    idx = elfRFindCharsFromString("/\\", filePath);
    if (idx < 1)
    {
        return elfCreateString("");
    }
    else
    {
        return elfSubString((char*)filePath, 0, idx + 1);
    }
}