#include "nelf/audio/AudioSource.h"

#include <cstdlib>
#include <cstring>

#include "nelf/List.h"
#include "nelf/Object.h"

elfAudioSource* elfPlaySound(elfSound* sound, float volume)
{
    elfAudioSource* source;

    if (!audioDevice || sound->streaming)
        return NULL;

    source = elfCreateAudioSource();

    // clear errors
    alGetError();

    source->sound = sound;
    elfIncRef((elfObject*)sound);

    alGenSources(1, &source->source);

    // check if everything went well
    if (alGetError() != AL_NO_ERROR)
    {
        elfDestroyAudioSource(source);
        return NULL;
    }

    if (!source->sound->streamed)
        alSourcei(source->source, AL_BUFFER, sound->buffer[0]);

    alSourcef(source->source, AL_GAIN, volume);
    alSourcef(source->source, AL_ROLLOFF_FACTOR, audioDevice->rolloff);

    if (source->sound->streamed)
    {
        elfStreamAudioSource(source);
        source->sound->streaming = ELF_TRUE;
    }
    alSourcePlay(source->source);

    elfAppendListObject(audioDevice->sources, (elfObject*)source);

    return source;
}

elfAudioSource* elfPlayEntitySound(elfEntity* entity, elfSound* sound, float volume)
{
    elfAudioSource* source;
    float position[3];

    if (!audioDevice || sound->streaming)
        return NULL;

    source = elfCreateAudioSource();

    // clear errors
    alGetError();

    source->sound = sound;
    elfIncRef((elfObject*)sound);

    alGenSources(1, &source->source);

    // check if everything went well
    if (alGetError() != AL_NO_ERROR)
    {
        elfDestroyAudioSource(source);
        return NULL;
    }

    if (!source->sound->streamed)
        alSourcei(source->source, AL_BUFFER, sound->buffer[0]);

    alSourcef(source->source, AL_GAIN, volume);
    alSourcef(source->source, AL_ROLLOFF_FACTOR, audioDevice->rolloff);

    elfGetActorPosition_((elfActor*)entity, position);
    alSource3f(source->source, AL_POSITION, position[0], position[1], position[2]);

    if (source->sound->streamed)
    {
        elfStreamAudioSource(source);
        source->sound->streaming = ELF_TRUE;
    }
    alSourcePlay(source->source);

    elfAppendListObject(audioDevice->sources, (elfObject*)source);
    elfAppendListObject(entity->sources, (elfObject*)source);

    return source;
}

elfAudioSource* elfLoopSound(elfSound* sound, float volume)
{
    elfAudioSource* source;

    if (!audioDevice || sound->streaming)
        return NULL;

    source = elfCreateAudioSource();

    // clear errors
    alGetError();

    source->loop = ELF_TRUE;
    source->sound = sound;
    elfIncRef((elfObject*)sound);

    alGenSources(1, &source->source);

    // check if everything went well
    if (alGetError() != AL_NO_ERROR)
    {
        elfDestroyAudioSource(source);
        return NULL;
    }

    if (!source->sound->streamed)
    {
        alSourcei(source->source, AL_BUFFER, sound->buffer[0]);
        alSourcei(source->source, AL_LOOPING, AL_TRUE);
    }

    alSourcef(source->source, AL_GAIN, volume);
    alSourcef(source->source, AL_ROLLOFF_FACTOR, audioDevice->rolloff);

    if (source->sound->streamed)
    {
        elfStreamAudioSource(source);
        source->sound->streaming = ELF_TRUE;
    }
    alSourcePlay(source->source);

    elfAppendListObject(audioDevice->sources, (elfObject*)source);

    return source;
}

elfAudioSource* elfLoopEntitySound(elfEntity* entity, elfSound* sound, float volume)
{
    elfAudioSource* source;
    float position[3];

    if (!audioDevice || sound->streaming)
        return NULL;

    source = elfCreateAudioSource();

    // clear errors
    alGetError();

    source->loop = ELF_TRUE;
    source->sound = sound;
    elfIncRef((elfObject*)sound);

    alGenSources(1, &source->source);

    // check if everything went well
    if (alGetError() != AL_NO_ERROR)
    {
        elfDestroyAudioSource(source);
        return NULL;
    }

    if (!source->sound->streamed)
    {
        alSourcei(source->source, AL_BUFFER, sound->buffer[0]);
        alSourcei(source->source, AL_LOOPING, AL_TRUE);
    }

    alSourcef(source->source, AL_GAIN, volume);
    alSourcef(source->source, AL_ROLLOFF_FACTOR, audioDevice->rolloff);

    elfGetActorPosition_((elfActor*)entity, position);
    alSource3f(source->source, AL_POSITION, position[0], position[1], position[2]);

    if (source->sound->streamed)
    {
        elfStreamAudioSource(source);
        source->sound->streaming = ELF_TRUE;
    }
    alSourcePlay(source->source);

    elfAppendListObject(audioDevice->sources, (elfObject*)source);
    elfAppendListObject(entity->sources, (elfObject*)source);

    return source;
}

elfAudioSource* elfCreateAudioSource()
{
    elfAudioSource* source;

    source = (elfAudioSource*)malloc(sizeof(elfAudioSource));
    memset(source, 0x0, sizeof(elfAudioSource));
    source->objType = ELF_AUDIO_SOURCE;
    source->objDestr = elfDestroyAudioSource;

    elfIncObj(ELF_AUDIO_SOURCE);

    return source;
}

void elfStreamAudioSource(elfAudioSource* source)
{
    int queued;
    int processed;
    unsigned int buffer;

    char buf[ELF_AUDIO_STREAM_CHUNK_SIZE];
    int read;
    int bytesRead;
    int bytesToRead;

    int endian = 0;
    int bitStream = 0;

    if (!source->sound->eof)
    {
        if (!elfIsSoundPlaying(source) && !elfIsSoundPaused(source))
        {
            elfResumeSound(source);
        }
    }
    else
    {
        if (!source->loop)
        {
            return;
        }
    }

    alGetSourcei(source->source, AL_BUFFERS_QUEUED, &queued);
    alGetSourcei(source->source, AL_BUFFERS_PROCESSED, &processed);

    while (processed--) alSourceUnqueueBuffers(source->source, 1, &buffer);

    if (queued < 3)
    {
        bytesRead = 0;
        if (source->sound->fileType == ELF_OGG)
        {
            bytesToRead = ELF_AUDIO_STREAM_CHUNK_SIZE;

            do
            {
                read = ov_read(&source->sound->oggFile, &buf[bytesRead], ELF_AUDIO_STREAM_CHUNK_SIZE - bytesRead,
                               endian, 2, 1, &bitStream);
                if (read < 1)
                {
                    if (source->loop)
                        elfInitSoundWithOgg(source->sound, source->sound->filePath);
                    else
                        source->sound->eof = ELF_TRUE;
                    break;
                }
                bytesRead += read;
                bytesToRead -= read;
            } while (bytesToRead > 0);

            source->sound->position += bytesRead;
        }
        else if (source->sound->fileType == ELF_WAV)
        {
            if (source->sound->length - source->sound->position > ELF_AUDIO_STREAM_CHUNK_SIZE)
                bytesToRead = ELF_AUDIO_STREAM_CHUNK_SIZE;
            else
                bytesToRead = source->sound->length - source->sound->position;

            if (bytesToRead > 0)
                fread(buf, sizeof(char), bytesToRead, source->sound->file);
            else
            {
                if (source->loop)
                    elfInitSoundWithWav(source->sound, source->sound->filePath);
                else
                    source->sound->eof = ELF_TRUE;
            }

            bytesRead = bytesToRead;
            source->sound->position += bytesRead;
        }

        if (bytesRead > 0)
        {
            alBufferData(source->sound->buffer[source->sound->oldestBuffer], source->sound->format, buf, bytesRead,
                         source->sound->freq);
            alSourceQueueBuffers(source->source, 1, &source->sound->buffer[source->sound->oldestBuffer]);
            source->sound->oldestBuffer++;
            if (source->sound->oldestBuffer > 2)
                source->sound->oldestBuffer = 0;
        }
    }
}

void elfDestroyAudioSource(void* data)
{
    elfAudioSource* source = (elfAudioSource*)data;

    if (source->sound)
    {
        source->sound->streaming = ELF_FALSE;
        elfDecRef((elfObject*)source->sound);
    }
    if (source->source)
        alDeleteSources(1, &source->source);

    free(source);

    elfDecObj(ELF_AUDIO_SOURCE);
}

void elfSetSoundVolume(elfAudioSource* source, float volume) { alSourcef(source->source, AL_GAIN, volume); }

float elfGetSoundVolume(elfAudioSource* source)
{
    float volume = 0.0f;
    alGetSourcef(source->source, AL_GAIN, &volume);
    return volume;
}

void elfPauseSound(elfAudioSource* source)
{
    alSourcePause(source->source);
    source->paused = ELF_TRUE;
}

void elfResumeSound(elfAudioSource* source)
{
    if (elfIsSoundPlaying(source))
        return;

    if (source->sound->streamed)
    {
        if (source->sound->eof)
        {
            if (source->sound->fileType == ELF_OGG)
                elfInitSoundWithOgg(source->sound, source->sound->filePath);
            else if (source->sound->fileType == ELF_WAV)
                elfInitSoundWithWav(source->sound, source->sound->filePath);
        }
        else
        {
            alSourcePlay(source->source);
        }
    }
    else
    {
        alSourcePlay(source->source);
    }
    source->paused = ELF_FALSE;
}

void elfStopSound(elfAudioSource* source)
{
    int queued;
    unsigned int buffer;

    if (source->sound->streamed)
    {
        alSourceStop(source->source);
        alGetSourcei(source->source, AL_BUFFERS_QUEUED, &queued);
        while (queued--) alSourceUnqueueBuffers(source->source, 1, &buffer);
        source->sound->eof = ELF_TRUE;
    }
    else
    {
        alSourceStop(source->source);
        source->paused = ELF_FALSE;
    }
}

unsigned char elfIsSoundPlaying(elfAudioSource* source)
{
    int state = 0;
    alGetSourcei(source->source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING)
        return ELF_TRUE;
    return ELF_FALSE;
}

unsigned char elfIsSoundPaused(elfAudioSource* source) { return source->paused; }

void elfSetSoundPosition(elfAudioSource* source, float x, float y, float z)
{
    alSource3f(source->source, AL_POSITION, x, y, z);
}