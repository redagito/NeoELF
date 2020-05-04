#include "nelf/audio/AudioDevice.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Object.h"

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

    audioDevice->device = alcOpenDevice(NULL);
    if (!audioDevice->device)
    {
        elfLogWrite("warning: could not create audio device\n");
        elfDeinitAudio();
        return false;
    }

    audioDevice->context = alcCreateContext(audioDevice->device, NULL);
    if (!audioDevice->context)
    {
        elfLogWrite("warning: could not create audio context\n");
        alcCloseDevice(audioDevice->device);
        audioDevice->device = NULL;
        elfDeinitAudio();
        return false;
    }

    alcMakeContextCurrent(audioDevice->context);

    alGetError();

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_DIRECTION, 0.0f, 0.0f, -1.0f);

    alListenerf(AL_GAIN, 1.0f);

    return false;
}

void elfDeinitAudio()
{
    if (!audioDevice)
        return;

    elfDecRef((elfObject*)audioDevice);

    audioDevice = NULL;
}

void elfUpdateAudio()
{
    elfAudioSource* source;

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