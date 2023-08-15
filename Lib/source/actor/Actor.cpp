#include "nelf/actor/Actor.h"

#include <cmath>
#include <cstring>

#include "nelf/Engine.h"
#include "nelf/FramePlayer.h"
#include "nelf/Ipo.h"
#include "nelf/List.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/Property.h"
#include "nelf/RenderStation.h"
#include "nelf/Scripting.h"
#include "nelf/String.h"
#include "nelf/actor/Entity.h"
#include "nelf/actor/Light.h"
#include "nelf/audio/AudioSource.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/objectType.h"
#include "nelf/physics/Joint.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/physics/PhysicsTriMesh.h"
#include "nelf/physics/shapeType.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Scene.h"

void elfActorIpoCallback(elfFramePlayer* player)
{
    elfActor* actor;
    float frame;
    elfVec3f pos;
    elfVec3f rot;
    elfVec3f scale;
    elfVec4f qua;

    actor = (elfActor*)elfGetFramePlayerUserData(player);
    frame = elfGetFramePlayerFrame(player);

    if (actor->ipo->loc)
    {
        pos = elfGetIpoLoc(actor->ipo, frame);
        elfSetActorPosition(actor, pos.x, pos.y, pos.z);
    }
    if (actor->ipo->rot)
    {
        rot = elfGetIpoRot(actor->ipo, frame);
        elfSetActorRotation(actor, rot.x, rot.y, rot.z);
    }
    if (actor->ipo->scale)
    {
        scale = elfGetIpoScale(actor->ipo, frame);
        if (actor->objType == ELF_ENTITY)
            elfSetEntityScale((elfEntity*)actor, scale.x, scale.y, scale.z);
    }
    if (actor->ipo->qua)
    {
        qua = elfGetIpoQua(actor->ipo, frame);
        elfSetActorOrientation(actor, qua.x, qua.y, qua.z, qua.w);
    }
}

void elfInitActor(elfActor* actor, bool camera)
{
    if (!camera)
        actor->transform = gfxCreateObjectTransform();
    else
        actor->transform = gfxCreateCameraTransform();

    actor->joints = elfCreateList();
    actor->sources = elfCreateList();
    actor->properties = elfCreateList();

    elfIncRef((elfObject*)actor->joints);
    elfIncRef((elfObject*)actor->sources);
    elfIncRef((elfObject*)actor->properties);

    actor->ipo = elfCreateIpo();
    elfIncRef((elfObject*)actor->ipo);

    actor->ipoPlayer = elfCreateFramePlayer();
    elfIncRef((elfObject*)actor->ipoPlayer);
    elfSetFramePlayerUserData(actor->ipoPlayer, actor);
    elfSetFramePlayerCallback(actor->ipoPlayer, elfActorIpoCallback);

    actor->physics = false;
    actor->shape = ELF_BOX;
    actor->pbbLengths.x = actor->pbbLengths.y = actor->pbbLengths.z = 1.0f;
    actor->pbbOffset.x = actor->pbbOffset.y = actor->pbbOffset.z = 0.0f;
    actor->mass = 0.0f;
    actor->linDamp = 0.0f;
    actor->angDamp = 0.0f;
    actor->linSleep = 0.8f;
    actor->angSleep = 1.0f;
    actor->restitution = 0.0f;
    actor->anisFric.x = actor->anisFric.y = actor->anisFric.z = 1.0f;
    actor->linFactor.x = actor->linFactor.y = actor->linFactor.z = 1.0f;
    actor->angFactor.x = actor->angFactor.y = actor->angFactor.z = 1.0f;

    actor->moved = true;
}

void elfUpdateActor(elfActor* actor)
{
    // TODO Why static
    static float oposition[3];
    static float oorient[4];
    static float position[3];
    static float orient[4];
    static elfAudioSource* source;

    if (actor->object && !elfIsPhysicsObjectStatic(actor->object))
    {
        gfxGetTransformPosition(actor->transform, oposition);
        gfxGetTransformOrientation(actor->transform, oorient);

        elfGetPhysicsObjectPosition(actor->object, position);
        elfGetPhysicsObjectOrientation(actor->object, orient);

        gfxSetTransformPosition(actor->transform, position[0], position[1], position[2]);
        gfxSetTransformOrientation(actor->transform, orient[0], orient[1], orient[2], orient[3]);

        if (actor->dobject)
        {
            elfSetPhysicsObjectPosition(actor->dobject, position[0], position[1], position[2]);
            elfSetPhysicsObjectOrientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);
        }

        if (memcmp(&oposition, &position, sizeof(float) * 3))
            actor->moved = true;
        if (memcmp(&oorient, &orient, sizeof(float) * 4))
            actor->moved = true;
    }
    else
    {
        elfGetActorPosition_(actor, position);
    }

    if (actor->script && actor->scene->runScripts)
    {
        eng->actor = (elfObject*)actor;
        elfIncRef((elfObject*)actor);

        elfRunString("me = GetActor()");
        elfRunScript(actor->script);
        elfRunString("me = nil");

        elfDecRef((elfObject*)actor);
        eng->actor = nullptr;
    }

    if (actor->object)
        elfClearPhysicsObjectCollisions(actor->object);

    for (source = (elfAudioSource*)elfBeginList(actor->sources); source;
         source = (elfAudioSource*)elfGetListNext(actor->sources))
    {
        if (elfGetObjectRefCount((elfObject*)source) < 2 && !elfIsSoundPlaying(source) && !elfIsSoundPaused(source))
        {
            elfRemoveListObject(actor->sources, (elfObject*)source);
        }
        else
        {
            elfSetSoundPosition(source, position[0], position[1], position[2]);
        }
    }

    elfUpdateFramePlayer(actor->ipoPlayer);
}

void elfActorPreDraw(elfActor* actor) {}

void elfActorPostDraw(elfActor* actor) { actor->moved = false; }

void elfCleanActor(elfActor* actor)
{
    elfJoint* joint;

    if (actor->name)
        elfDestroyString(actor->name);
    if (actor->filePath)
        elfDestroyString(actor->filePath);
    if (actor->transform)
        gfxDestroyTransform(actor->transform);
    if (actor->object)
    {
        elfSetPhysicsObjectActor(actor->object, nullptr);
        elfSetPhysicsObjectWorld(actor->object, nullptr);
        elfDecRef((elfObject*)actor->object);
    }
    if (actor->dobject)
    {
        elfSetPhysicsObjectActor(actor->dobject, nullptr);
        elfSetPhysicsObjectWorld(actor->dobject, nullptr);
        elfDecRef((elfObject*)actor->dobject);
    }
    if (actor->script)
        elfDecRef((elfObject*)actor->script);

    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        if (elfGetJointActorA(joint) == actor)
            elfRemoveListObject(elfGetJointActorB(joint)->joints, (elfObject*)joint);
        else
            elfRemoveListObject(elfGetJointActorA(joint)->joints, (elfObject*)joint);
        elfClearJoint(joint);
    }

    elfDecRef((elfObject*)actor->joints);
    elfDecRef((elfObject*)actor->sources);
    elfDecRef((elfObject*)actor->properties);

    elfDecRef((elfObject*)actor->ipo);
    elfDecRef((elfObject*)actor->ipoPlayer);
}

const char* elfGetActorName(elfActor* actor) { return actor->name; }

const char* elfGetActorFilePath(elfActor* actor) { return actor->filePath; }

elfScript* elfGetActorScript(elfActor* actor) { return actor->script; }

void elfSetActorName(elfActor* actor, const char* name)
{
    if (actor->name)
        elfDestroyString(actor->name);
    actor->name = elfCreateString(name);
}

void elfSetActorScript(elfActor* actor, elfScript* script)
{
    if (actor->script)
        elfDecRef((elfObject*)actor->script);
    actor->script = script;
    if (actor->script)
        elfIncRef((elfObject*)actor->script);
}

void elfClearActorScript(elfActor* actor)
{
    if (actor->script)
        elfDecRef((elfObject*)actor->script);
    actor->script = nullptr;
}

void elfSetActorPosition(elfActor* actor, float x, float y, float z)
{
    actor->moved = true;

    gfxSetTransformPosition(actor->transform, x, y, z);

    if (actor->object)
        elfSetPhysicsObjectPosition(actor->object, x, y, z);
    if (actor->dobject)
        elfSetPhysicsObjectPosition(actor->dobject, x, y, z);

    if (actor->objType == ELF_LIGHT)
        elfSetActorPosition((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfSetActorRotation(elfActor* actor, float x, float y, float z)
{
    float orient[4];

    actor->moved = true;

    gfxSetTransformRotation(actor->transform, x, y, z);
    gfxGetTransformOrientation(actor->transform, orient);

    if (actor->object)
        elfSetPhysicsObjectOrientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
    if (actor->dobject)
        elfSetPhysicsObjectOrientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

    if (actor->objType == ELF_LIGHT)
        elfSetActorRotation((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfSetActorOrientation(elfActor* actor, float x, float y, float z, float w)
{
    actor->moved = true;

    gfxSetTransformOrientation(actor->transform, x, y, z, w);

    if (actor->object)
        elfSetPhysicsObjectOrientation(actor->object, x, y, z, w);
    if (actor->dobject)
        elfSetPhysicsObjectOrientation(actor->dobject, x, y, z, w);

    if (actor->objType == ELF_LIGHT)
        elfSetActorOrientation((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z, w);
}

void elfRotateActor(elfActor* actor, float x, float y, float z)
{
    float orient[4];

    actor->moved = true;

    gfxRotateTransform(actor->transform, x * eng->sync, y * eng->sync, z * eng->sync);
    gfxGetTransformOrientation(actor->transform, orient);

    if (actor->object)
        elfSetPhysicsObjectOrientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
    if (actor->dobject)
        elfSetPhysicsObjectOrientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

    if (actor->objType == ELF_LIGHT)
        elfRotateActor((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfRotateActorLocal(elfActor* actor, float x, float y, float z)
{
    float orient[4];

    actor->moved = true;

    gfxRotateTransformLocal(actor->transform, x * eng->sync, y * eng->sync, z * eng->sync);
    gfxGetTransformOrientation(actor->transform, orient);

    if (actor->object)
        elfSetPhysicsObjectOrientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
    if (actor->dobject)
        elfSetPhysicsObjectOrientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

    if (actor->objType == ELF_LIGHT)
        elfRotateActorLocal((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfMoveActor(elfActor* actor, float x, float y, float z)
{
    float position[3];

    actor->moved = true;

    gfxMoveTransform(actor->transform, x * eng->sync, y * eng->sync, z * eng->sync);
    gfxGetTransformPosition(actor->transform, position);

    if (actor->object)
        elfSetPhysicsObjectPosition(actor->object, position[0], position[1], position[2]);
    if (actor->dobject)
        elfSetPhysicsObjectPosition(actor->dobject, position[0], position[1], position[2]);

    if (actor->objType == ELF_LIGHT)
        elfMoveActor((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfMoveActorLocal(elfActor* actor, float x, float y, float z)
{
    float position[3];

    actor->moved = true;

    gfxMoveTransformLocal(actor->transform, x * eng->sync, y * eng->sync, z * eng->sync);
    gfxGetTransformPosition(actor->transform, position);

    if (actor->object)
        elfSetPhysicsObjectPosition(actor->object, position[0], position[1], position[2]);
    if (actor->dobject)
        elfSetPhysicsObjectPosition(actor->dobject, position[0], position[1], position[2]);

    if (actor->objType == ELF_LIGHT)
        elfMoveActorLocal((elfActor*)((elfLight*)actor)->shadowCamera, x, y, z);
}

void elfSetActorPositionRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z)
{
    elfVec3f vec;
    elfVec3f pos;
    elfVec3f result;
    elfVec4f orient;

    vec.x = x;
    vec.y = y;
    vec.z = z;

    elfGetActorOrientation_(to, &orient.x);
    elfGetActorPosition_(to, &pos.x);

    gfxMulQuaVec(&orient.x, &vec.x, &result.x);

    result.x += pos.x;
    result.y += pos.y;
    result.z += pos.z;

    elfSetActorPosition(actor, result.x, result.y, result.z);

    if (actor->objType == ELF_LIGHT)
        elfSetActorPositionRelativeTo((elfActor*)((elfLight*)actor)->shadowCamera, to, x, y, z);
}

void elfSetActorRotationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z)
{
    elfVec4f lorient;
    elfVec4f orient;
    elfVec4f result;

    gfxQuaFromEuler(x, y, z, &lorient.x);

    elfGetActorOrientation_(to, &orient.x);

    gfxMulQuaQua(&orient.x, &lorient.x, &result.x);

    elfSetActorOrientation(actor, result.x, result.y, result.z, result.w);

    if (actor->objType == ELF_LIGHT)
        elfSetActorRotationRelativeTo((elfActor*)((elfLight*)actor)->shadowCamera, to, x, y, z);
}

void elfSetActorOrientationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z, float w)
{
    elfVec4f lorient;
    elfVec4f orient;
    elfVec4f result;

    lorient.x = x;
    lorient.y = y;
    lorient.z = z;
    lorient.w = w;

    elfGetActorOrientation_(to, &orient.x);

    gfxMulQuaQua(&lorient.x, &orient.x, &result.x);

    elfSetActorOrientation(actor, result.x, result.y, result.z, result.w);
}

elfVec3f elfGetActorPosition(elfActor* actor)
{
    elfVec3f pos;
    gfxGetTransformPosition(actor->transform, &pos.x);
    return pos;
}

elfVec3f elfGetActorRotation(elfActor* actor)
{
    elfVec3f rot;
    gfxGetTransformRotation(actor->transform, &rot.x);
    return rot;
}

elfVec4f elfGetActorOrientation(elfActor* actor)
{
    elfVec4f orient;
    gfxGetTransformOrientation(actor->transform, &orient.x);
    return orient;
}

void elfGetActorPosition_(elfActor* actor, float* params) { gfxGetTransformPosition(actor->transform, params); }

void elfGetActorRotation_(elfActor* actor, float* params) { gfxGetTransformRotation(actor->transform, params); }

void elfGetActorOrientation_(elfActor* actor, float* params) { gfxGetTransformOrientation(actor->transform, params); }

void elfSetActorPhysics(elfActor* actor, bool physics)
{
    float position[3];
    float orient[4];
    float scale[3];
    float radius;
    elfJoint* joint;
    elfEntity* entity;

    if (actor->object)
    {
        elfSetPhysicsObjectActor(actor->object, NULL);
        elfSetPhysicsObjectWorld(actor->object, NULL);
        elfDecRef((elfObject*)actor->object);
        actor->object = nullptr;
    }

    actor->physics = false;

    if (!physics)
        return;

    actor->physics = true;

    switch (actor->shape)
    {
    case ELF_BOX:
        actor->object = elfCreatePhysicsObjectBox(actor->pbbLengths.x / 2.0f, actor->pbbLengths.y / 2.0f,
                                                  actor->pbbLengths.z / 2.0f, actor->mass, actor->pbbOffset.x,
                                                  actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_SPHERE:
        if (actor->pbbLengths.x > actor->pbbLengths.y && actor->pbbLengths.x > actor->pbbLengths.z)
            radius = actor->pbbLengths.x / 2.0f;
        else if (actor->pbbLengths.y > actor->pbbLengths.x && actor->pbbLengths.y > actor->pbbLengths.z)
            radius = actor->pbbLengths.y / 2.0f;
        else
            radius = actor->pbbLengths.z / 2.0f;

        actor->object = elfCreatePhysicsObjectSphere(radius, actor->mass, actor->pbbOffset.x, actor->pbbOffset.y,
                                                     actor->pbbOffset.z);
        break;
    case ELF_MESH:
        if (actor->objType != ELF_ENTITY)
            return;
        entity = (elfEntity*)actor;
        if (!entity->model || !elfGetModelIndices(entity->model))
            return;
        if (!entity->model->triMesh)
        {
            entity->model->triMesh =
                elfCreatePhysicsTriMesh(elfGetModelVertices(entity->model), elfGetModelIndices(entity->model),
                                        elfGetModelIndiceCount(entity->model));
            elfIncRef((elfObject*)entity->model->triMesh);
        }
        actor->object = elfCreatePhysicsObjectMesh(entity->model->triMesh, actor->mass);
        break;
    case ELF_CAPSULE_X:
        if (actor->pbbLengths.z > actor->pbbLengths.y)
            radius = actor->pbbLengths.z / 2.0f;
        else
            radius = actor->pbbLengths.y / 2.0f;

        actor->object =
            elfCreatePhysicsObjectCapsule(ELF_CAPSULE_X, elfFloatMax(actor->pbbLengths.x - radius * 2, 0.0f), radius,
                                          actor->mass, actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_CAPSULE_Y:
        if (actor->pbbLengths.z > actor->pbbLengths.x)
            radius = actor->pbbLengths.z / 2.0f;
        else
            radius = actor->pbbLengths.x / 2.0f;

        actor->object =
            elfCreatePhysicsObjectCapsule(ELF_CAPSULE_Y, elfFloatMax(actor->pbbLengths.y - radius * 2, 0.0f), radius,
                                          actor->mass, actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_CAPSULE_Z:
        if (actor->pbbLengths.x > actor->pbbLengths.y)
            radius = actor->pbbLengths.x / 2.0f;
        else
            radius = actor->pbbLengths.y / 2.0f;

        actor->object =
            elfCreatePhysicsObjectCapsule(ELF_CAPSULE_Z, elfFloatMax(actor->pbbLengths.z - radius * 2, 0.0f), radius,
                                          actor->mass, actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_CONE_X:
        if (actor->pbbLengths.z > actor->pbbLengths.y)
            radius = actor->pbbLengths.z / 2.0f;
        else
            radius = actor->pbbLengths.y / 2.0f;

        actor->object = elfCreatePhysicsObjectCone(ELF_CONE_X, actor->pbbLengths.x, radius, actor->mass,
                                                   actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_CONE_Y:
        if (actor->pbbLengths.z > actor->pbbLengths.x)
            radius = actor->pbbLengths.z / 2.0f;
        else
            radius = actor->pbbLengths.x / 2.0f;

        actor->object = elfCreatePhysicsObjectCone(ELF_CONE_Y, actor->pbbLengths.y, radius, actor->mass,
                                                   actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    case ELF_CONE_Z:
        if (actor->pbbLengths.x > actor->pbbLengths.y)
            radius = actor->pbbLengths.x / 2.0f;
        else
            radius = actor->pbbLengths.y / 2.0f;

        actor->object = elfCreatePhysicsObjectCone(ELF_CONE_Z, actor->pbbLengths.z, radius, actor->mass,
                                                   actor->pbbOffset.x, actor->pbbOffset.y, actor->pbbOffset.z);
        break;
    default:
        return;
    }

    elfSetPhysicsObjectActor(actor->object, (elfActor*)actor);
    elfIncRef((elfObject*)actor->object);

    gfxGetTransformPosition(actor->transform, position);
    gfxGetTransformOrientation(actor->transform, orient);
    gfxGetTransformScale(actor->transform, scale);

    elfSetPhysicsObjectPosition(actor->object, position[0], position[1], position[2]);
    elfSetPhysicsObjectOrientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
    elfSetPhysicsObjectScale(actor->object, scale[0], scale[1], scale[2]);

    elfSetPhysicsObjectDamping(actor->object, actor->linDamp, actor->angDamp);
    elfSetPhysicsObjectSleepThresholds(actor->object, actor->linSleep, actor->angSleep);
    elfSetPhysicsObjectRestitution(actor->object, actor->restitution);
    elfSetPhysicsObjectAnisotropicFriction(actor->object, actor->anisFric.x, actor->anisFric.y, actor->anisFric.z);
    elfSetPhysicsObjectLinearFactor(actor->object, actor->linFactor.x, actor->linFactor.y, actor->linFactor.z);
    elfSetPhysicsObjectAngularFactor(actor->object, actor->angFactor.x, actor->angFactor.y, actor->angFactor.z);

    if (actor->scene)
        elfSetPhysicsObjectWorld(actor->object, actor->scene->world);

    // things are seriously going to blow up if we don't update the joints
    // when a new physics object has been created
    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        elfSetJointWorld(joint, NULL);
        if (actor->scene)
            elfSetJointWorld(joint, actor->scene->world);
    }
}

void elfSetActorShape(elfActor* actor, int shape)
{
    actor->shape = (unsigned char)shape;
    if (actor->object)
        elfSetActorPhysics(actor, true);
}

void elfSetActorBoundingLengths(elfActor* actor, float x, float y, float z)
{
    actor->pbbLengths.x = x;
    actor->pbbLengths.y = y;
    actor->pbbLengths.z = z;

    if (actor->object)
        elfSetActorPhysics(actor, true);
}

void elfSetActorBoundingOffset(elfActor* actor, float x, float y, float z)
{
    actor->pbbOffsetSet = true;

    actor->pbbOffset.x = x;
    actor->pbbOffset.y = y;
    actor->pbbOffset.z = z;

    if (actor->object)
        elfSetActorPhysics(actor, true);
}

void elfResetActorBoundingOffsetSetFlag(elfActor* actor) { actor->pbbOffsetSet = false; }

void elfSetActorMass(elfActor* actor, float mass)
{
    actor->mass = mass;
    if (actor->object)
        elfSetActorPhysics(actor, true);
}

void elfSetActorDamping(elfActor* actor, float linDamp, float angDamp)
{
    actor->linDamp = linDamp;
    actor->angDamp = angDamp;
    if (actor->object)
        elfSetPhysicsObjectDamping(actor->object, linDamp, angDamp);
}

void elfSetActorSleep(elfActor* actor, float linThrs, float angThrs)
{
    actor->linSleep = linThrs;
    actor->angSleep = angThrs;
    if (actor->object)
        elfSetPhysicsObjectSleepThresholds(actor->object, linThrs, angThrs);
}

void elfSetActorRestitution(elfActor* actor, float restitution)
{
    actor->restitution = restitution;
    if (actor->object)
        elfSetPhysicsObjectRestitution(actor->object, restitution);
}

void elfSetActorAnisotropicFriction(elfActor* actor, float x, float y, float z)
{
    actor->anisFric.x = x;
    actor->anisFric.y = y;
    actor->anisFric.z = z;
    if (actor->object)
        elfSetPhysicsObjectAnisotropicFriction(actor->object, x, y, z);
}

void elfSetActorLinearFactor(elfActor* actor, float x, float y, float z)
{
    actor->linFactor.x = x;
    actor->linFactor.y = y;
    actor->linFactor.z = z;
    if (actor->object)
        elfSetPhysicsObjectLinearFactor(actor->object, x, y, z);
}

void elfSetActorAngularFactor(elfActor* actor, float x, float y, float z)
{
    actor->angFactor.x = x;
    actor->angFactor.y = y;
    actor->angFactor.z = z;
    if (actor->object)
        elfSetPhysicsObjectAngularFactor(actor->object, x, y, z);
}

void elfAddActorForce(elfActor* actor, float x, float y, float z)
{
    if (actor->object)
        elfAddPhysicsObjectForce(actor->object, x, y, z);
}

void elfAddActorForceLocal(elfActor* actor, float x, float y, float z)
{
    elfVec3f vec;
    elfVec3f result;
    elfVec4f orient;

    if (actor->object)
    {
        vec.x = x;
        vec.y = y;
        vec.z = z;
        elfGetActorOrientation_(actor, &orient.x);
        gfxMulQuaVec(&orient.x, &vec.x, &result.x);
        elfAddPhysicsObjectForce(actor->object, result.x, result.y, result.z);
    }
}

void elfAddActorTorque(elfActor* actor, float x, float y, float z)
{
    if (actor->object)
        elfAddPhysicsObjectTorque(actor->object, x, y, z);
}

void elfSetActorLinearVelocity(elfActor* actor, float x, float y, float z)
{
    if (actor->object)
        elfSetPhysicsObjectLinearVelocity(actor->object, x, y, z);
}

void elfSetActorLinearVelocityLocal(elfActor* actor, float x, float y, float z)
{
    elfVec3f vec;
    elfVec3f result;
    elfVec4f orient;

    if (actor->object)
    {
        vec.x = x;
        vec.y = y;
        vec.z = z;
        elfGetActorOrientation_(actor, &orient.x);
        gfxMulQuaVec(&orient.x, &vec.x, &result.x);
        elfSetPhysicsObjectLinearVelocity(actor->object, result.x, result.y, result.z);
    }
}

void elfSetActorAngularVelocity(elfActor* actor, float x, float y, float z)
{
    if (actor->object)
        elfSetPhysicsObjectAngularVelocity(actor->object, x, y, z);
}

unsigned char elfGetActorPhysics(elfActor* actor) { return actor->physics; }

int elfGetActorShape(elfActor* actor) { return actor->shape; }

elfVec3f elfGetActorBoundingLengths(elfActor* actor) { return actor->pbbLengths; }

elfVec3f elfGetActorBoundingOffset(elfActor* actor) { return actor->pbbOffset; }

float elfGetActorMass(elfActor* actor) { return actor->mass; }

float elfGetActorLinearDamping(elfActor* actor) { return actor->linDamp; }

float elfGetActorAngularDamping(elfActor* actor) { return actor->angDamp; }

float elfGetActorLinearSleep(elfActor* actor) { return actor->linSleep; }

float elfGetActorAngularSleep(elfActor* actor) { return actor->angSleep; }

float elfGetActorRestitution(elfActor* actor) { return actor->restitution; }

elfVec3f elfGetActorAnisotropicFriction(elfActor* actor) { return actor->anisFric; }

elfVec3f elfGetActorLinearFactor(elfActor* actor) { return actor->linFactor; }

elfVec3f elfGetActorAngularFactor(elfActor* actor) { return actor->angFactor; }

elfVec3f elfGetActorLinearVelocity(elfActor* actor)
{
    elfVec3f result;
    memset(&result, 0x0, sizeof(elfVec3f));

    if (actor->object)
        elfGetPhysicsObjectLinearVelocity(actor->object, &result.x);

    return result;
}

elfVec3f elfGetActorAngularVelocity(elfActor* actor)
{
    elfVec3f result;
    memset(&result, 0x0, sizeof(elfVec3f));

    if (actor->object)
        elfGetPhysicsObjectAngularVelocity(actor->object, &result.x);

    return result;
}

elfJoint* elfAddActorHingeJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz,
                                float ax, float ay, float az)
{
    elfJoint* joint;

    if (!actor->object || !actor2->object)
        return NULL;

    joint = elfCreateHingeJoint(actor, actor2, name, px, py, pz, ax, ay, az);

    elfAppendListObject(actor->joints, (elfObject*)joint);
    elfAppendListObject(actor2->joints, (elfObject*)joint);

    return joint;
}

elfJoint* elfAddActorBallJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz)
{
    elfJoint* joint;

    if (!actor->object || !actor2->object)
        return NULL;

    joint = elfCreateBallJoint(actor, actor2, name, px, py, pz);

    elfAppendListObject(actor->joints, (elfObject*)joint);
    elfAppendListObject(actor2->joints, (elfObject*)joint);

    return joint;
}

elfJoint* elfAddActorConeTwistJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz,
                                    float ax, float ay, float az)
{
    elfJoint* joint;

    if (!actor->object || !actor2->object)
        return NULL;

    joint = elfCreateConeTwistJoint(actor, actor2, name, px, py, pz, ax, ay, az);

    elfAppendListObject(actor->joints, (elfObject*)joint);
    elfAppendListObject(actor2->joints, (elfObject*)joint);

    return joint;
}

elfJoint* elfGetActorJoint(elfActor* actor, const char* name)
{
    elfJoint* joint;

    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        if (!strcmp(elfGetJointName(joint), name))
            return joint;
    }

    return NULL;
}

elfJoint* elfGetActorJointByIndex(elfActor* actor, int idx) { return (elfJoint*)elfGetListObject(actor->joints, idx); }

bool elfRemoveActorJoint(elfActor* actor, const char* name)
{
    elfJoint* joint;

    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        if (!strcmp(elfGetJointName(joint), name))
        {
            if (elfGetJointActorA(joint) == actor)
                elfRemoveListObject(elfGetJointActorB(joint)->joints, (elfObject*)joint);
            else
                elfRemoveListObject(elfGetJointActorA(joint)->joints, (elfObject*)joint);
            elfClearJoint(joint);
            return elfRemoveListObject(actor->joints, (elfObject*)joint);
        }
    }

    return false;
}

bool elfRemoveActorJointByIndex(elfActor* actor, int idx)
{
    elfJoint* joint;
    int i;

    for (i = 0, joint = (elfJoint*)elfBeginList(actor->joints); joint;
         joint = (elfJoint*)elfGetListNext(actor->joints), i++)
    {
        if (i == idx)
        {
            if (elfGetJointActorA(joint) == actor)
                elfRemoveListObject(elfGetJointActorB(joint)->joints, (elfObject*)joint);
            else
                elfRemoveListObject(elfGetJointActorA(joint)->joints, (elfObject*)joint);
            elfClearJoint(joint);
            return elfRemoveListObject(actor->joints, (elfObject*)joint);
        }
    }

    return false;
}

bool elfRemoveActorJointByObject(elfActor* actor, elfJoint* joint)
{
    if (elfGetJointActorA(joint) == actor)
        elfRemoveListObject(elfGetJointActorB(joint)->joints, (elfObject*)joint);
    else
        elfRemoveListObject(elfGetJointActorA(joint)->joints, (elfObject*)joint);
    elfClearJoint(joint);
    return elfRemoveListObject(actor->joints, (elfObject*)joint);
}

void elfSetActorIpo(elfActor* actor, elfIpo* ipo)
{
    if (!ipo)
        return;
    if (actor->ipo)
        elfDecRef((elfObject*)actor->ipo);
    actor->ipo = ipo;
    if (actor->ipo)
        elfIncRef((elfObject*)actor->ipo);
}

elfIpo* elfGetActorIpo(elfActor* actor) { return actor->ipo; }

void elfSetActorIpoFrame(elfActor* actor, float frame) { elfSetFramePlayerFrame(actor->ipoPlayer, frame); }

void elfPlayActorIpo(elfActor* actor, float start, float end, float speed)
{
    elfPlayFramePlayer(actor->ipoPlayer, start, end, speed);
}

void elfLoopActorIpo(elfActor* actor, float start, float end, float speed)
{
    elfLoopFramePlayer(actor->ipoPlayer, start, end, speed);
}

void elfStopActorIpo(elfActor* actor) { elfStopFramePlayer(actor->ipoPlayer); }

void elfPauseActorIpo(elfActor* actor) { elfStopFramePlayer(actor->ipoPlayer); }

void elfResumeActorIpo(elfActor* actor) { elfStopFramePlayer(actor->ipoPlayer); }

float elfGetActorIpoStart(elfActor* actor) { return elfGetFramePlayerStart(actor->ipoPlayer); }

float elfGetActorIpoEnd(elfActor* actor) { return elfGetFramePlayerEnd(actor->ipoPlayer); }

float elfGetActorIpoSpeed(elfActor* actor) { return elfGetFramePlayerSpeed(actor->ipoPlayer); }

float elfGetActorIpoFrame(elfActor* actor) { return elfGetFramePlayerFrame(actor->ipoPlayer); }

bool elfIsActorIpoPlaying(elfActor* actor) { return elfIsFramePlayerPlaying(actor->ipoPlayer); }

bool elfIsActorIpoPaused(elfActor* actor) { return elfIsFramePlayerPaused(actor->ipoPlayer); }

int elfGetActorCollisionCount(elfActor* actor)
{
    if (actor->object)
        return elfGetPhysicsObjectCollisionCount(actor->object);
    return 0;
}

elfCollision* elfGetActorCollision(elfActor* actor, int idx)
{
    return elfGetPhysicsObjectCollision(actor->object, idx);
}

int elfGetActorPropertyCount(elfActor* actor) { return elfGetListLength(actor->properties); }

void elfAddPropertyToActor(elfActor* actor, elfProperty* property)
{
    elfAppendListObject(actor->properties, (elfObject*)property);
}

elfProperty* elfGetActorPropertyByName(elfActor* actor, const char* name)
{
    elfProperty* prop;

    if (!name || strlen(name) < 1)
        return nullptr;

    for (prop = (elfProperty*)elfBeginList(actor->properties); prop;
         prop = (elfProperty*)elfGetListNext(actor->properties))
    {
        if (!strcmp(prop->name, name))
        {
            return prop;
        }
    }

    return NULL;
}

elfProperty* elfGetActorPropertyByIndex(elfActor* actor, int idx)
{
    return (elfProperty*)elfGetListObject(actor->properties, idx);
}

bool elfRemoveActorPropertyByName(elfActor* actor, const char* name)
{
    elfProperty* prop;

    if (!name || strlen(name) < 1)
        return false;

    for (prop = (elfProperty*)elfBeginList(actor->properties); prop;
         prop = (elfProperty*)elfGetListNext(actor->properties))
    {
        if (!strcmp(prop->name, name))
        {
            elfRemoveListObject(actor->properties, (elfObject*)prop);
            return true;
        }
    }

    return false;
}

bool elfRemoveActorPropertyByIndex(elfActor* actor, int idx)
{
    elfProperty* prop;
    int i;

    if (idx < 0 && idx >= elfGetListLength(actor->properties))
        return false;

    for (i = 0, prop = (elfProperty*)elfBeginList(actor->properties); prop;
         prop = (elfProperty*)elfGetListNext(actor->properties), i++)
    {
        if (i == idx)
        {
            elfRemoveListObject(actor->properties, (elfObject*)prop);
            return true;
        }
    }

    return false;
}

bool elfRemoveActorPropertyByObject(elfActor* actor, elfProperty* property)
{
    return elfRemoveListObject(actor->properties, (elfObject*)property);
}

void elfRemoveActorProperties(elfActor* actor)
{
    elfDecRef((elfObject*)actor->properties);
    actor->properties = elfCreateList();
    elfIncRef((elfObject*)actor->properties);
}

void elfSetActorSelected(elfActor* actor, bool selected) { actor->selected = selected; }

bool elfGetActorSelected(elfActor* actor) { return actor->selected; }

void elfDrawActorDebug(elfActor* actor, gfxShaderParams* shaderParams)
{
    float min[3];
    float max[3];
    float* vertexBuffer;
    float step;
    float size;
    int i;
    float halfLength;

    if (!actor->selected || !elfGetActorPhysics(actor))
        return;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.5f, 1.0f, 0.5f, 1.0f);
    gfxSetShaderParams(shaderParams);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(renderStation->lines);

    if (actor->shape == ELF_BOX)
    {
        min[0] = -actor->pbbLengths.x / 2 + actor->pbbOffset.x;
        min[1] = -actor->pbbLengths.y / 2 + actor->pbbOffset.y;
        min[2] = -actor->pbbLengths.z / 2 + actor->pbbOffset.z;
        max[0] = actor->pbbLengths.x / 2 + actor->pbbOffset.x;
        max[1] = actor->pbbLengths.y / 2 + actor->pbbOffset.y;
        max[2] = actor->pbbLengths.z / 2 + actor->pbbOffset.z;

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

        gfxSetShaderParams(shaderParams);
        elfDrawLines(24, renderStation->lines);
    }
    else if (actor->shape == ELF_SPHERE)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.x > actor->pbbLengths.y && actor->pbbLengths.x > actor->pbbLengths.z)
            size = actor->pbbLengths.x / 2.0f;
        else if (actor->pbbLengths.y > actor->pbbLengths.x && actor->pbbLengths.y > actor->pbbLengths.z)
            size = actor->pbbLengths.y / 2.0f;
        else
            size = actor->pbbLengths.z / 2.0f;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);
    }
    else if (actor->shape == ELF_CAPSULE_X)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.y > actor->pbbLengths.z)
            size = actor->pbbLengths.y / 2.0f;
        else
            size = actor->pbbLengths.z / 2.0f;

        halfLength = elfFloatMax(actor->pbbLengths.x - size * 2, 0.0f) / 2.0f;

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = +actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);
    }
    else if (actor->shape == ELF_CAPSULE_Y)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.x > actor->pbbLengths.z)
            size = actor->pbbLengths.x / 2.0f;
        else
            size = actor->pbbLengths.z / 2.0f;

        halfLength = elfFloatMax(actor->pbbLengths.y - size * 2, 0.0f) / 2.0f;

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);
    }
    else if (actor->shape == ELF_CAPSULE_Z)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.x > actor->pbbLengths.y)
            size = actor->pbbLengths.x / 2.0f;
        else
            size = actor->pbbLengths.y / 2.0f;

        halfLength = elfFloatMax(actor->pbbLengths.z - size * 2, 0.0f) / 2.0f;

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        for (i = 0; i < 32; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.z;
        }

        for (i = 32; i < 64; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.z;
        }

        for (i = 64; i < 96; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size - halfLength + actor->pbbOffset.z;
        }

        for (i = 96; i < 128; i++)
        {
            vertexBuffer[i * 3] = actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + halfLength + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);
    }
    else if (actor->shape == ELF_CONE_X)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.y > actor->pbbLengths.z)
            size = actor->pbbLengths.y / 2.0f;
        else
            size = actor->pbbLengths.z / 2.0f;

        halfLength = actor->pbbLengths.x / 2.0f;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -halfLength + actor->pbbOffset.z;
            vertexBuffer[i * 3 + 1] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.y;
        }

        elfDrawLineLoop(128, renderStation->lines);

        vertexBuffer[0] = halfLength + actor->pbbOffset.z;
        vertexBuffer[1] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[2] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[3] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[4] = size + actor->pbbOffset.x;
        vertexBuffer[5] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[6] = halfLength + actor->pbbOffset.z;
        vertexBuffer[7] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[8] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[9] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[10] = -size + actor->pbbOffset.x;
        vertexBuffer[11] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[12] = halfLength + actor->pbbOffset.z;
        vertexBuffer[13] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[14] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[15] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[16] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[17] = size + actor->pbbOffset.y;
        vertexBuffer[18] = halfLength + actor->pbbOffset.z;
        vertexBuffer[19] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[20] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[21] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[22] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[23] = -size + actor->pbbOffset.y;

        elfDrawLines(8, renderStation->lines);
    }
    else if (actor->shape == ELF_CONE_Y)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.x > actor->pbbLengths.z)
            size = actor->pbbLengths.x / 2.0f;
        else
            size = actor->pbbLengths.z / 2.0f;

        halfLength = actor->pbbLengths.y / 2.0f;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = -halfLength + actor->pbbOffset.z;
            vertexBuffer[i * 3 + 2] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.y;
        }

        elfDrawLineLoop(128, renderStation->lines);

        vertexBuffer[0] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[1] = halfLength + actor->pbbOffset.z;
        vertexBuffer[2] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[3] = size + actor->pbbOffset.x;
        vertexBuffer[4] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[5] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[6] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[7] = halfLength + actor->pbbOffset.z;
        vertexBuffer[8] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[9] = -size + actor->pbbOffset.x;
        vertexBuffer[10] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[11] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[12] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[13] = halfLength + actor->pbbOffset.z;
        vertexBuffer[14] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[15] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[16] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[17] = size + actor->pbbOffset.y;
        vertexBuffer[18] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[19] = halfLength + actor->pbbOffset.z;
        vertexBuffer[20] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[21] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[22] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[23] = -size + actor->pbbOffset.y;

        elfDrawLines(8, renderStation->lines);
    }
    else if (actor->shape == ELF_CONE_Z)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        if (actor->pbbLengths.x > actor->pbbLengths.y)
            size = actor->pbbLengths.x / 2.0f;
        else
            size = actor->pbbLengths.y / 2.0f;

        halfLength = actor->pbbLengths.z / 2.0f;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * size + actor->pbbOffset.x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * size + actor->pbbOffset.y;
            vertexBuffer[i * 3 + 2] = -halfLength + actor->pbbOffset.z;
        }

        elfDrawLineLoop(128, renderStation->lines);

        vertexBuffer[0] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[1] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[2] = halfLength + actor->pbbOffset.z;
        vertexBuffer[3] = size + actor->pbbOffset.x;
        vertexBuffer[4] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[5] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[6] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[7] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[8] = halfLength + actor->pbbOffset.z;
        vertexBuffer[9] = -size + actor->pbbOffset.x;
        vertexBuffer[10] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[11] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[12] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[13] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[14] = halfLength + actor->pbbOffset.z;
        vertexBuffer[15] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[16] = size + actor->pbbOffset.y;
        vertexBuffer[17] = -halfLength + actor->pbbOffset.z;
        vertexBuffer[18] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[19] = 0.0f + actor->pbbOffset.y;
        vertexBuffer[20] = halfLength + actor->pbbOffset.z;
        vertexBuffer[21] = 0.0f + actor->pbbOffset.x;
        vertexBuffer[22] = -size + actor->pbbOffset.y;
        vertexBuffer[23] = -halfLength + actor->pbbOffset.z;

        elfDrawLines(8, renderStation->lines);
    }
}
