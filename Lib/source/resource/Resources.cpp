#include "nelf/resource/Resources.h"

#include <cstdlib>
#include <cstring>

#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Object.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"
#include "nelf/resource/Texture.h"

elfResources* res = nullptr;

elfResources* elfCreateResources()
{
    elfResources* resources;

    resources = (elfResources*)malloc(sizeof(elfResources));
    memset(resources, 0x0, sizeof(elfResources));
    resources->objType = ELF_RESOURCES;
    resources->objDestr = elfDestroyResources;

    resources->textures = elfCreateList();
    elfIncRef((elfObject*)resources->textures);

    return resources;
}

void elfDestroyResources(void* data)
{
    elfResources* resources = (elfResources*)data;

    elfDecRef((elfObject*)resources->textures);

    free(resources);
}

bool elfInitResources()
{
    if (res)
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: can't initialize resources twice!\n");
        return false;
    }

    res = elfCreateResources();
    elfIncRef((elfObject*)res);

    return true;
}

void elfDeinitResources()
{
    if (res)
    {
        elfDecRef((elfObject*)res);
        res = NULL;
    }
}

elfTexture* elfGetResourcesTexture(const char* filePath, const char* name)
{
    elfTexture* tex;

    if (name == nullptr || strlen(name) == 0)
    {
        // Name not set, fetch by filepath
        for (tex = (elfTexture*)elfBeginList(res->textures); tex; tex = (elfTexture*)elfGetListNext(res->textures))
        {
            if (strcmp(tex->filePath, filePath) == 0)
                return tex;
        }
    }
    else
    {
        // Name is set, must match filepath and name
        for (tex = (elfTexture*)elfBeginList(res->textures); tex; tex = (elfTexture*)elfGetListNext(res->textures))
        {
            if (strcmp(tex->filePath, filePath) == 0 && strcmp(tex->name, name) == 0)
                return tex;
        }
    }

    // Not found
    return nullptr;
}

elfTexture* elfGetOrLoadResourcesTexture(const char* filePath, const char* name)
{
    elfTexture* texture;

    // Get from loaded textures
    texture = elfGetResourcesTexture(filePath, name);
    if (texture == nullptr)
    {
        // Load from file
        texture = elfCreateTextureFromFile(name, filePath);
        if (texture != nullptr)
            elfAppendListObject(res->textures, (elfObject*)texture);
    }

    return texture;
}