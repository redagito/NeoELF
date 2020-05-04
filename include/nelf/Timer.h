#pragma once

#include "nelf/objectHeader.h"

struct elfTimer
{
    ELF_OBJECT_HEADER;
    double start = 0.0;
};

elfTimer* elfCreateTimer();

void elfDestroyTimer(void* data);

void elfStartTimer(elfTimer* timer);

double elfGetElapsedTime(elfTimer* timer);