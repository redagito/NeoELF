#pragma once

#include "nelf/Vector.h"
#include "nelf/actorHeader.h"

struct elfModel;
struct elfArmature;
struct elfMaterial;
struct elfCamera;
struct gfxVertexData;
struct gfxQuery;
struct gfxShaderParams;

struct elfEntity
{
    ELF_ACTOR_HEADER;

    elfModel* model = nullptr;

    elfArmature* armature = nullptr;
    gfxVertexData* vertices = nullptr;
    gfxVertexData* normals = nullptr;

    elfList* materials = nullptr;
    elfFramePlayer* armaturePlayer = nullptr;
    float prevArmatureFrame = 0.f;

    elfVec3f position;
    elfVec3f scale;
    elfVec3f bbMin;
    elfVec3f bbMax;
    elfVec3f bbOffset;
    elfVec3f armBbMin;
    elfVec3f armBbMax;
    elfVec3f cullAabbMin;
    elfVec3f cullAabbMax;
    float cullRadius = 0.f;

    gfxQuery* query = nullptr;
    bool visible;
    unsigned char occluder;
    bool culled;
};

elfEntity* elfCreateEntity(const char* name);

void elfGenerateEntityTangents(elfEntity* entity);

void elfUpdateEntity(elfEntity* entity);

void elfEntityPreDraw(elfEntity* entity);

void elfEntityPostDraw(elfEntity* entity);

void elfDestroyEntity(void* data);

void elfEvalEntityAabbCorner(elfEntity* entity, elfVec4f* orient, elfVec3f* corner, elfVec3f* result);

void elfEvalEntityAabbArmatureCorner(elfEntity* entity, elfVec4f* orient, elfVec3f* corner, elfVec3f* result);

void elfCalcEntityAabb(elfEntity* entity);

void elfCalcEntityBoundingVolumes(elfEntity* entity, unsigned char newModel);

void elfSetEntityScale(elfEntity* entity, float x, float y, float z);

elfVec3f elfGetEntityScale(elfEntity* entity);

void elfGetEntityScale_(elfEntity* entity, float* scale);

void elfSetEntityModel(elfEntity* entity, elfModel* model);

void elfClearEntityModel(elfEntity* entity);

elfModel* elfGetEntityModel(elfEntity* entity);

int elfGetEntityMaterialCount(elfEntity* entity);

void elfAddEntityMaterial(elfEntity* entity, elfMaterial* material);

void elfSetEntityMaterial(elfEntity* entity, int idx, elfMaterial* material);

void elfRemoveEntityMaterial(elfEntity* entity, int idx);

elfMaterial* elfGetEntityMaterial(elfEntity* entity, int idx);

void elfSetEntityVisible(elfEntity* entity, unsigned char visible);

unsigned char elfGetEntityVisible(elfEntity* entity);

void elfSetEntityOccluder(elfEntity* entity, unsigned char occluder);

unsigned char elfGetEntityOccluder(elfEntity* entity);

void elfResetEntityDebugPhysicsObject(elfEntity* entity);

void elfSetEntityArmature(elfEntity* entity, elfArmature* armature);

void elfSetEntityArmatureFrame(elfEntity* entity, float frame);

void elfPlayEntityArmature(elfEntity* entity, float start, float end, float speed);

void elfLoopEntityArmature(elfEntity* entity, float start, float end, float speed);

void elfStopEntityArmature(elfEntity* entity);

void elfPauseEntityArmature(elfEntity* entity);

void elfResumeEntityArmature(elfEntity* entity);

float elfGetEntityArmatureStart(elfEntity* entity);

float elfGetEntityArmatureEnd(elfEntity* entity);

float elfGetEntityArmatureSpeed(elfEntity* entity);

float elfGetEntityArmatureFrame(elfEntity* entity);

unsigned char elfIsEntityArmaturePlaying(elfEntity* entity);

unsigned char elfIsEntityArmaturePaused(elfEntity* entity);

elfArmature* elfGetEntityArmature(elfEntity* entity);

void elfPreDrawEntity(elfEntity* entity);

void elfPostDrawEntity(elfEntity* entity);

void elfDrawEntity(elfEntity* entity, int mode, gfxShaderParams* shaderParams);

void elfDrawEntityBoundingBox(elfEntity* entity, gfxShaderParams* shaderParams);

void elfDrawEntityDebug(elfEntity* entity, gfxShaderParams* shaderParams);

unsigned char elfCullEntity(elfEntity* entity, elfCamera* camera);

unsigned char elfGetEntityChanged(elfEntity* entity);
