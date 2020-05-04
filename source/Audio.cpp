#include "nelf/Audio.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef ELF_MACOSX
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#else
#include <AL/alc.h>
#include <AL/al.h>
#endif

#include <vorbis/vorbisfile.h>

#include "blendelf.h"
#include "gfx.h"
#include "types.h"
