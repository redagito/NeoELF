#include "nelf/Particle.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/objectType.h"

elfParticle* elfCreateParticle()
{
    elfParticle* particle;

    particle = (elfParticle*)malloc(sizeof(elfParticle));
    memset(particle, 0x0, sizeof(elfParticle));
    particle->objType = ELF_PARTICLE;
    particle->objDestr = elfDestroyParticle;

    elfIncObj(ELF_PARTICLE);

    return particle;
}

void elfDestroyParticle(void* data)
{
    elfParticle* particle = (elfParticle*)data;

    free(particle);

    elfDecObj(ELF_PARTICLE);
}