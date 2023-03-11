#include "nelf/physics/PhysicsObject.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/physics/PhysicsTriMesh.h"
#include "nelf/physics/PhysicsWorld.h"
#include "nelf/physics/shapeType.h"

elfPhysicsObject* elfCreatePhysicsObject()
{
    elfPhysicsObject* object;

    object = (elfPhysicsObject*)malloc(sizeof(elfPhysicsObject));
    memset(object, 0x0, sizeof(elfPhysicsObject));
    object->objType = ELF_PHYSICS_OBJECT;
    object->objDestr = elfDestroyPhysicsObject;

    object->collisions = elfCreateList();
    elfIncRef((elfObject*)object->collisions);

    elfIncObj(ELF_PHYSICS_OBJECT);

    return object;
}

elfPhysicsObject* elfCreatePhysicsObjectMesh(elfPhysicsTriMesh* triMesh, float mass)
{
    elfPhysicsObject* object;

    object = elfCreatePhysicsObject();

    object->shape = new btBvhTriangleMeshShape(triMesh->triMesh, true);

    object->shapeType = ELF_MESH;
    object->mass = mass;

    object->triMesh = triMesh;
    elfIncRef((elfObject*)triMesh);

    btScalar bodyMass(mass);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (!elfAboutZero(mass))
        object->shape->calculateLocalInertia(mass, localInertia);

    btTransform startTransform;
    startTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    object->motionState = new btDefaultMotionState(startTransform);
    object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

    object->body->setUserPointer(object);

    return object;
}

elfPhysicsObject* elfCreatePhysicsObjectSphere(float radius, float mass, float ox, float oy, float oz)
{
    elfPhysicsObject* object;

    object = elfCreatePhysicsObject();

    object->shape = new btSphereShape(btScalar(radius));
    object->shapeType = ELF_SPHERE;
    object->mass = mass;
    object->offset.x = ox;
    object->offset.y = oy;
    object->offset.z = oz;

    btScalar bodyMass(mass);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (!elfAboutZero(mass))
        object->shape->calculateLocalInertia(mass, localInertia);

    btTransform startTransform;
    startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    object->motionState = new btDefaultMotionState(startTransform);
    object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

    object->body->setUserPointer(object);

    return object;
}

elfPhysicsObject* elfCreatePhysicsObjectBox(float hx, float hy, float hz, float mass, float ox, float oy, float oz)
{
    elfPhysicsObject* object;

    object = elfCreatePhysicsObject();

    object->shape = new btBoxShape(btVector3(hx + 0.001f, hy + 0.001f, hz + 0.001f));
    object->shapeType = ELF_BOX;
    object->mass = mass;
    object->offset.x = ox;
    object->offset.y = oy;
    object->offset.z = oz;

    btScalar bodyMass(mass);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (!elfAboutZero(mass))
        object->shape->calculateLocalInertia(mass, localInertia);

    btTransform startTransform;
    startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    object->motionState = new btDefaultMotionState(startTransform);
    object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

    object->body->setUserPointer(object);

    return object;
}

elfPhysicsObject* elfCreatePhysicsObjectCapsule(unsigned char type, float length, float radius, float mass, float ox,
                                                float oy, float oz)
{
    elfPhysicsObject* object;

    object = elfCreatePhysicsObject();

    if (type == ELF_CAPSULE_X)
        object->shape = new btCapsuleShapeX(radius, length);
    else if (type == ELF_CAPSULE_Y)
        object->shape = new btCapsuleShape(radius, length);
    else if (type == ELF_CAPSULE_Z)
        object->shape = new btCapsuleShapeZ(radius, length);

    object->shapeType = type;
    object->mass = mass;
    object->offset.x = ox;
    object->offset.y = oy;
    object->offset.z = oz;

    btScalar bodyMass(mass);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (!elfAboutZero(mass))
        object->shape->calculateLocalInertia(mass, localInertia);

    btTransform startTransform;
    startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    object->motionState = new btDefaultMotionState(startTransform);
    object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

    object->body->setUserPointer(object);

    return object;
}

elfPhysicsObject* elfCreatePhysicsObjectCone(unsigned char type, float length, float radius, float mass, float ox,
                                             float oy, float oz)
{
    elfPhysicsObject* object;

    object = elfCreatePhysicsObject();

    if (type == ELF_CONE_X)
        object->shape = new btConeShapeX(radius, length);
    else if (type == ELF_CONE_Y)
        object->shape = new btConeShape(radius, length);
    else if (type == ELF_CONE_Z)
        object->shape = new btConeShapeZ(radius, length);

    object->shapeType = type;
    object->mass = mass;
    object->offset.x = ox;
    object->offset.y = oy;
    object->offset.z = oz;

    btScalar bodyMass(mass);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (!elfAboutZero(mass))
        object->shape->calculateLocalInertia(mass, localInertia);

    btTransform startTransform;
    startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    object->motionState = new btDefaultMotionState(startTransform);
    object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

    object->body->setUserPointer(object);

    return object;
}

void elfSetPhysicsObjectWorld(elfPhysicsObject* object, elfPhysicsWorld* world)
{
    if (object->world)
    {
        object->world->world->removeRigidBody(object->body);
        object->world = nullptr;
    }
    object->world = world;
    if (object->world)
    {
        object->world->world->addRigidBody(object->body);
        object->body->activate(true);
    }
}

void elfDestroyPhysicsObject(void* data)
{
    elfPhysicsObject* object = (elfPhysicsObject*)data;

    if (object->body)
    {
        if (object->world)
            object->world->world->removeRigidBody(object->body);
        delete object->body;
    }
    if (object->shape)
        delete object->shape;
    if (object->motionState)
        delete object->motionState;
    if (object->triMesh)
        elfDecRef((elfObject*)object->triMesh);
    elfDecRef((elfObject*)object->collisions);

    free(object);

    elfDecObj(ELF_PHYSICS_OBJECT);
}

void elfSetPhysicsObjectActor(elfPhysicsObject* object, elfActor* actor) { object->actor = actor; }

void elfRemovePhysicsObjectCollisions(elfPhysicsObject* object)
{
    if (elfGetListLength(object->collisions) > 0)
    {
        elfDecRef((elfObject*)object->collisions);
        object->collisions = elfCreateList();
        elfIncRef((elfObject*)object->collisions);
    }
    object->collisionCount = 0;
}

void elfClearPhysicsObjectCollisions(elfPhysicsObject* object)
{
    if (elfGetListLength(object->collisions) > 0)
        elfRemoveListObject(object->collisions, elfBeginList(object->collisions));
    object->collisionCount = 0;
}

int elfGetPhysicsObjectCollisionCount(elfPhysicsObject* object) { return elfGetListLength(object->collisions); }

elfCollision* elfGetPhysicsObjectCollision(elfPhysicsObject* object, int idx)
{
    if (idx < 0 || idx > elfGetListLength(object->collisions) - 1)
        return NULL;

    return (elfCollision*)elfGetListObject(object->collisions, idx);
}

void elfSetPhysicsObjectPosition(elfPhysicsObject* object, float x, float y, float z)
{
    float orient[4];
    float offset[3];

    elfGetPhysicsObjectOrientation(object, orient);

    gfxMulQuaVec(orient, &object->offset.x, offset);

    object->body->activate(true);
    if (object->body->isStaticObject())
    {
        object->body->setCollisionFlags(object->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }
    btTransform xform = object->body->getCenterOfMassTransform();
    xform.setOrigin(btVector3(x + offset[0], y + offset[1], z + offset[2]));
    object->body->setCenterOfMassTransform(xform);
    object->motionState->setWorldTransform(xform);
}

void elfSetPhysicsObjectOrientation(elfPhysicsObject* object, float x, float y, float z, float w)
{
    float orient[4];
    float offset[3];
    float newPos[3];
    btVector3 origin;

    elfGetPhysicsObjectOrientation(object, orient);

    gfxMulQuaVec(orient, &object->offset.x, offset);

    origin = object->motionState->m_graphicsWorldTrans.getOrigin();

    newPos[0] = origin.x() - offset[0];
    newPos[1] = origin.y() - offset[1];
    newPos[2] = origin.z() - offset[2];

    orient[0] = x;
    orient[1] = y;
    orient[2] = z;
    orient[3] = w;

    gfxMulQuaVec(orient, &object->offset.x, offset);

    newPos[0] += offset[0];
    newPos[1] += offset[1];
    newPos[2] += offset[2];

    object->body->activate(true);
    if (object->body->isStaticObject())
    {
        object->body->setCollisionFlags(object->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }
    btTransform xform = object->body->getCenterOfMassTransform();
    xform.setOrigin(btVector3(newPos[0], newPos[1], newPos[2]));
    xform.setRotation(btQuaternion(x, y, z, w));
    object->body->setCenterOfMassTransform(xform);
    object->motionState->setWorldTransform(xform);
}

void elfSetPhysicsObjectScale(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->shape->setLocalScaling(btVector3(x, y, z));
}

int elfGetPhysicsObjectShape(elfPhysicsObject* object) { return object->shapeType; }

float elfGetPhysicsObjectMass(elfPhysicsObject* object) { return object->mass; }

void elfGetPhysicsObjectPosition(elfPhysicsObject* object, float* params)
{
    float orient[4];
    float offset[3];
    btVector3 origin;

    elfGetPhysicsObjectOrientation(object, orient);

    gfxMulQuaVec(orient, &object->offset.x, offset);

    origin = object->motionState->m_graphicsWorldTrans.getOrigin();
    params[0] = origin.x() - offset[0];
    params[1] = origin.y() - offset[1];
    params[2] = origin.z() - offset[2];
}

void elfGetPhysicsObjectOrientation(elfPhysicsObject* object, float* params)
{
    btQuaternion orient = object->motionState->m_graphicsWorldTrans.getRotation();
    params[0] = orient.x();
    params[1] = orient.y();
    params[2] = orient.z();
    params[3] = orient.w();
}

void elfGetPhysicsObjectScale(elfPhysicsObject* object, float* params)
{
    btVector3 origin = object->shape->getLocalScaling();
    params[0] = origin.x();
    params[1] = origin.y();
    params[2] = origin.z();
}

unsigned char elfIsPhysicsObjectStatic(elfPhysicsObject* object) { return object->body->isStaticObject(); }

void elfSetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->setAnisotropicFriction(btVector3(x, y, z));
}

void elfSetPhysicsObjectDamping(elfPhysicsObject* object, float linDamp, float angDamp)
{
    object->body->setDamping(linDamp, angDamp);
}

void elfSetPhysicsObjectSleepThresholds(elfPhysicsObject* object, float linThrs, float angThrs)
{
    object->body->setSleepingThresholds(linThrs, angThrs);
}

void elfSetPhysicsObjectRestitution(elfPhysicsObject* object, float restitution)
{
    object->body->setRestitution(restitution);
}

void elfAddPhysicsObjectForce(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->applyCentralForce(btVector3(x, y, z));
}

void elfAddPhysicsObjectTorque(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->applyTorque(btVector3(x, y, z));
}

void elfSetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->setLinearVelocity(btVector3(x, y, z));
}

void elfSetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->setAngularVelocity(btVector3(x, y, z));
}

void elfSetPhysicsObjectLinearFactor(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->setLinearFactor(btVector3(x, y, z));
}

void elfSetPhysicsObjectAngularFactor(elfPhysicsObject* object, float x, float y, float z)
{
    object->body->activate(true);
    object->body->setAngularFactor(btVector3(x, y, z));
}

void elfGetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float* params)
{
    btVector3 vec;
    vec = object->body->getLinearVelocity();
    params[0] = vec.x();
    params[1] = vec.y();
    params[2] = vec.z();
}

void elfGetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float* params)
{
    btVector3 vec;
    vec = object->body->getAngularVelocity();
    params[0] = vec.x();
    params[1] = vec.y();
    params[2] = vec.z();
}

void elfGetPhysicsObjectLinearFactor(elfPhysicsObject* object, float* params)
{
    btVector3 vec;
    vec = object->body->getLinearFactor();
    params[0] = vec.x();
    params[1] = vec.y();
    params[2] = vec.z();
}

void elfGetPhysicsObjectAngularFactor(elfPhysicsObject* object, float* params)
{
    btVector3 vec;
    vec = object->body->getAngularFactor();
    params[0] = vec.x();
    params[1] = vec.y();
    params[2] = vec.z();
}

void elfGetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float* params)
{
    btVector3 vec;
    vec = object->body->getAnisotropicFriction();
    params[0] = vec.x();
    params[1] = vec.y();
    params[2] = vec.z();
}

float elfGetPhysicsObjectLinearDamping(elfPhysicsObject* object) { return object->body->getLinearDamping(); }

float elfGetPhysicsObjectAngularDamping(elfPhysicsObject* object) { return object->body->getAngularDamping(); }

float elfGetPhysicsObjectLinearSleepThreshold(elfPhysicsObject* object)
{
    return object->body->getLinearSleepingThreshold();
}

float elfGetPhysicsObjectAngularSleepThreshold(elfPhysicsObject* object)
{
    return object->body->getAngularSleepingThreshold();
}

float elfGetPhysicsObjectRestitution(elfPhysicsObject* object) { return object->body->getRestitution(); }
