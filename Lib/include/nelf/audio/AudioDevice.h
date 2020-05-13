#pragma once

#include <AL/alc.h>

#include "nelf/objectHeader.h"

struct elfList;
struct elfAudioDevice;

struct elfAudioDevice
{
    ELF_OBJECT_HEADER;
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    float volume = 0.f;
    float rolloff = 0.f;
    elfList* sources = nullptr;
};

// Global audio device
extern elfAudioDevice* audioDevice;

// Constructor, destructor
elfAudioDevice* elfCreateAudioDevice();
void elfDestroyAudioDevice(void* data);

// Manage global audio device
bool elfInitAudio();
void elfDeinitAudio();
void elfUpdateAudio();

// LUA Api
void elfSetAudioVolume(float volume);
float elfGetAudioVolume();

void elfSetAudioRolloff(float rolloff);
float elfGetAudioRolloff();

// Listener position in world space
void elfSetAudioListenerPosition(float x, float y, float z);
// Listener orientattion
// float[6], 2x 3d vectors
// first is the lookat direction, default (0, 0, 1)
// second the up vector, default (0, 1, 0)
void elfSetAudioListenerOrientation(float* params);