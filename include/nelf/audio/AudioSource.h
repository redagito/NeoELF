#pragma once

#include <AL/al.h>

#include "nelf/objectHeader.h"

struct elfSound;
struct elfEntity;

struct elfAudioSource
{
    ELF_OBJECT_HEADER;
    elfSound* sound = nullptr;
    ALuint source;
    bool loop = false;
    bool paused = false;
};

elfAudioSource* elfPlaySound(elfSound* sound, float volume);
elfAudioSource* elfPlayEntitySound(elfEntity* entity, elfSound* sound, float volume);

elfAudioSource* elfLoopSound(elfSound* sound, float volume);
elfAudioSource* elfLoopEntitySound(elfEntity* entity, elfSound* sound, float volume);

elfAudioSource* elfCreateAudioSource();

void elfStreamAudioSource(elfAudioSource* source);

void elfDestroyAudioSource(void* data);

void elfSetSoundVolume(elfAudioSource* source, float volume);
float elfGetSoundVolume(elfAudioSource* source);

void elfPauseSound(elfAudioSource* source);
void elfResumeSound(elfAudioSource* source);

void elfStopSound(elfAudioSource* source);

unsigned char elfIsSoundPlaying(elfAudioSource* source);
unsigned char elfIsSoundPaused(elfAudioSource* source);

// TODO No getter?
void elfSetSoundPosition(elfAudioSource* source, float x, float y, float z);