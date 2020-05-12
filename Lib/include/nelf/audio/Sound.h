#pragma once

#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include <cstdio>

#include "nelf/objectHeader.h"

struct elfSound
{
    ELF_OBJECT_HEADER;
    char* filePath;
    unsigned char fileType;
    ALuint buffer[3];
    int freq;
    int format;

    bool streamed;
    bool streaming;
    bool eof;
    OggVorbis_File oggFile;
    FILE* file;
    int length;
    int position;
    unsigned char oldestBuffer;
};

elfSound* elfCreateSound();
void elfDestroySound(void* data);

bool elfInitSoundWithOgg(elfSound* snd, const char* filePath);
bool elfInitSoundWithWav(elfSound* snd, const char* filePath);

elfSound* elfLoadSound(const char* filePath);
elfSound* elfLoadStreamedSound(const char* filePath);

int elfGetSoundFileType(elfSound* sound);