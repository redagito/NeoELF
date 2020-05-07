
struct elfDirectory
{
    ELF_OBJECT_HEADER;
    char* path;
    elfList* items;
};

struct elfDirectoryItem
{
    ELF_OBJECT_HEADER;
    char* name;
    unsigned char itemType;
};

struct elfRequest
{
    ELF_RESOURCE_HEADER;
    char* url;
    char* method;
};
