#pragma once

#include "nelf/objectHeader.h"

struct elfList;
struct elfTexture;

struct elfResources
{
    ELF_OBJECT_HEADER;
    elfList* textures = nullptr;

    int sceneIdCounter = 0;
    int scriptIdCounter = 0;
    int textureIdCounter = 0;
    int materialIdCounter = 0;
    int modelIdCounter = 0;
    int cameraIdCounter = 0;
    int entityIdCounter = 0;
    int lightIdCounter = 0;
    int armatureIdCounter = 0;
    int particlesIdCounter = 0;
    int spriteIdCounter = 0;
};

// Globally accessible
elfResources* res;

elfResources* elfCreateResources();

void elfDestroyResources(void* data);

bool elfInitResources();

void elfDeinitResources();

elfTexture* elfGetResourcesTexture(const char* filePath, const char* name);

elfTexture* elfGetOrLoadResourcesTexture(const char* filePath, const char* name);