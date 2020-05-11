#pragma once

#include "nelf/actor/actorHeader.h"

struct elfFramePlayer;
struct elfScript;
struct elfJoint;
struct elfCollision;
struct elfIpo;
struct elfProperty;
struct gfxShaderParams;

struct elfActor
{
    ELF_ACTOR_HEADER;
};

void elfActorIpoCallback(elfFramePlayer* player);

void elfInitActor(elfActor* actor, bool camera);

void elfUpdateActor(elfActor* actor);

void elfActorPreDraw(elfActor* actor);
void elfActorPostDraw(elfActor* actor);

void elfCleanActor(elfActor* actor);

const char* elfGetActorName(elfActor* actor);
const char* elfGetActorFilePath(elfActor* actor);
elfScript* elfGetActorScript(elfActor* actor);

void elfSetActorName(elfActor* actor, const char* name);
void elfSetActorScript(elfActor* actor, elfScript* script);

void elfClearActorScript(elfActor* actor);

// Transformations
void elfSetActorPosition(elfActor* actor, float x, float y, float z);
void elfSetActorRotation(elfActor* actor, float x, float y, float z);
void elfSetActorOrientation(elfActor* actor, float x, float y, float z, float w);

void elfRotateActor(elfActor* actor, float x, float y, float z);
void elfRotateActorLocal(elfActor* actor, float x, float y, float z);

void elfMoveActor(elfActor* actor, float x, float y, float z);
void elfMoveActorLocal(elfActor* actor, float x, float y, float z);

void elfSetActorPositionRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z);
void elfSetActorRotationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z);
void elfSetActorOrientationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z, float w);

elfVec3f elfGetActorPosition(elfActor* actor);
elfVec3f elfGetActorRotation(elfActor* actor);
elfVec4f elfGetActorOrientation(elfActor* actor);

void elfGetActorPosition_(elfActor* actor, float* params);
void elfGetActorRotation_(elfActor* actor, float* params);
void elfGetActorOrientation_(elfActor* actor, float* params);

// Physics

void elfSetActorPhysics(elfActor* actor, bool physics);
void elfSetActorShape(elfActor* actor, int shape);

void elfSetActorBoundingLengths(elfActor* actor, float x, float y, float z);
void elfSetActorBoundingOffset(elfActor* actor, float x, float y, float z);
void elfResetActorBoundingOffsetSetFlag(elfActor* actor);

void elfSetActorMass(elfActor* actor, float mass);
void elfSetActorDamping(elfActor* actor, float linDamp, float angDamp);
void elfSetActorSleep(elfActor* actor, float linThrs, float angThrs);
void elfSetActorRestitution(elfActor* actor, float restitution);
void elfSetActorAnisotropicFriction(elfActor* actor, float x, float y, float z);
void elfSetActorLinearFactor(elfActor* actor, float x, float y, float z);
void elfSetActorAngularFactor(elfActor* actor, float x, float y, float z);

void elfAddActorForce(elfActor* actor, float x, float y, float z);
void elfAddActorForceLocal(elfActor* actor, float x, float y, float z);
void elfAddActorTorque(elfActor* actor, float x, float y, float z);

void elfSetActorLinearVelocity(elfActor* actor, float x, float y, float z);
void elfSetActorLinearVelocityLocal(elfActor* actor, float x, float y, float z);
void elfSetActorAngularVelocity(elfActor* actor, float x, float y, float z);

unsigned char elfGetActorPhysics(elfActor* actor);
int elfGetActorShape(elfActor* actor);

elfVec3f elfGetActorBoundingLengths(elfActor* actor);
elfVec3f elfGetActorBoundingOffset(elfActor* actor);

float elfGetActorMass(elfActor* actor);
float elfGetActorLinearDamping(elfActor* actor);
float elfGetActorAngularDamping(elfActor* actor);
float elfGetActorLinearSleep(elfActor* actor);
float elfGetActorAngularSleep(elfActor* actor);
float elfGetActorRestitution(elfActor* actor);

elfVec3f elfGetActorAnisotropicFriction(elfActor* actor);
elfVec3f elfGetActorLinearFactor(elfActor* actor);
elfVec3f elfGetActorAngularFactor(elfActor* actor);
elfVec3f elfGetActorLinearVelocity(elfActor* actor);
elfVec3f elfGetActorAngularVelocity(elfActor* actor);

/**
 * Joints
 */
elfJoint* elfAddActorHingeJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz,
                                float ax, float ay, float az);

elfJoint* elfAddActorBallJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz);

elfJoint* elfAddActorConeTwistJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz,
                                    float ax, float ay, float az);

elfJoint* elfGetActorJoint(elfActor* actor, const char* name);

elfJoint* elfGetActorJointByIndex(elfActor* actor, int idx);

bool elfRemoveActorJoint(elfActor* actor, const char* name);
bool elfRemoveActorJointByIndex(elfActor* actor, int idx);
bool elfRemoveActorJointByObject(elfActor* actor, elfJoint* joint);

// IPO

void elfSetActorIpo(elfActor* actor, elfIpo* ipo);
elfIpo* elfGetActorIpo(elfActor* actor);

void elfSetActorIpoFrame(elfActor* actor, float frame);

void elfPlayActorIpo(elfActor* actor, float start, float end, float speed);
void elfLoopActorIpo(elfActor* actor, float start, float end, float speed);
void elfStopActorIpo(elfActor* actor);
void elfPauseActorIpo(elfActor* actor);
void elfResumeActorIpo(elfActor* actor);

float elfGetActorIpoStart(elfActor* actor);
float elfGetActorIpoEnd(elfActor* actor);
float elfGetActorIpoSpeed(elfActor* actor);
float elfGetActorIpoFrame(elfActor* actor);

bool elfIsActorIpoPlaying(elfActor* actor);
bool elfIsActorIpoPaused(elfActor* actor);

int elfGetActorCollisionCount(elfActor* actor);
elfCollision* elfGetActorCollision(elfActor* actor, int idx);

// Property

int elfGetActorPropertyCount(elfActor* actor);
void elfAddPropertyToActor(elfActor* actor, elfProperty* property);

elfProperty* elfGetActorPropertyByName(elfActor* actor, const char* name);
elfProperty* elfGetActorPropertyByIndex(elfActor* actor, int idx);

bool elfRemoveActorPropertyByName(elfActor* actor, const char* name);
bool elfRemoveActorPropertyByIndex(elfActor* actor, int idx);
bool elfRemoveActorPropertyByObject(elfActor* actor, elfProperty* property);
void elfRemoveActorProperties(elfActor* actor);

// Select
void elfSetActorSelected(elfActor* actor, bool selected);
bool elfGetActorSelected(elfActor* actor);

// Debug draw
void elfDrawActorDebug(elfActor* actor, gfxShaderParams* shaderParams);