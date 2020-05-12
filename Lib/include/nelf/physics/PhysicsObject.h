#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfActor;
struct elfList;
struct elfPhysicsWorld;
struct elfPhysicsTriMesh;
struct elfCollision;
struct btCollisionShape;
struct btRigidBody;
struct btDefaultMotionState;

struct elfPhysicsObject
{
    ELF_OBJECT_HEADER;
    int shapeType;
    float mass = 0.f;
    elfVec3f offset;
    btCollisionShape* shape = nullptr;
    btRigidBody* body = nullptr;
    btDefaultMotionState* motionState = nullptr;
    elfPhysicsTriMesh* triMesh = nullptr;
    elfPhysicsWorld* world = nullptr;
    elfList* collisions = nullptr;
    int collisionCount = 0;
    elfActor* actor = nullptr;
};

elfPhysicsObject* elfCreatePhysicsObject();
elfPhysicsObject* elfCreatePhysicsObjectMesh(elfPhysicsTriMesh* triMesh, float mass);
elfPhysicsObject* elfCreatePhysicsObjectSphere(float radius, float mass, float ox, float oy, float oz);
elfPhysicsObject* elfCreatePhysicsObjectBox(float hx, float hy, float hz, float mass, float ox, float oy, float oz);
elfPhysicsObject* elfCreatePhysicsObjectCapsule(unsigned char type, float length, float radius, float mass, float ox,
                                                float oy, float oz);
elfPhysicsObject* elfCreatePhysicsObjectCone(unsigned char type, float length, float radius, float mass, float ox,
                                             float oy, float oz);

void elfSetPhysicsObjectWorld(elfPhysicsObject* object, elfPhysicsWorld* world);

void elfDestroyPhysicsObject(void* data);

void elfSetPhysicsObjectActor(elfPhysicsObject* object, elfActor* actor);

void elfRemovePhysicsObjectCollisions(elfPhysicsObject* object);

void elfClearPhysicsObjectCollisions(elfPhysicsObject* object);

int elfGetPhysicsObjectCollisionCount(elfPhysicsObject* object);
elfCollision* elfGetPhysicsObjectCollision(elfPhysicsObject* object, int idx);

void elfSetPhysicsObjectPosition(elfPhysicsObject* object, float x, float y, float z);
void elfSetPhysicsObjectOrientation(elfPhysicsObject* object, float x, float y, float z, float w);
void elfSetPhysicsObjectScale(elfPhysicsObject* object, float x, float y, float z);

int elfGetPhysicsObjectShape(elfPhysicsObject* object);
float elfGetPhysicsObjectMass(elfPhysicsObject* object);

void elfGetPhysicsObjectPosition(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectOrientation(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectScale(elfPhysicsObject* object, float* params);

unsigned char elfIsPhysicsObjectStatic(elfPhysicsObject* object);

void elfSetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float x, float y, float z);
void elfSetPhysicsObjectDamping(elfPhysicsObject* object, float linDamp, float angDamp);
void elfSetPhysicsObjectSleepThresholds(elfPhysicsObject* object, float linThrs, float angThrs);
void elfSetPhysicsObjectRestitution(elfPhysicsObject* object, float restitution);

void elfAddPhysicsObjectForce(elfPhysicsObject* object, float x, float y, float z);
void elfAddPhysicsObjectTorque(elfPhysicsObject* object, float x, float y, float z);

void elfSetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float x, float y, float z);
void elfSetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float x, float y, float z);
void elfSetPhysicsObjectLinearFactor(elfPhysicsObject* object, float x, float y, float z);
void elfSetPhysicsObjectAngularFactor(elfPhysicsObject* object, float x, float y, float z);

void elfGetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectLinearFactor(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectAngularFactor(elfPhysicsObject* object, float* params);
void elfGetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float* params);

float elfGetPhysicsObjectLinearDamping(elfPhysicsObject* object);
float elfGetPhysicsObjectAngularDamping(elfPhysicsObject* object);
float elfGetPhysicsObjectLinearSleepThreshold(elfPhysicsObject* object);
float elfGetPhysicsObjectAngularSleepThreshold(elfPhysicsObject* object);
float elfGetPhysicsObjectRestitution(elfPhysicsObject* object);