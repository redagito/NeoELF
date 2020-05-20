#pragma once

#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include <cstdio>

#include "nelf/objectHeader.h"

struct elfSound
{
    ELF_OBJECT_HEADER;
    char* filePath = nullptr;
    unsigned char fileType;
    ALuint buffer[3];
    int freq;
    int format;

    // Sound is streamed from disk
    // If false, the whole sound file is already loaded
    bool streamed = false;

    bool streaming;
    bool eof;
    OggVorbis_File oggFile;
    FILE* file;
    int length;
    int position;
    unsigned char oldestBuffer;
};

// Constuctor, destructor
elfSound* elfCreateSound();
void elfDestroySound(void* data);

// Used by AudioSource
bool elfInitSoundWithOgg(elfSound* snd, const char* filePath);
bool elfInitSoundWithWav(elfSound* snd, const char* filePath);

// Lua API
// For small sound files, loads everything into memory
elfSound* elfLoadSound(const char* filePath);
// Sets the sound to be streamed form the disk
elfSound* elfLoadStreamedSound(const char* filePath);
// SoundFileType
int elfGetSoundFileType(elfSound* sound);