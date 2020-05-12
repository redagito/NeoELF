#pragma once

#include "nelf/Color.h"
#include "nelf/Vector.h"
#include "nelf/actor/actorHeader.h"

struct elfTexture;
struct elfModel;
struct elfEntity;
struct elfParticle;
struct elfCamera;

struct gfxVertexArray;
struct gfxVertexData;
struct gfxShaderParams;

// TODO Should be subtype of Actor
// Represents a collection of single particles
struct elfParticles
{
    ELF_ACTOR_HEADER;

    // TODO Use unsigned 64bit (or just capacity of array if particle container is changed)
    int maxCount = 0;
    // TODO Change type to ParticleDrawMode
    unsigned char drawMode = 0;

    // TODO Probably REALLY slow to iterate, use dynamic array instead
    elfList* particles = nullptr;

    elfTexture* texture = nullptr;
    elfModel* model = nullptr;
    elfEntity* entity = nullptr;
    gfxVertexArray* vertexArray = nullptr;
    gfxVertexData* vertices = nullptr;
    gfxVertexData* texCoords = nullptr;
    gfxVertexData* colors = nullptr;

    // TODO Use unsigned 64bit
    int spawnCount = 0;
    float spawnDelay = 0.f;
    bool spawn = false;
    float curTime = 0.f;
    elfVec3f gravity;

    float sizeMin = 0.f;
    float sizeMax = 0.f;
    float sizeGrowthMin = 0.f;
    float sizeGrowthMax = 0.f;
    float rotationMin = 0.f;
    float rotationMax = 0.f;
    float rotationGrowthMin = 0.f;
    float rotationGrowthMax = 0.f;
    float lifeSpanMin = 0.f;
    float lifeSpanMax = 0.f;
    float fadeSpeedMin = 0.f;
    float fadeSpeedMax = 0.f;

    elfVec3f velocityMin;
    elfVec3f velocityMax;
    elfVec3f positionMin;
    elfVec3f positionMax;
    elfColor colorMin;
    elfColor colorMax;

    // AABB
    // TODO Maybe just create an Aabb struct?
    elfVec3f cullAabbMin;
    elfVec3f cullAabbMax;
};

elfParticles* elfCreateParticles(const char* name, int maxCount);

void elfInitNewParticle(elfParticles* particles, elfParticle* particle);

void elfCalcParticlesAabb(elfParticles* particles);

void elfParticlesPreDraw(elfParticles* particles);
void elfParticlesPostDraw(elfParticles* particles);

void elfUpdateParticles(elfParticles* particles, float sync);

void elfDrawParticles(elfParticles* particles, elfCamera* camera, gfxShaderParams* shaderParams);
void elfDrawParticlesDebug(elfParticles* particles, gfxShaderParams* shaderParams);

// Destructor
void elfDestroyParticles(void* data);

const char* elfGetParticlesName(elfParticles* particles);
const char* elfGetParticlesFilePath(elfParticles* particles);

// Setter

void elfSetParticlesMaxCount(elfParticles* particles, int maxCount);
void elfSetParticlesDrawMode(elfParticles* particles, int mode);

// Manage texture, model, entity

void elfSetParticlesTexture(elfParticles* particles, elfTexture* texture);
void elfClearParticlesTexture(elfParticles* particles);

void elfSetParticlesModel(elfParticles* particles, elfModel* model);
void elfClearParticlesModel(elfParticles* particles);

void elfSetParticlesEntity(elfParticles* particles, elfEntity* entity);
void elfClearParticlesEntity(elfParticles* particles);

// Setter

void elfSetParticlesGravity(elfParticles* particles, float x, float y, float z);
void elfSetParticlesSpawnCount(elfParticles* particles, int perSecond);
void elfSetParticlesSpawn(elfParticles* particles, bool spawn);
void elfSetParticlesSize(elfParticles* particles, float min, float max);
void elfSetParticlesSizeGrowth(elfParticles* particles, float min, float max);
void elfSetParticlesRotation(elfParticles* particles, float min, float max);
void elfSetParticlesRotationGrowth(elfParticles* particles, float min, float max);
void elfSetParticlesLifeSpan(elfParticles* particles, float min, float max);
void elfSetParticlesFadeSpeed(elfParticles* particles, float min, float max);
void elfSetParticlesVelocityMin(elfParticles* particles, float x, float y, float z);
void elfSetParticlesPositionMin(elfParticles* particles, float x, float y, float z);
void elfSetParticlesPositionMax(elfParticles* particles, float x, float y, float z);
void elfSetParticlesVelocityMax(elfParticles* particles, float x, float y, float z);
void elfSetParticlesColorMin(elfParticles* particles, float r, float g, float b, float a);
void elfSetParticlesColorMax(elfParticles* particles, float r, float g, float b, float a);

// Getter

int elfGetParticlesMaxCount(elfParticles* particles);
int elfGetParticlesCount(elfParticles* particles);
int elfGetParticlesDrawMode(elfParticles* particles);

elfTexture* elfGetParticlesTexture(elfParticles* particles);
elfModel* elfGetParticlesModel(elfParticles* particles);
elfEntity* elfGetParticlesEntity(elfParticles* particles);
elfVec3f elfGetParticlesGravity(elfParticles* particles);

int elfGetParticlesSpawnCount(elfParticles* particles);
bool elfGetParticlesSpawn(elfParticles* particles);

float elfGetParticlesSizeMin(elfParticles* particles);
float elfGetParticlesSizeMax(elfParticles* particles);
float elfGetParticlesRotationMin(elfParticles* particles);
float elfGetParticlesRotationMax(elfParticles* particles);
float elfGetParticlesRotationGrowthMin(elfParticles* particles);
float elfGetParticlesRotationGrowthMax(elfParticles* particles);
float elfGetParticlesSizeGrowthMin(elfParticles* particles);
float elfGetParticlesSizeGrowthMax(elfParticles* particles);
float elfGetParticlesLifeSpanMin(elfParticles* particles);
float elfGetParticlesLifeSpanMax(elfParticles* particles);
float elfGetParticlesFadeSpeedMin(elfParticles* particles);
float elfGetParticlesFadeSpeedMax(elfParticles* particles);

elfVec3f elfGetParticlesPositionMin(elfParticles* particles);
elfVec3f elfGetParticlesPositionMax(elfParticles* particles);
elfVec3f elfGetParticlesVelocityMin(elfParticles* particles);
elfVec3f elfGetParticlesVelocityMax(elfParticles* particles);
elfColor elfGetParticlesColorMin(elfParticles* particles);
elfColor elfGetParticlesColorMax(elfParticles* particles);

bool elfCullParticles(elfParticles* particles, elfCamera* camera);