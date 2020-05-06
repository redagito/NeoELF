#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfActor;

struct elfCollision
{
    ELF_OBJECT_HEADER;
    elfActor* actor = nullptr;
    elfVec3f position;
    elfVec3f normal;
    float depth = 0.f;
};

elfCollision* elfCreateCollision()
{
    elfCollision* collision;

    collision = (elfCollision*)malloc(sizeof(elfCollision));
    memset(collision, 0x0, sizeof(elfCollision));
    collision->objType = ELF_COLLISION;
    collision->objDestr = elfDestroyCollision;

    elfIncObj(ELF_COLLISION);

    return collision;
}

void elfDestroyCollision(void* data)
{
    elfCollision* collision = (elfCollision*)data;

    if (collision->actor)
        elfDecRef((elfObject*)collision->actor);

    free(collision);

    elfDecObj(ELF_COLLISION);
}

elfActor* elfGetCollisionActor(elfCollision* collision) { return collision->actor; }

elfVec3f elfGetCollisionPosition(elfCollision* collision) { return collision->position; }

elfVec3f elfGetCollisionNormal(elfCollision* collision) { return collision->normal; }

float elfGetCollisionDepth(elfCollision* collision) { return collision->depth; }