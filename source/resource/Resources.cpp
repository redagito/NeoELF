#include "nelf/resource/Resources.h"

#include <cstdlib>
#include <cstring>

#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Object.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

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

    if (!name || strlen(name) < 1)
    {
        for (tex = (elfTexture*)elfBeginList(res->textures); tex; tex = (elfTexture*)elfGetListNext(res->textures))
        {
            if (!strcmp(tex->filePath, filePath))
                return tex;
        }
    }
    else
    {
        for (tex = (elfTexture*)elfBeginList(res->textures); tex; tex = (elfTexture*)elfGetListNext(res->textures))
        {
            if (!strcmp(tex->filePath, filePath) && !strcmp(tex->name, name))
                return tex;
        }
    }

    return NULL;
}

elfTexture* elfGetOrLoadResourcesTexture(const char* filePath, const char* name)
{
    elfTexture* texture;

    texture = elfGetResourcesTexture(filePath, name);
    if (!texture)
    {
        texture = elfCreateTextureFromFile(name, filePath);
        if (texture)
            elfAppendListObject(res->textures, (elfObject*)texture);
    }

    return texture;
}