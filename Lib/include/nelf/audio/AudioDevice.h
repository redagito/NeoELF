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

elfAudioDevice* elfCreateAudioDevice();

void elfDestroyAudioDevice(void* data);

bool elfInitAudio();
void elfDeinitAudio();
void elfUpdateAudio();

void elfSetAudioVolume(float volume);
float elfGetAudioVolume();

void elfSetAudioRolloff(float rolloff);
float elfGetAudioRolloff();

void elfSetAudioListenerPosition(float x, float y, float z);
void elfSetAudioListenerOrientation(float* params);