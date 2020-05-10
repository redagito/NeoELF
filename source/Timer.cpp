#include "nelf/Timer.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Context.h"
#include "nelf/General.h"
#include "nelf/objectType.h"

elfTimer* elfCreateTimer()
{
    elfTimer* timer;

    timer = (elfTimer*)malloc(sizeof(elfTimer));
    memset(timer, 0x0, sizeof(elfTimer));
    timer->objType = ELF_TIMER;
    timer->objDestr = elfDestroyTimer;

    elfIncObj(ELF_TIMER);

    return timer;
}

void elfDestroyTimer(void* data)
{
    elfTimer* timer = (elfTimer*)data;

    free(timer);

    elfDecObj(ELF_TIMER);
}

void elfStartTimer(elfTimer* timer) { timer->start = elfGetTime(); }

double elfGetElapsedTime(elfTimer* timer)
{
    if (!timer->start)
        return 0.0;
    return elfGetTime() - timer->start;
}