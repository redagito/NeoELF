#pragma once

#include "nelf/Vector.h"
#include "nelf/resourceHeader.h"

struct elfScene;
struct gfxTransform;
struct elfList;
struct elfScript;
struct elfIpo;
struct elfFramePlayer;
struct elfPhysicsObject;

#define ELF_ACTOR_HEADER       \
    ELF_RESOURCE_HEADER;       \
    char* filePath;            \
    elfScene* scene;           \
    gfxTransform* transform;   \
    elfList* joints;           \
    elfScript* script;         \
    elfList* sources;          \
    elfIpo* ipo;               \
    elfFramePlayer* ipoPlayer; \
    elfList* properties;       \
    elfPhysicsObject* object;  \
    elfPhysicsObject* dobject; \
    bool physics;              \
    elfVec3f pbbLengths;       \
    elfVec3f pbbOffset;        \
    bool pbbOffsetSet;         \
    unsigned char shape;       \
    float mass;                \
    float linDamp;             \
    float angDamp;             \
    float linSleep;            \
    float angSleep;            \
    float restitution;         \
    elfVec3f anisFric;         \
    elfVec3f linFactor;        \
    elfVec3f angFactor;        \
    bool moved;                \
    unsigned char selected