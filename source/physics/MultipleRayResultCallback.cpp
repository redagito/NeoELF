#include "nelf/physics/MultipleRayResultCallback.h"

#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/physics/Collision.h"
#include "nelf/physics/PhysicsObject.h"

MultipleRayResultCallback::MultipleRayResultCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld,
                                                     elfList* list)
    : m_rayFromWorld(rayFromWorld), m_rayToWorld(rayToWorld), m_list(list)
{
}

btScalar MultipleRayResultCallback::addSingleResult(btCollisionWorld::LocalRayResult& rayResult,
                                                    bool normalInWorldSpace)
{
    elfCollision* collision;

    m_collisionObject = rayResult.m_collisionObject;
    if (normalInWorldSpace)
    {
        m_hitNormalWorld = rayResult.m_hitNormalLocal;
    }
    else
    {
        m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis() * rayResult.m_hitNormalLocal;
    }
    m_hitPointWorld.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);

    collision = elfCreateCollision();

    collision->position.x = m_hitPointWorld.x();
    collision->position.y = m_hitPointWorld.y();
    collision->position.z = m_hitPointWorld.z();
    collision->normal.x = m_hitNormalWorld.x();
    collision->normal.y = m_hitNormalWorld.y();
    collision->normal.z = m_hitNormalWorld.z();
    collision->actor = ((elfPhysicsObject*)((btRigidBody*)m_collisionObject)->getUserPointer())->actor;
    elfIncRef((elfObject*)collision->actor);

    elfAppendListObject(m_list, (elfObject*)collision);

    return rayResult.m_hitFraction;
}