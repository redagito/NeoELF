#include "nelf/actor/Entity.h"

#include <cstdlib>
#include <cstring>

#include "nelf/FramePlayer.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/gfx/gfxAttributeType.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxQuery.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/gfx/gfxVertexArray.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/objectType.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/resource/Armature.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Scene.h"

elfEntity* elfCreateEntity(const char* name)
{
    elfEntity* entity;

    entity = (elfEntity*)malloc(sizeof(elfEntity));
    memset(entity, 0x0, sizeof(elfEntity));
    entity->objType = ELF_ENTITY;
    entity->objDestr = elfDestroyEntity;

    elfInitActor((elfActor*)entity, false);

    entity->scale.x = entity->scale.y = entity->scale.z = 1.0f;
    if (gfxGetVersion() >= 150)
        entity->query = gfxCreateQuery();
    entity->visible = true;

    entity->materials = elfCreateList();
    elfIncRef((elfObject*)entity->materials);

    entity->culled = true;

    entity->dobject = elfCreatePhysicsObjectBox(0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.f);
    elfSetPhysicsObjectActor(entity->dobject, (elfActor*)entity);
    elfIncRef((elfObject*)entity->dobject);

    entity->pbbLengths.x = entity->pbbLengths.y = entity->pbbLengths.z = 0.4f;

    entity->armaturePlayer = elfCreateFramePlayer();
    elfIncRef((elfObject*)entity->armaturePlayer);

    if (name)
        entity->name = elfCreateString(name);

    entity->id = ++res->entityIdCounter;

    elfIncObj(ELF_ENTITY);

    return entity;
}

void elfGenerateEntityTangents(elfEntity* entity)
{
    elfMaterial* material;

    if (!entity->model || elfGetModelTangents(entity->model))
        return;

    for (material = (elfMaterial*)elfBeginList(entity->materials); material;
         material = (elfMaterial*)elfGetListNext(entity->materials))
    {
        if (elfGetMaterialNormalMap(material) || elfGetMaterialHeightMap(material))
        {
            elfGenerateModelTangents(entity->model);
            return;
        }
    }
}

void elfUpdateEntity(elfEntity* entity)
{
    elfUpdateActor((elfActor*)entity);
    elfUpdateFramePlayer(entity->armaturePlayer);
}

void elfEntityPreDraw(elfEntity* entity)
{
    elfActorPreDraw((elfActor*)entity);

    gfxGetTransformPosition(entity->transform, &entity->position.x);

    if (entity->armature &&
        fabs(elfGetFramePlayerFrame(entity->armaturePlayer) - entity->prevArmatureFrame) > 0.0001f &&
        elfGetFramePlayerFrame(entity->armaturePlayer) <= entity->armature->frameCount)
    {
        elfDeformEntityWithArmature(entity->armature, entity, elfGetFramePlayerFrame(entity->armaturePlayer));
        entity->prevArmatureFrame = elfGetFramePlayerFrame(entity->armaturePlayer);
    }

    if (entity->moved)
    {
        elfCalcEntityAabb(entity);
    }
}

void elfEntityPostDraw(elfEntity* entity) { elfActorPostDraw((elfActor*)entity); }

void elfDestroyEntity(void* data)
{
    elfEntity* entity = (elfEntity*)data;

    elfCleanActor((elfActor*)entity);

    if (entity->model)
        elfDecRef((elfObject*)entity->model);
    if (entity->armature)
        elfDecRef((elfObject*)entity->armature);
    if (entity->vertices)
        gfxDecRef((gfxObject*)entity->vertices);
    if (entity->normals)
        gfxDecRef((gfxObject*)entity->normals);
    if (gfxGetVersion() >= 150)
    {
        if (entity->query)
            gfxDestroyQuery(entity->query);
    }

    elfDecRef((elfObject*)entity->materials);
    elfDecRef((elfObject*)entity->armaturePlayer);

    free(entity);

    elfDecObj(ELF_ENTITY);
}

void elfEvalEntityAabbCorner(elfEntity* entity, elfVec4f* orient, elfVec3f* corner, elfVec3f* result)
{
    corner->x -= entity->bbOffset.x;
    corner->y -= entity->bbOffset.y;
    corner->z -= entity->bbOffset.z;
    gfxMulQuaVec(&orient->x, &corner->x, &result->x);

    if (result->x < entity->cullAabbMin.x)
        entity->cullAabbMin.x = result->x;
    if (result->y < entity->cullAabbMin.y)
        entity->cullAabbMin.y = result->y;
    if (result->z < entity->cullAabbMin.z)
        entity->cullAabbMin.z = result->z;
    if (result->x > entity->cullAabbMax.x)
        entity->cullAabbMax.x = result->x;
    if (result->y > entity->cullAabbMax.y)
        entity->cullAabbMax.y = result->y;
    if (result->z > entity->cullAabbMax.z)
        entity->cullAabbMax.z = result->z;
}

void elfEvalEntityAabbArmatureCorner(elfEntity* entity, elfVec4f* orient, elfVec3f* corner, elfVec3f* result)
{
    gfxMulQuaVec(&orient->x, &corner->x, &result->x);

    if (result->x < entity->cullAabbMin.x)
        entity->cullAabbMin.x = result->x;
    if (result->y < entity->cullAabbMin.y)
        entity->cullAabbMin.y = result->y;
    if (result->z < entity->cullAabbMin.z)
        entity->cullAabbMin.z = result->z;
    if (result->x > entity->cullAabbMax.x)
        entity->cullAabbMax.x = result->x;
    if (result->y > entity->cullAabbMax.y)
        entity->cullAabbMax.y = result->y;
    if (result->z > entity->cullAabbMax.z)
        entity->cullAabbMax.z = result->z;
}

void elfCalcEntityAabb(elfEntity* entity)
{
    elfVec3f tmpVec;

    elfVec3f position;
    elfVec4f orient;
    elfVec3f corner;
    elfVec3f result;

    gfxGetTransformPosition(entity->transform, &position.x);
    gfxGetTransformOrientation(entity->transform, &orient.x);

    corner = entity->bbMin;
    corner.x -= entity->bbOffset.x;
    corner.y -= entity->bbOffset.y;
    corner.z -= entity->bbOffset.z;
    gfxMulQuaVec(&orient.x, &corner.x, &result.x);

    entity->cullAabbMin = corner;
    entity->cullAabbMax = corner;

    corner.x = entity->bbMin.x;
    corner.y = entity->bbMax.y;
    corner.z = entity->bbMin.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMin.x;
    corner.y = entity->bbMax.y;
    corner.z = entity->bbMax.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMin.x;
    corner.y = entity->bbMin.y;
    corner.z = entity->bbMax.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMax.x;
    corner.y = entity->bbMin.y;
    corner.z = entity->bbMin.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMax.x;
    corner.y = entity->bbMax.y;
    corner.z = entity->bbMin.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMax.x;
    corner.y = entity->bbMax.y;
    corner.z = entity->bbMax.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    corner.x = entity->bbMax.x;
    corner.y = entity->bbMin.y;
    corner.z = entity->bbMax.z;
    elfEvalEntityAabbCorner(entity, &orient, &corner, &result);

    if (entity->armature)
    {
        corner.x = entity->armBbMin.x;
        corner.y = entity->armBbMin.y;
        corner.z = entity->armBbMin.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMin.x;
        corner.y = entity->armBbMax.y;
        corner.z = entity->armBbMin.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMin.x;
        corner.y = entity->armBbMax.y;
        corner.z = entity->armBbMax.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMin.x;
        corner.y = entity->armBbMin.y;
        corner.z = entity->armBbMax.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMax.x;
        corner.y = entity->armBbMin.y;
        corner.z = entity->armBbMin.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMax.x;
        corner.y = entity->armBbMax.y;
        corner.z = entity->armBbMin.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMax.x;
        corner.y = entity->armBbMax.y;
        corner.z = entity->armBbMax.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);

        corner.x = entity->armBbMax.x;
        corner.y = entity->armBbMin.y;
        corner.z = entity->armBbMax.z;
        elfEvalEntityAabbArmatureCorner(entity, &orient, &corner, &result);
    }

    entity->cullAabbMin.x += position.x;
    entity->cullAabbMin.y += position.y;
    entity->cullAabbMin.z += position.z;
    entity->cullAabbMax.x += position.x;
    entity->cullAabbMax.y += position.y;
    entity->cullAabbMax.z += position.z;

    tmpVec.x = entity->bbMax.x - entity->bbMin.x;
    tmpVec.y = entity->bbMax.y - entity->bbMin.y;
    tmpVec.z = entity->bbMax.z - entity->bbMin.z;
    entity->cullRadius = gfxVecLength(&tmpVec.x) / 2;
}

void elfCalcEntityBoundingVolumes(elfEntity* entity, bool newModel)
{
    if (!entity->model)
    {
        entity->bbMin.x = entity->bbMin.y = entity->bbMin.z = -0.2f;
        entity->bbMax.x = entity->bbMax.y = entity->bbMax.z = 0.2f;
        entity->bbOffset.x = entity->bbOffset.y = entity->bbOffset.z = 0.0f;
        elfCalcEntityAabb(entity);
        entity->cullRadius = 0.2f;
        return;
    }

    entity->bbMin = entity->model->bbMin;
    entity->bbMax = entity->model->bbMax;

    entity->bbMin.x *= entity->scale.x;
    entity->bbMin.y *= entity->scale.y;
    entity->bbMin.z *= entity->scale.z;
    entity->bbMax.x *= entity->scale.x;
    entity->bbMax.y *= entity->scale.y;
    entity->bbMax.z *= entity->scale.z;

    if (entity->armature)
    {
        entity->armBbMin = entity->armature->bbMin;
        entity->armBbMax = entity->armature->bbMax;

        entity->armBbMin.x *= entity->scale.x;
        entity->armBbMin.y *= entity->scale.y;
        entity->armBbMin.z *= entity->scale.z;
        entity->armBbMax.x *= entity->scale.x;
        entity->armBbMax.y *= entity->scale.y;
        entity->armBbMax.z *= entity->scale.z;
    }

    entity->bbOffset.x = (entity->bbMax.x + entity->bbMin.x) / 2.0f;
    entity->bbOffset.y = (entity->bbMax.y + entity->bbMin.y) / 2.0f;
    entity->bbOffset.z = (entity->bbMax.z + entity->bbMin.z) / 2.0f;

    if (!entity->pbbOffsetSet)
        entity->pbbOffset = entity->bbOffset;

    entity->bbMin.x += entity->bbOffset.x;
    entity->bbMin.y += entity->bbOffset.y;
    entity->bbMin.z += entity->bbOffset.z;
    entity->bbMax.x += entity->bbOffset.x;
    entity->bbMax.y += entity->bbOffset.y;
    entity->bbMax.z += entity->bbOffset.z;

    if (newModel)
    {
        entity->pbbLengths.x = entity->model->bbMax.x - entity->model->bbMin.x;
        entity->pbbLengths.y = entity->model->bbMax.y - entity->model->bbMin.y;
        entity->pbbLengths.z = entity->model->bbMax.z - entity->model->bbMin.z;
    }

    elfCalcEntityAabb(entity);
}

void elfSetEntityScale(elfEntity* entity, float x, float y, float z)
{
    gfxSetTransformScale(entity->transform, x, y, z);
    gfxGetTransformScale(entity->transform, &entity->scale.x);

    elfCalcEntityBoundingVolumes(entity, false);

    if (entity->object)
        elfSetPhysicsObjectScale(entity->object, x, y, z);
    elfResetEntityDebugPhysicsObject(entity);
}

elfVec3f elfGetEntityScale(elfEntity* entity)
{
    elfVec3f result;

    gfxGetTransformScale(entity->transform, &result.x);

    return result;
}

void elfGetEntityScale_(elfEntity* entity, float* scale) { gfxGetTransformScale(entity->transform, scale); }

void elfSetEntityModel(elfEntity* entity, elfModel* model)
{
    elfMaterial* material;

    if (entity->model)
        elfDecRef((elfObject*)entity->model);
    if (entity->vertices)
        gfxDecRef((gfxObject*)entity->vertices);
    if (entity->normals)
        gfxDecRef((gfxObject*)entity->normals);

    entity->model = model;
    entity->vertices = nullptr;
    entity->normals = nullptr;

    if (!entity->model)
    {
        if (entity->object)
            elfSetActorPhysics((elfActor*)entity, false);
        elfResetEntityDebugPhysicsObject(entity);
        elfCalcEntityBoundingVolumes(entity, false);
        return;
    }
    else
    {
        elfIncRef((elfObject*)entity->model);
    }

    while ((int)entity->model->areaCount > elfGetEntityMaterialCount(entity))
    {
        material = elfCreateMaterial("Material");
        elfAddEntityMaterial(entity, material);
    }

    elfSetEntityScale(entity, 1.0f, 1.0f, 1.0f);
    elfCalcEntityBoundingVolumes(entity, true);

    if (elfGetActorPhysics((elfActor*)entity))
        elfSetActorPhysics((elfActor*)entity, true);

    elfResetEntityDebugPhysicsObject(entity);

    entity->moved = true;
}

void elfClearEntityModel(elfEntity* entity)
{
    if (entity->model)
        elfDecRef((elfObject*)entity->model);
    entity->model = nullptr;

    if (entity->object)
    {
        elfDecRef((elfObject*)entity->object);
        entity->object = nullptr;
    }

    elfSetEntityScale(entity, 1.0f, 1.0f, 1.0f);
    elfCalcEntityBoundingVolumes(entity, false);
}

elfModel* elfGetEntityModel(elfEntity* entity) { return entity->model; }

int elfGetEntityMaterialCount(elfEntity* entity) { return elfGetListLength(entity->materials); }

void elfAddEntityMaterial(elfEntity* entity, elfMaterial* material)
{
    elfAppendListObject(entity->materials, (elfObject*)material);
    elfGenerateEntityTangents(entity);
}

void elfSetEntityMaterial(elfEntity* entity, int idx, elfMaterial* material)
{
    elfObject* mat;
    int i;

    if (idx < 0 || idx > elfGetListLength(entity->materials) - 1)
        return;

    for (i = 0, mat = elfBeginList(entity->materials); mat; mat = elfGetListNext(entity->materials), i++)
    {
        if (idx == i)
        {
            elfSetListCurPtr(entity->materials, (elfObject*)material);
            return;
        }
    }

    elfGenerateEntityTangents(entity);
}

void elfRemoveEntityMaterial(elfEntity* entity, int idx)
{
    elfObject* mat;
    int i;

    if (idx < 0 || idx > elfGetListLength(entity->materials) - 1)
        return;

    for (i = 0, mat = elfBeginList(entity->materials); mat; mat = elfGetListNext(entity->materials), i++)
    {
        if (idx == i)
        {
            elfRemoveListObject(entity->materials, (elfObject*)mat);
            return;
        }
    }
}

elfMaterial* elfGetEntityMaterial(elfEntity* entity, int idx)
{
    if (idx < 0 || idx > elfGetListLength(entity->materials) - 1)
        return NULL;
    return (elfMaterial*)elfGetListObject(entity->materials, idx);
}

void elfSetEntityVisible(elfEntity* entity, bool visible)
{
    if (entity->visible == visible)
        return;

    entity->visible = visible;

    if (!entity->visible)
        entity->moved = true;
}

bool elfGetEntityVisible(elfEntity* entity) { return entity->visible; }

void elfSetEntityOccluder(elfEntity* entity, bool occluder) { entity->occluder = occluder; }

bool elfGetEntityOccluder(elfEntity* entity) { return entity->occluder; }

void elfResetEntityDebugPhysicsObject(elfEntity* entity)
{
    float position[3];
    float orient[4];
    float scale[3];

    if (entity->dobject)
    {
        elfSetPhysicsObjectActor(entity->dobject, NULL);
        elfSetPhysicsObjectWorld(entity->dobject, NULL);
        elfDecRef((elfObject*)entity->dobject);
    }

    if (!entity->model)
    {
        entity->dobject = elfCreatePhysicsObjectBox(0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        entity->dobject = elfCreatePhysicsObjectBox((entity->model->bbMax.x - entity->model->bbMin.x) / 2.0f,
                                                    (entity->model->bbMax.y - entity->model->bbMin.y) / 2.0f,
                                                    (entity->model->bbMax.z - entity->model->bbMin.z) / 2.0f, 0.0f,
                                                    entity->bbOffset.x, entity->bbOffset.y, entity->bbOffset.z);
    }

    elfSetPhysicsObjectActor(entity->dobject, (elfActor*)entity);
    elfIncRef((elfObject*)entity->dobject);

    gfxGetTransformPosition(entity->transform, position);
    gfxGetTransformOrientation(entity->transform, orient);
    gfxGetTransformScale(entity->transform, scale);

    elfSetPhysicsObjectPosition(entity->dobject, position[0], position[1], position[2]);
    elfSetPhysicsObjectOrientation(entity->dobject, orient[0], orient[1], orient[2], orient[3]);
    elfSetPhysicsObjectScale(entity->dobject, scale[0], scale[1], scale[2]);

    if (entity->scene)
        elfSetPhysicsObjectWorld(entity->dobject, entity->scene->dworld);
}

void elfSetEntityArmature(elfEntity* entity, elfArmature* armature)
{
    if (entity->armature)
        elfDecRef((elfObject*)entity->armature);
    entity->armature = armature;
    if (entity->armature)
        elfIncRef((elfObject*)entity->armature);
    elfCalcEntityBoundingVolumes(entity, false);
}

void elfSetEntityArmatureFrame(elfEntity* entity, float frame)
{
    elfSetFramePlayerFrame(entity->armaturePlayer, frame);
}

void elfPlayEntityArmature(elfEntity* entity, float start, float end, float speed)
{
    elfPlayFramePlayer(entity->armaturePlayer, start, end, speed);
    if (entity->armature)
        elfDeformEntityWithArmature(entity->armature, entity, start);
}

void elfLoopEntityArmature(elfEntity* entity, float start, float end, float speed)
{
    elfLoopFramePlayer(entity->armaturePlayer, start, end, speed);
    if (entity->armature)
        elfDeformEntityWithArmature(entity->armature, entity, start);
}

void elfStopEntityArmature(elfEntity* entity) { elfStopFramePlayer(entity->armaturePlayer); }

void elfPauseEntityArmature(elfEntity* entity) { elfStopFramePlayer(entity->armaturePlayer); }

void elfResumeEntityArmature(elfEntity* entity) { elfStopFramePlayer(entity->armaturePlayer); }

float elfGetEntityArmatureStart(elfEntity* entity) { return elfGetFramePlayerStart(entity->armaturePlayer); }

float elfGetEntityArmatureEnd(elfEntity* entity) { return elfGetFramePlayerEnd(entity->armaturePlayer); }

float elfGetEntityArmatureSpeed(elfEntity* entity) { return elfGetFramePlayerSpeed(entity->armaturePlayer); }

float elfGetEntityArmatureFrame(elfEntity* entity) { return elfGetFramePlayerFrame(entity->armaturePlayer); }

unsigned char elfIsEntityArmaturePlaying(elfEntity* entity) { return elfIsFramePlayerPlaying(entity->armaturePlayer); }

unsigned char elfIsEntityArmaturePaused(elfEntity* entity) { return elfIsFramePlayerPaused(entity->armaturePlayer); }

elfArmature* elfGetEntityArmature(elfEntity* entity) { return entity->armature; }

void elfPreDrawEntity(elfEntity* entity)
{
    if (entity->armature)
    {
        if (entity->vertices)
            gfxSetVertexArrayData(entity->model->vertexArray, GFX_VERTEX, entity->vertices);
        if (entity->normals)
            gfxSetVertexArrayData(entity->model->vertexArray, GFX_NORMAL, entity->normals);
    }
}

void elfPostDrawEntity(elfEntity* entity)
{
    if (entity->armature)
    {
        if (entity->vertices)
            gfxSetVertexArrayData(entity->model->vertexArray, GFX_VERTEX, entity->vertices);
        if (entity->normals)
            gfxSetVertexArrayData(entity->model->vertexArray, GFX_NORMAL, entity->model->normals);
    }
}

void elfDrawEntity(elfEntity* entity, int mode, gfxShaderParams* shaderParams)
{
    // No model set
    if (entity->model == nullptr)
        return;
    // Not visible
    if (!entity->visible)
        return;

    // Set ModelView matrix
    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(entity->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);
    // Set normal matrix
    gfxMulMatrix3Matrix4(gfxGetTransformNormalMatrix(entity->transform), shaderParams->cameraMatrix,
                         shaderParams->normalMatrix);

    elfPreDrawEntity(entity);
    elfDrawModel(entity->materials, entity->model, mode, shaderParams);
    elfPostDrawEntity(entity);
}

void elfDrawEntityBoundingBox(elfEntity* entity, gfxShaderParams* shaderParams)
{
    if (!entity->model || !entity->visible || !entity->model->vertexArray)
        return;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(entity->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);

    gfxSetShaderParams(shaderParams);
    elfDrawBoundingBox(&entity->model->bbMin.x, &entity->model->bbMax.x);
}

void elfDrawEntityDebug(elfEntity* entity, gfxShaderParams* shaderParams)
{
    float min[3];
    float max[3];
    float* vertexBuffer;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(entity->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);

    if (entity->model)
    {
        memcpy(min, &entity->model->bbMin.x, sizeof(float) * 3);
        memcpy(max, &entity->model->bbMax.x, sizeof(float) * 3);
    }
    else
    {
        min[0] = min[1] = min[2] = -0.2f;
        max[0] = max[1] = max[2] = 0.2f;
    }

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->lines);

    vertexBuffer[0] = min[0];
    vertexBuffer[1] = max[1];
    vertexBuffer[2] = max[2];
    vertexBuffer[3] = min[0];
    vertexBuffer[4] = max[1];
    vertexBuffer[5] = min[2];

    vertexBuffer[6] = min[0];
    vertexBuffer[7] = max[1];
    vertexBuffer[8] = min[2];
    vertexBuffer[9] = min[0];
    vertexBuffer[10] = min[1];
    vertexBuffer[11] = min[2];

    vertexBuffer[12] = min[0];
    vertexBuffer[13] = min[1];
    vertexBuffer[14] = min[2];
    vertexBuffer[15] = min[0];
    vertexBuffer[16] = min[1];
    vertexBuffer[17] = max[2];

    vertexBuffer[18] = min[0];
    vertexBuffer[19] = min[1];
    vertexBuffer[20] = max[2];
    vertexBuffer[21] = min[0];
    vertexBuffer[22] = max[1];
    vertexBuffer[23] = max[2];

    vertexBuffer[24] = max[0];
    vertexBuffer[25] = max[1];
    vertexBuffer[26] = max[2];
    vertexBuffer[27] = max[0];
    vertexBuffer[28] = max[1];
    vertexBuffer[29] = min[2];

    vertexBuffer[30] = max[0];
    vertexBuffer[31] = max[1];
    vertexBuffer[32] = min[2];
    vertexBuffer[33] = max[0];
    vertexBuffer[34] = min[1];
    vertexBuffer[35] = min[2];

    vertexBuffer[36] = max[0];
    vertexBuffer[37] = min[1];
    vertexBuffer[38] = min[2];
    vertexBuffer[39] = max[0];
    vertexBuffer[40] = min[1];
    vertexBuffer[41] = max[2];

    vertexBuffer[42] = max[0];
    vertexBuffer[43] = min[1];
    vertexBuffer[44] = max[2];
    vertexBuffer[45] = max[0];
    vertexBuffer[46] = max[1];
    vertexBuffer[47] = max[2];

    vertexBuffer[48] = min[0];
    vertexBuffer[49] = max[1];
    vertexBuffer[50] = max[2];
    vertexBuffer[51] = max[0];
    vertexBuffer[52] = max[1];
    vertexBuffer[53] = max[2];

    vertexBuffer[54] = min[0];
    vertexBuffer[55] = min[1];
    vertexBuffer[56] = max[2];
    vertexBuffer[57] = max[0];
    vertexBuffer[58] = min[1];
    vertexBuffer[59] = max[2];

    vertexBuffer[60] = min[0];
    vertexBuffer[61] = min[1];
    vertexBuffer[62] = min[2];
    vertexBuffer[63] = max[0];
    vertexBuffer[64] = min[1];
    vertexBuffer[65] = min[2];

    vertexBuffer[66] = min[0];
    vertexBuffer[67] = max[1];
    vertexBuffer[68] = min[2];
    vertexBuffer[69] = max[0];
    vertexBuffer[70] = max[1];
    vertexBuffer[71] = min[2];

    if (!entity->selected)
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.1f, 0.1f, 0.2f, 1.0f);
        shaderParams->renderParams.blendMode = GFX_ADD;
    }
    else
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
        shaderParams->renderParams.blendMode = GFX_BLEND_NONE;
    }
    gfxSetShaderParams(shaderParams);
    elfDrawLines(24, rnd->lines);

    if (entity->armature)
    {
        elfDrawArmatureDebug(entity->armature, shaderParams);
    }

    elfDrawActorDebug((elfActor*)entity, shaderParams);
}

unsigned char elfCullEntity(elfEntity* entity, elfCamera* camera)
{
    if (!entity->model || !entity->visible)
        return true;

    return !elfAabbInsideFrustum(camera, &entity->cullAabbMin.x, &entity->cullAabbMax.x);
}

unsigned char elfGetEntityChanged(elfEntity* entity) { return entity->moved; }
