#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfCollision;
struct elfList;
struct btCollisionConfiguration;
struct btCollisionDispatcher;
struct btBroadphaseInterface;
struct btConstraintSolver;
struct btDiscreteDynamicsWorld;

struct elfPhysicsWorld
{
    ELF_OBJECT_HEADER;
    btCollisionConfiguration* collisionConfiguration = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btBroadphaseInterface* broadphase = nullptr;
    btConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* world = nullptr;
    float sync = 0.f;
};

elfPhysicsWorld* elfCreatePhysicsWorld();
void elfDestroyPhysicsWorld(void* data);

void elfUpdatePhysicsWorld(elfPhysicsWorld* world, float time);

void elfSetPhysicsWorldGravity(elfPhysicsWorld* world, float x, float y, float z);

elfVec3f elfGetPhysicsWorldGravity(elfPhysicsWorld* world);
elfCollision* elfGetRayCastResult(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz);
elfList* elfGetRayCastResults(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz);