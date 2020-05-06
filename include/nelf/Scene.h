#pragma once

#include "nelf/Color.h"
#include "nelf/Vector.h"
#include "nelf/resourceHeader.h"

struct elfList;
struct elfPhysicsWorld;
struct elfCamera;
struct elfPak;
struct elfMaterial;
struct elfActor;
struct elfLight;
struct elfParticle;
struct elfEntity;
struct elfParticles;
struct elfSprite;
struct elfCollision;
struct elfArmature;
struct elfModel;
struct elfScript;
struct gfxShaderProgram;

// Assimp
struct aiScene;
struct aiNode;

struct elfScene
{
    ELF_RESOURCE_HEADER;
    char* filePath = nullptr;

    bool physics = true;
    bool runScripts = true;
    bool debugDraw = true;
    bool occlusionCulling = true;

    elfColor ambientColor;

    bool fog;
    float fogStart = 0.f;
    float fogEnd = 0.f;
    elfColor fogColor;

    elfList* scripts = nullptr;
    elfList* textures = nullptr;
    elfList* materials = nullptr;
    elfList* models = nullptr;
    elfList* cameras = nullptr;
    elfList* entities = nullptr;
    elfList* lights = nullptr;
    elfList* armatures = nullptr;
    elfList* particles = nullptr;
    elfList* sprites = nullptr;

    elfList* entityQueue = nullptr;
    // TODO Why not use queue size?
    int entityQueueCount = 0;

    elfList* spriteQueue = nullptr;
    // TODO Why not use queue size?
    int spriteQueueCount = 0;

    // TODO Why is the physics world part of the scene?
    elfPhysicsWorld* world = nullptr;
    // Debug world?
    elfPhysicsWorld* dworld = nullptr;

    elfCamera* curCamera = nullptr;

    gfxShaderParams shaderParams;

    gfxShaderProgram* composeFogShdr = nullptr;

    elfPak* pak = nullptr;
};

elfScene* elfCreateScene(const char* name);

// Load from assimp
void elfRecursivelyImportAssets(elfScene* scene, const aiScene* aiscn, aiNode* aind);

// Save / load
elfScene* elfCreateSceneFromFile(const char* name, const char* filePath);
unsigned char elfSaveScene(elfScene* scene, const char* filePath);

void elfUpdateScene(elfScene* scene, float sync);

void elfScenePreDraw(elfScene* scene);
void elfScenePostDraw(elfScene* scene);

// Destructor
void elfDestroyScene(void* data);

const char* elfGetSceneName(elfScene* scene);

const char* elfGetSceneFilePath(elfScene* scene);

// Has physics
void elfSetScenePhysics(elfScene* scene, unsigned char physics);
bool elfGetScenePhysics(elfScene* scene);

// Run entity scripts
void elfSetSceneRunScripts(elfScene* scene, bool runScripts);
bool elfGetSceneRunScripts(elfScene* scene);

// Draw debug info
void elfSetSceneDebugDraw(elfScene* scene, bool debugDraw);
bool elfGetSceneDebugDraw(elfScene* scene);

// Use culling
void elfSetSceneOcclusionCulling(elfScene* scene, bool occlusionCulling);
bool elfGetSceneOcclusionCulling(elfScene* scene);

void elfSetSceneGravity(elfScene* scene, float x, float y, float z);
elfVec3f elfGetSceneGravity(elfScene* scene);

void elfSetSceneAmbientColor(elfScene* scene, float r, float g, float b, float a);
elfColor elfGetSceneAmbientColor(elfScene* scene);

// Fog
void elfSetSceneFog(elfScene* scene, bool fog);

void elfSetSceneFogStart(elfScene* scene, float start);
void elfSetSceneFogEnd(elfScene* scene, float end);

void elfSetSceneFogColor(elfScene* scene, float r, float g, float b, float a);

bool elfGetSceneFog(elfScene* scene);

float elfGetSceneFogStart(elfScene* scene);
float elfGetFogEnd(elfScene* scene);

elfColor elfGetFogColor(elfScene* scene);

// Counts
int elfGetSceneCameraCount(elfScene* scene);
int elfGetSceneEntityCount(elfScene* scene);
int elfGetSceneLightCount(elfScene* scene);
int elfGetSceneArmatureCount(elfScene* scene);
int elfGetSceneParticlesCount(elfScene* scene);
int elfGetSceneSpriteCount(elfScene* scene);

// Sets main actor
void elfSetActorScene(elfScene* scene, elfActor* actor);

// Add scene objects
void elfAddSceneCamera(elfScene* scene, elfCamera* camera);
void elfAddSceneEntity(elfScene* scene, elfEntity* entity);
void elfAddSceneLight(elfScene* scene, elfLight* light);
void elfAddSceneParticles(elfScene* scene, elfParticles* particles);
void elfAddSceneSprite(elfScene* scene, elfSprite* sprite);

void elfSetSceneActiveCamera(elfScene* scene, elfCamera* camera);
elfCamera* elfGetSceneActiveCamera(elfScene* scene);

elfCollision* elfGetSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);
elfList* elfGetSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);

elfCollision* elfGetDebugSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);
elfList* elfGetDebugSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);

// Get by index
elfCamera* elfGetSceneCameraByIndex(elfScene* scene, int idx);
elfEntity* elfGetSceneEntityByIndex(elfScene* scene, int idx);
elfLight* elfGetSceneLightByIndex(elfScene* scene, int idx);
elfArmature* elfGetSceneArmatureByIndex(elfScene* scene, int idx);
elfParticles* elfGetSceneParticlesByIndex(elfScene* scene, int idx);
elfSprite* elfGetSceneSpriteByIndex(elfScene* scene, int idx);

// Get by name
elfTexture* elfGetSceneTexture(elfScene* scene, const char* name);
elfModel* elfGetSceneModel(elfScene* scene, const char* name);
elfScript* elfGetSceneScript(elfScene* scene, const char* name);
elfMaterial* elfGetSceneMaterial(elfScene* scene, const char* name);
elfActor* elfGetSceneActor(elfScene* scene, const char* name);
elfCamera* elfGetSceneCamera(elfScene* scene, const char* name);
elfEntity* elfGetSceneEntity(elfScene* scene, const char* name);
elfLight* elfGetSceneLight(elfScene* scene, const char* name);
elfArmature* elfGetSceneArmature(elfScene* scene, const char* name);
elfParticles* elfGetSceneParticles(elfScene* scene, const char* name);
elfSprite* elfGetSceneSprite(elfScene* scene, const char* name);

// Get or load from file
elfTexture* elfGetOrLoadTextureByName(elfScene* scene, const char* name);
elfModel* elfGetOrLoadModelByName(elfScene* scene, const char* name);
elfScript* elfGetOrLoadScriptByName(elfScene* scene, const char* name);
elfMaterial* elfGetOrLoadMaterialByName(elfScene* scene, const char* name);
elfCamera* elfGetOrLoadCameraByName(elfScene* scene, const char* name);
elfEntity* elfGetOrLoadEntityByName(elfScene* scene, const char* name);
elfLight* elfGetOrLoadLightByName(elfScene* scene, const char* name);
elfArmature* elfGetOrLoadArmatureByName(elfScene* scene, const char* name);
elfParticles* elfGetOrLoadParticlesByName(elfScene* scene, const char* name);
elfSprite* elfGetOrLoadSpriteByName(elfScene* scene, const char* name);
elfActor* elfGetOrLoadActorByName(elfScene* scene, const char* name);

// TODO Why no return value?
void elfRemoveActor(elfActor* actor);
bool elfRemoveSceneCamera(elfScene* scene, const char* name);
bool elfRemoveSceneEntity(elfScene* scene, const char* name);
bool elfRemoveSceneLight(elfScene* scene, const char* name);
bool elfRemoveSceneParticles(elfScene* scene, const char* name);
bool elfRemoveSceneSprite(elfScene* scene, const char* name);
bool elfRemoveSceneCameraByIndex(elfScene* scene, int idx);
bool elfRemoveSceneEntityByIndex(elfScene* scene, int idx);
bool elfRemoveSceneLightByIndex(elfScene* scene, int idx);
bool elfRemoveSceneParticlesByIndex(elfScene* scene, int idx);
bool elfRemoveSceneSpriteByIndex(elfScene* scene, int idx);
bool elfRemoveSceneCameraByObject(elfScene* scene, elfCamera* camera);
bool elfRemoveSceneEntityByObject(elfScene* scene, elfEntity* entity);
bool elfRemoveSceneLightByObject(elfScene* scene, elfLight* light);
bool elfRemoveSceneParticlesByObject(elfScene* scene, elfParticles* particles);
bool elfRemoveSceneSpriteByObject(elfScene* scene, elfSprite* sprite);
bool elfRemoveSceneActorByObject(elfScene* scene, elfActor* actor);

// TODO Was commented out, investigate later
void elfDrawSceneDeferred(elfScene* scene);

// TODO Draw method should NOT be in the scene
//      Scene should only manage objects and provide culling and updates
//      Refactor this asap
void elfDrawScene(elfScene* scene);

void elfDrawSceneDebug(elfScene* scene);

elfList* elfGetSceneScripts(elfScene* scene);

elfList* elfGetSceneTextures(elfScene* scene);

elfList* elfGetSceneMaterials(elfScene* scene);

elfList* elfGetSceneModels(elfScene* scene);