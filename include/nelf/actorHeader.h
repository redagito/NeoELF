#pragma once

#include "nelf/Vector.h"
#include "nelf/resourceHeader.h"

struct elfScene;
struct elfList;
struct elfScript;
struct elfIpo;
struct elfFramePlayer;
struct elfPhysicsObject;
struct gfxTransform;

// TODO Why does an actor need to know its scene
#define ELF_ACTOR_HEADER                 \
    ELF_RESOURCE_HEADER;                 \
    char* filePath = nullptr;            \
    elfScene* scene = nullptr;           \
    gfxTransform* transform = nullptr;   \
    elfList* joints = nullptr;           \
    elfScript* script = nullptr;         \
    elfList* sources = nullptr;          \
    elfIpo* ipo = nullptr;               \
    elfFramePlayer* ipoPlayer = nullptr; \
    elfList* properties = nullptr;       \
    elfPhysicsObject* object = nullptr;  \
    elfPhysicsObject* dobject = nullptr; \
    bool physics = false;                \
    elfVec3f pbbLengths;                 \
    elfVec3f pbbOffset;                  \
    bool pbbOffsetSet = false;           \
    unsigned char shape = 0;             \
    float mass = 0.f;                    \
    float linDamp = 0.f;                 \
    float angDamp = 0.f;                 \
    float linSleep = 0.f;                \
    float angSleep = 0.f;                \
    float restitution = 0.f;             \
    elfVec3f anisFric;                   \
    elfVec3f linFactor;                  \
    elfVec3f angFactor;                  \
    bool moved = 0.f;                    \
    bool selected = false