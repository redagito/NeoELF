#pragma once

#include "nelf/objectHeader.h"

struct elfList;
struct elfTexture;

// Info for resource lifetime and totla resource usage
struct elfResources
{
    ELF_OBJECT_HEADER;
    // List of all loaded textures
    elfList* textures = nullptr;

    // Separate counters for each resource type
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
extern elfResources* res;

elfResources* elfCreateResources();
void elfDestroyResources(void* data);

bool elfInitResources();
void elfDeinitResources();

// Get from loaded textures
// TODO Weird that filepath is required here
elfTexture* elfGetResourcesTexture(const char* filePath, const char* name);
// Get from loaded or load from file
elfTexture* elfGetOrLoadResourcesTexture(const char* filePath, const char* name);