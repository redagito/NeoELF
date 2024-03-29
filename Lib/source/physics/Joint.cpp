#include "nelf/physics/Joint.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/objectType.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/physics/PhysicsWorld.h"
#include "nelf/physics/jointType.h"
#include "nelf/resource/Scene.h"

elfJoint* elfCreateJoint()
{
    elfJoint* joint;

    joint = (elfJoint*)malloc(sizeof(elfJoint));
    memset(joint, 0x0, sizeof(elfJoint));
    joint->objType = ELF_JOINT;
    joint->objDestr = elfDestroyJoint;

    elfIncObj(ELF_JOINT);

    return joint;
}

void elfRecalcJoint(elfJoint* joint)
{
    elfVec3f pos1;
    elfVec3f pos2;
    elfVec4f qua1;
    elfVec4f qua2;
    elfVec4f aqua1;
    elfVec4f aqua2;
    elfVec3f worldCoordPivot1;
    elfVec3f localCoordPivot2;
    elfVec3f localAxis1;
    elfVec3f localAxis2;
    float matrix1[16];
    float matrix2[16];
    float matrix3[16];

    pos1 = elfGetActorPosition(joint->actor1);
    pos2 = elfGetActorPosition(joint->actor2);
    qua1 = elfGetActorOrientation(joint->actor1);
    qua2 = elfGetActorOrientation(joint->actor2);

    worldCoordPivot1.x = joint->pivot[0];
    worldCoordPivot1.y = joint->pivot[1];
    worldCoordPivot1.z = joint->pivot[2];

    worldCoordPivot1 = elfAddVec3fVec3f(elfMulQuaVec3f(qua1, worldCoordPivot1), pos1);
    localCoordPivot2 = elfMulQuaVec3f(elfGetQuaInverted(qua2), elfSubVec3fVec3f(worldCoordPivot1, pos2));

    btVector3 pivotInA(joint->pivot[0], joint->pivot[1], joint->pivot[2]);
    btVector3 pivotInB(localCoordPivot2.x, localCoordPivot2.y, localCoordPivot2.z);

    aqua1 = elfCreateQuaFromEuler(joint->axis[0], joint->axis[1], joint->axis[2]);
    localAxis1 = elfMulQuaVec3f(aqua1, elfCreateVec3f(0.0f, 0.0f, 1.0f));

    aqua2 = elfMulQuaQua(aqua1, elfMulQuaQua(qua1, elfGetQuaInverted(qua2)));
    localAxis2 = elfMulQuaVec3f(aqua2, elfCreateVec3f(0.0f, 0.0f, 1.0f));

    btVector3 axisInA(localAxis1.x, localAxis1.y, localAxis1.z);
    btVector3 axisInB(localAxis2.x, localAxis2.y, localAxis2.z);

    if (joint->jointType == ELF_HINGE)
    {
        joint->constraint = new btHingeConstraint(*joint->actor1->object->body, *joint->actor2->object->body, pivotInA,
                                                  pivotInB, axisInA, axisInB);
        joint->actor1->scene->world->world->addConstraint(joint->constraint);
    }
    else if (joint->jointType == ELF_BALL)
    {
        joint->constraint =
            new btPoint2PointConstraint(*joint->actor1->object->body, *joint->actor2->object->body, pivotInA, pivotInB);
        joint->actor1->scene->world->world->addConstraint(joint->constraint);
    }
    else if (joint->jointType == ELF_CONE_TWIST)
    {
        gfxMatrix4SetIdentity(matrix1);
        matrix1[12] = joint->pivot[0];
        matrix1[13] = joint->pivot[1];
        matrix1[14] = joint->pivot[2];
        gfxQuaToMatrix4(&aqua1.x, matrix2);
        gfxMulMatrix4Matrix4(matrix1, matrix2, matrix3);
        btTransform frameInA;
        frameInA.setFromOpenGLMatrix(matrix3);

        gfxMatrix4SetIdentity(matrix1);
        matrix1[12] = localCoordPivot2.x;
        matrix1[13] = localCoordPivot2.y;
        matrix1[14] = localCoordPivot2.z;
        gfxQuaToMatrix4(&aqua2.x, matrix2);
        gfxMulMatrix4Matrix4(matrix1, matrix2, matrix3);
        btTransform frameInB;
        frameInB.setFromOpenGLMatrix(matrix3);

        joint->constraint =
            new btConeTwistConstraint(*joint->actor1->object->body, *joint->actor2->object->body, frameInA, frameInB);
        joint->actor1->scene->world->world->addConstraint(joint->constraint);
    }
}

void elfSetJointWorld(elfJoint* joint, elfPhysicsWorld* world)
{
    if (joint->world && joint->constraint)
    {
        joint->world->world->removeConstraint(joint->constraint);
        delete joint->constraint;
        joint->constraint = nullptr;
    }
    joint->world = world;
    if (joint->world)
    {
        if (joint->world != joint->actor1->scene->world || joint->world != joint->actor2->scene->world)
        {
            joint->world = nullptr;
            return;
        }
        elfRecalcJoint(joint);
    }
}

elfJoint* elfCreateHingeJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                              float ax, float ay, float az)
{
    elfJoint* joint;

    joint = elfCreateJoint();
    joint->jointType = ELF_HINGE;
    if (name)
        joint->name = elfCreateString(name);

    joint->actor1 = actor1;
    joint->actor2 = actor2;

    joint->pivot[0] = px;
    joint->pivot[1] = py;
    joint->pivot[2] = pz;

    joint->axis[0] = ax;
    joint->axis[1] = ay;
    joint->axis[2] = az;

    if (actor1->scene)
        elfSetJointWorld(joint, actor1->scene->world);

    return joint;
}

elfJoint* elfCreateBallJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz)
{
    elfJoint* joint;

    joint = elfCreateJoint();
    joint->jointType = ELF_BALL;
    if (name)
        joint->name = elfCreateString(name);

    joint->actor1 = actor1;
    joint->actor2 = actor2;

    joint->pivot[0] = px;
    joint->pivot[1] = py;
    joint->pivot[2] = pz;

    if (actor1->scene)
        elfSetJointWorld(joint, actor1->scene->world);

    return joint;
}

elfJoint* elfCreateConeTwistJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                                  float ax, float ay, float az)
{
    elfJoint* joint;

    joint = elfCreateJoint();
    joint->jointType = ELF_BALL;
    if (name)
        joint->name = elfCreateString(name);

    joint->actor1 = actor1;
    joint->actor2 = actor2;

    joint->pivot[0] = px;
    joint->pivot[1] = py;
    joint->pivot[2] = pz;

    if (actor1->scene)
        elfSetJointWorld(joint, actor1->scene->world);

    return joint;
}

void elfClearJoint(elfJoint* joint)
{
    if (joint->constraint)
    {
        if (joint->world)
            joint->world->world->removeConstraint(joint->constraint);
        delete joint->constraint;
        joint->constraint = nullptr;
    }
    joint->actor1 = nullptr;
    joint->actor2 = nullptr;
    joint->world = nullptr;
}

void elfDestroyJoint(void* data)
{
    elfJoint* joint = (elfJoint*)data;

    if (joint->name)
        elfDestroyString(joint->name);
    if (joint->constraint)
    {
        if (joint->world)
            joint->world->world->removeConstraint(joint->constraint);
        delete joint->constraint;
    }
    free(joint);

    elfDecObj(ELF_JOINT);
}

const char* elfGetJointName(elfJoint* joint) { return joint->name; }

int elfGetJointType(elfJoint* joint) { return joint->jointType; }

elfActor* elfGetJointActorA(elfJoint* joint) { return joint->actor1; }

elfActor* elfGetJointActorB(elfJoint* joint) { return joint->actor2; }

elfVec3f elfGetJointPivot(elfJoint* joint)
{
    elfVec3f result;

    result.x = joint->pivot[0];
    result.y = joint->pivot[1];
    result.z = joint->pivot[2];

    return result;
}

elfVec3f elfGetJointAxis(elfJoint* joint)
{
    elfVec3f result;

    result.x = joint->axis[0];
    result.y = joint->axis[1];
    result.z = joint->axis[2];

    return result;
}