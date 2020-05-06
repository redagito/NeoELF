#pragma once

#include "nelf/objectHeader.h"

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

elfPhysicsWorld* elfCreatePhysicsWorld()
{
    elfPhysicsWorld* world;

    world = (elfPhysicsWorld*)malloc(sizeof(elfPhysicsWorld));
    memset(world, 0x0, sizeof(elfPhysicsWorld));
    world->objType = ELF_PHYSICS_WORLD;
    world->objDestr = elfDestroyPhysicsWorld;

    world->collisionConfiguration = new btDefaultCollisionConfiguration();
    world->dispatcher = new btCollisionDispatcher(world->collisionConfiguration);
    world->broadphase = new btDbvtBroadphase();
    world->solver = new btSequentialImpulseConstraintSolver();
    world->world =
        new btDiscreteDynamicsWorld(world->dispatcher, world->broadphase, world->solver, world->collisionConfiguration);
    world->world->getDispatchInfo().m_enableSPU = true;
    world->world->setGravity(btVector3(0.0f, 0.0f, -9.81f));

    elfIncObj(ELF_PHYSICS_WORLD);

    return world;
}

void elfDestroyPhysicsWorld(void* data)
{
    elfPhysicsWorld* world = (elfPhysicsWorld*)data;

    delete world->world;
    delete world->solver;
    delete world->broadphase;
    delete world->dispatcher;
    delete world->collisionConfiguration;

    free(world);

    elfDecObj(ELF_PHYSICS_WORLD);
}

void elfUpdatePhysicsWorld(elfPhysicsWorld* world, float time)
{
    int manifoldCount;
    int contactCount;
    btPersistentManifold* manifold = 0;
    btManifoldPoint* point = 0;
    elfPhysicsObject* obj0;
    elfPhysicsObject* obj1;
    elfCollision* col0;
    elfCollision* col1;
    int i, j;

    world->world->stepSimulation(time, 4);

    manifoldCount = world->dispatcher->getNumManifolds();
    contactCount = 0;

    for (i = 0; i < manifoldCount; i++)
    {
        manifold = world->dispatcher->getManifoldByIndexInternal(i);
        contactCount = manifold->getNumContacts();
        if (!contactCount)
            continue;

        obj0 = (elfPhysicsObject*)((btRigidBody*)manifold->getBody0())->getUserPointer();
        obj1 = (elfPhysicsObject*)((btRigidBody*)manifold->getBody1())->getUserPointer();
        if (obj0 == obj1)
            continue;

        // add elfClearPhysicObjectCollisions somewhere...

        while (elfGetListLength(obj0->collisions) < obj0->collisionCount + contactCount)
        {
            col0 = elfCreateCollision();
            elfAppendListObject(obj0->collisions, (elfObject*)col0);
        }
        while (elfGetListLength(obj1->collisions) < obj1->collisionCount + contactCount)
        {
            col1 = elfCreateCollision();
            elfAppendListObject(obj1->collisions, (elfObject*)col1);
        }

        obj0->collisionCount += contactCount;
        obj1->collisionCount += contactCount;

        for (j = 0, col0 = (elfCollision*)elfBeginList(obj0->collisions),
            col1 = (elfCollision*)elfBeginList(obj1->collisions);
             j < contactCount; col0 = (elfCollision*)elfGetListNext(obj0->collisions),
            col1 = (elfCollision*)elfGetListNext(obj1->collisions), j++)
        {
            point = &manifold->getContactPoint(j);

            if (col0->actor)
                elfDecRef((elfObject*)col0->actor);
            col0->actor = obj1->actor;
            elfIncRef((elfObject*)col0->actor);
            col0->position.x = point->m_positionWorldOnB.getX();
            col0->position.y = point->m_positionWorldOnB.getY();
            col0->position.z = point->m_positionWorldOnB.getZ();
            col0->normal.x = point->m_normalWorldOnB.getX();
            col0->normal.y = point->m_normalWorldOnB.getY();
            col0->normal.z = point->m_normalWorldOnB.getZ();
            col0->depth = point->m_distance1;

            if (col1->actor)
                elfDecRef((elfObject*)col1->actor);
            col1->actor = obj0->actor;
            elfIncRef((elfObject*)col1->actor);
            col1->position.x = point->m_positionWorldOnB.getX();
            col1->position.y = point->m_positionWorldOnB.getY();
            col1->position.z = point->m_positionWorldOnB.getZ();
            col1->normal.x = point->m_normalWorldOnB.getX();
            col1->normal.y = point->m_normalWorldOnB.getY();
            col1->normal.z = point->m_normalWorldOnB.getZ();
            col1->depth = point->m_distance1;
        }
    }
}

void elfSetPhysicsWorldGravity(elfPhysicsWorld* world, float x, float y, float z)
{
    world->world->setGravity(btVector3(x, y, z));
}

elfVec3f elfGetPhysicsWorldGravity(elfPhysicsWorld* world)
{
    btVector3 grav;
    elfVec3f result;

    grav = world->world->getGravity();

    result.x = grav.x();
    result.y = grav.y();
    result.z = grav.z();

    return result;
}

elfCollision* elfGetRayCastResult(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz)
{
    elfCollision* collision;

    btCollisionWorld::ClosestRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz));

    world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

    if (!rayResult.hasHit())
        return NULL;

    collision = elfCreateCollision();

    collision->position.x = rayResult.m_hitPointWorld.x();
    collision->position.y = rayResult.m_hitPointWorld.y();
    collision->position.z = rayResult.m_hitPointWorld.z();
    collision->normal.x = rayResult.m_hitNormalWorld.x();
    collision->normal.y = rayResult.m_hitNormalWorld.y();
    collision->normal.z = rayResult.m_hitNormalWorld.z();
    collision->actor = ((elfPhysicsObject*)((btRigidBody*)rayResult.m_collisionObject)->getUserPointer())->actor;
    elfIncRef((elfObject*)collision->actor);

    return collision;
}

elfList* elfGetRayCastResults(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz)
{
    elfList* list;

    list = elfCreateList();

    MultipleRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz), list);

    world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

    if (!rayResult.hasHit())
    {
        elfDestroyList(list);
        return NULL;
    }

    return list;
}