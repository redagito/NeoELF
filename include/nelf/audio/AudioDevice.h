#pragma once

#include "nelf/objectHeader.h"

struct ALCdevice;
struct ALCcontext;
struct elfList;

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
elfAudioDevice* audioDevice;

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