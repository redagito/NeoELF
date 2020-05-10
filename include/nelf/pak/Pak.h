#pragma once

#include "nelf/objectHeader.h"

struct elfList;
struct elfResource;
struct elfPakIndex;
struct elfActor;
struct elfArmature;
struct elfCamera;
struct elfEntity;
struct elfLight;
struct elfMaterial;
struct elfModel;
struct elfParticles;
struct elfScript;
struct elfSprite;

// TODO Lazy loading / hot reloading
// TODO Move to separate lib
struct elfPak
{
    ELF_OBJECT_HEADER;
    char* filePath = nullptr;
    elfList* indexes = nullptr;

    // TODO Should be unsigned
    int textureCount = 0;
    int materialCount = 0;
    int modelCount = 0;
    int cameraCount = 0;
    int entityCount = 0;
    int lightCount = 0;
    int armatureCount = 0;
    int sceneCount = 0;
    int scriptCount = 0;
};

elfPak* elfCreatePakFromFile(const char* filePath);

void elfDestroyPak(void* data);

const char* elfGetPakFilePath(elfPak* pak);
int elfGetPakIndexCount(elfPak* pak);

elfPakIndex* elfGetPakIndexByName(elfPak* pak, const char* name, unsigned char type);
elfPakIndex* elfGetPakIndexByIndex(elfPak* pak, int idx);

// Calculates size in byte
// TODO These functions should probably just be part of the interface for the respective types
int elfGetActorHeaderSizeBytes(elfActor* actor);
int elfGetArmatureSizeBytes(elfArmature* armature);
int elfGetCameraSizeBytes(elfCamera* camera);
int elfGetEntitySizeBytes(elfEntity* entity);
int elfGetLightSizeBytes(elfLight* light);
int elfGetMaterialSizeBytes(elfMaterial* material);
int elfGetModelSizeBytes(elfModel* model);
int elfGetSceneSizeBytes(elfScene* scene);
int elfGetParticlesSizeBytes(elfParticles* particles);
int elfGetScriptSizeBytes(elfScript* script);
int elfGetSpriteSizeBytes(elfSprite* sprite);
int elfGetTextureSizeBytes(elfTexture* texture);

void elfReadActorHeader(elfActor* actor, FILE* file, elfScene* scene);

// Create from PAK file
elfArmature* elfCreateArmatureFromPak(FILE* file, const char* name, elfScene* scene);
elfCamera* elfCreateCameraFromPak(FILE* file, const char* name, elfScene* scene);
elfEntity* elfCreateEntityFromPak(FILE* file, const char* name, elfScene* scene);
elfLight* elfCreateLightFromPak(FILE* file, const char* name, elfScene* scene);
elfMaterial* elfCreateMaterialFromPak(FILE* file, const char* name, elfScene* scene);
elfModel* elfCreateModelFromPak(FILE* file, const char* name, elfScene* scene);
elfParticles* elfCreateParticlesFromPak(FILE* file, const char* name, elfScene* scene);
elfScript* elfCreateScriptFromPak(FILE* file, const char* name, elfScene* scene);
elfSprite* elfCreateSpriteFromPak(FILE* file, const char* name, elfScene* scene);
elfTexture* elfCreateTextureFromPak(FILE* file, const char* name, elfScene* scene);
elfScene* elfCreateSceneFromPak(const char* name, elfPak* pak);

// Write to PAK file
void elfWriteNameToFile(const char* name, FILE* file);
void elfWriteActorHeader(elfActor* actor, FILE* file);
void elfWriteArmatureToFile(elfArmature* armature, FILE* file);
void elfWriteCameraToFile(elfCamera* camera, FILE* file);
void elfWriteEntityToFile(elfEntity* entity, FILE* file);
void elfWriteLightToFile(elfLight* light, FILE* file);
void elfWriteMaterialToFile(elfMaterial* material, FILE* file);
void elfWriteModelToFile(elfModel* model, FILE* file);
void elfWriteParticlesToFile(elfParticles* particles, FILE* file);
void elfWriteSceneToFile(elfScene* scene, FILE* file);
void elfWriteScriptToFile(elfScript* script, FILE* file);
void elfWriteSpriteToFile(elfSprite* sprite, FILE* file);
void elfWriteTextureToFile(elfTexture* texture, FILE* file);

void elfWriteResourceIndexToFile(elfResource* resource, unsigned int* offset, FILE* file);
void elfWriteResourceIndexesToFile(elfList* resources, unsigned int* offset, FILE* file);
void elfWriteResourcesToFile(elfList* resources, FILE* file);

void elfAddTextureForSaving(elfList* textures, elfTexture* texture);

// Save full scene
// TODO Why does this not use same convention as write methods?
//      Whats special? Maybe top level entry point?
unsigned char elfSaveSceneToPak(elfScene* scene, const char* filePath);