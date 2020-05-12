#pragma once

#include <bullet/btBulletCollisionCommon.h>

struct elfList;
struct elfCollision;

class MultipleRayResultCallback : public btCollisionWorld::RayResultCallback
{
   public:
    MultipleRayResultCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld, elfList* list);

    btVector3 m_rayFromWorld;
    btVector3 m_rayToWorld;
    btVector3 m_hitNormalWorld;
    btVector3 m_hitPointWorld;
    elfList* m_list = nullptr;

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace);
};