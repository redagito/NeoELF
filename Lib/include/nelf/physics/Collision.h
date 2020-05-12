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

elfCollision* elfCreateCollision();
void elfDestroyCollision(void* data);

elfActor* elfGetCollisionActor(elfCollision* collision);
elfVec3f elfGetCollisionPosition(elfCollision* collision);
elfVec3f elfGetCollisionNormal(elfCollision* collision);
float elfGetCollisionDepth(elfCollision* collision);