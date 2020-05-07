#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfActor;
struct elfPhysicsWorld;
struct btTypedConstraint;

struct elfJoint
{
    ELF_OBJECT_HEADER;
    char* name = nullptr;
    unsigned char jointType;
    elfActor* actor1 = nullptr;
    elfActor* actor2 = nullptr;
    btTypedConstraint* constraint = nullptr;
    elfPhysicsWorld* world = nullptr;
    float pivot[3];
    float axis[3];
};

elfJoint* elfCreateJoint();

void elfRecalcJoint(elfJoint* joint);

void elfSetJointWorld(elfJoint* joint, elfPhysicsWorld* world);

elfJoint* elfCreateHingeJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                              float ax, float ay, float az);
elfJoint* elfCreateBallJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz);
elfJoint* elfCreateConeTwistJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                                  float ax, float ay, float az);

void elfClearJoint(elfJoint* joint);

void elfDestroyJoint(void* data);

const char* elfGetJointName(elfJoint* joint);

int elfGetJointType(elfJoint* joint);
elfActor* elfGetJointActorA(elfJoint* joint);
elfActor* elfGetJointActorB(elfJoint* joint);
elfVec3f elfGetJointPivot(elfJoint* joint);
elfVec3f elfGetJointAxis(elfJoint* joint);