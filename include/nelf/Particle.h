#pragma once

#include "nelf/Color.h"
#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfParticle
{
    ELF_OBJECT_HEADER;

    float size = 0.f;
    float sizeGrowth = 0.f;

    float rotation = 0.f;
    float rotationGrowth = 0.f;

    elfColor color;

    float lifeSpan = 0.f;
    float fadeSpeed = 0.f;

    elfVec3f position;
    elfVec3f velocity;
};

elfParticle* elfCreateParticle();
void elfDestroyParticle(void* data);