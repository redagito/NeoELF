#include "nelf/audio/AudioDevice.h"

#include <cstdlib>
#include <cstring>

// OpenAL
#include <AL/al.h>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Object.h"
#include "nelf/audio/AudioSource.h"
#include "nelf/audio/Sound.h"

elfAudioDevice* audioDevice = nullptr;

elfAudioDevice* elfCreateAudioDevice()
{
    elfAudioDevice* device;

    device = (elfAudioDevice*)malloc(sizeof(elfAudioDevice));
    memset(device, 0x0, sizeof(elfAudioDevice));
    device->objType = ELF_AUDIO_DEVICE;
    device->objDestr = elfDestroyAudioDevice;

    device->volume = 1.0f;
    device->rolloff = 1.0f;

    device->sources = elfCreateList();
    elfIncRef((elfObject*)device->sources);

    elfIncObj(ELF_AUDIO_DEVICE);

    return device;
}

void elfDestroyAudioDevice(void* data)
{
    elfAudioDevice* device = (elfAudioDevice*)data;

    if (device->sources)
        elfDecRef((elfObject*)device->sources);

    if (device->device)
    {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(device->context);
        alcCloseDevice(device->device);
    }

    free(device);

    elfDecObj(ELF_AUDIO_DEVICE);
}

bool elfInitAudio()
{
    if (audioDevice)
    {
        elfLogWrite("warning: cannot initialize audio twice\n");
        return true;
    }

    audioDevice = elfCreateAudioDevice();
    elfIncRef((elfObject*)audioDevice);

    // Device
    audioDevice->device = alcOpenDevice(NULL);
    if (audioDevice->device == nullptr)
    {
        elfLogWrite("warning: could not create audio device\n");
        elfDeinitAudio();
        return false;
    }

    // Context
    audioDevice->context = alcCreateContext(audioDevice->device, nullptr);
    if (!audioDevice->context)
    {
        elfLogWrite("warning: could not create audio context\n");
        alcCloseDevice(audioDevice->device);
        audioDevice->device = nullptr;
        elfDeinitAudio();
        return false;
    }

    // Make current (for multi threading?)
    if (!alcMakeContextCurrent(audioDevice->context))
    {
        elfLogWrite("warning: could not make audio context current\n");
        alcCloseDevice(audioDevice->device);
        audioDevice->device = nullptr;
        elfDeinitAudio();
        return false;
    }

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    // 2x 3d vectors
    // 1. is at
    // 2. is up
    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, listenerOri);
    alListenerf(AL_GAIN, 1.0f);

    return alGetError() == AL_NO_ERROR;
}

void elfDeinitAudio()
{
    if (!audioDevice)
        return;

    elfDecRef((elfObject*)audioDevice);

    audioDevice = nullptr;
}

void elfUpdateAudio()
{
    elfAudioSource* source = nullptr;

    for (source = (elfAudioSource*)elfBeginList(audioDevice->sources); source;
         source = (elfAudioSource*)elfGetListNext(audioDevice->sources))
    {
        if (source->sound->streamed && !source->paused)
        {
            elfStreamAudioSource(source);
        }

        if (elfGetObjectRefCount((elfObject*)source) < 2 && !elfIsSoundPlaying(source) && !elfIsSoundPaused(source))
        {
            if (source->sound->streamed)
            {
                if (source->sound->eof && !source->loop)
                    elfRemoveListObject(audioDevice->sources, (elfObject*)source);
            }
            else
            {
                elfRemoveListObject(audioDevice->sources, (elfObject*)source);
            }
        }
    }
}

void elfSetAudioVolume(float volume)
{
    if (!audioDevice)
        return;
    alListenerf(AL_GAIN, volume);
}

float elfGetAudioVolume()
{
    if (!audioDevice)
        return 0.0f;
    return audioDevice->volume;
}

void elfSetAudioRolloff(float rolloff)
{
    if (!audioDevice)
        return;
    audioDevice->rolloff = rolloff;
}

float elfGetAudioRolloff()
{
    if (!audioDevice)
        return 0.0f;
    return audioDevice->rolloff;
}

void elfSetAudioListenerPosition(float x, float y, float z)
{
    if (!audioDevice)
        return;
    alListener3f(AL_POSITION, x, y, z);
}

void elfSetAudioListenerOrientation(float* params)
{
    if (!audioDevice)
        return;
    alListenerfv(AL_ORIENTATION, params);
}