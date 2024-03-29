#include "nelf/audio/Sound.h"

#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include <cstdlib>
#include <cstring>
#include <vector>

#include "audio/DataDump.h"
#include "audio/streamChunkSize.h"
#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/Object.h"
#include "nelf/String.h"
#include "nelf/audio/AudioDevice.h"
#include "nelf/audio/soundFileType.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

elfSound* elfCreateSound()
{
    elfSound* sound;

    sound = (elfSound*)malloc(sizeof(elfSound));
    memset(sound, 0x0, sizeof(elfSound));
    sound->objType = ELF_SOUND;
    sound->objDestr = elfDestroySound;

    elfIncObj(ELF_SOUND);

    return sound;
}

void elfDestroySound(void* data)
{
    elfSound* sound = (elfSound*)data;

    if (sound->filePath)
        elfDestroyString(sound->filePath);

    if (!sound->streamed)
    {
        if (sound->buffer[0])
            alDeleteBuffers(1, &sound->buffer[0]);
    }
    else
    {
        alDeleteBuffers(3, sound->buffer);
        if (sound->fileType == ELF_OGG)
            ov_clear(&sound->oggFile);
        if (sound->fileType == ELF_WAV)
            fclose(sound->file);
    }
    free(sound);

    elfDecObj(ELF_SOUND);
}

bool elfInitSoundWithOgg(elfSound* snd, const char* filePath)
{
    vorbis_info* info = nullptr;

    if (snd->streamed && snd->fileType == ELF_OGG)
    {
        ov_clear(&snd->oggFile);
        snd->file = nullptr;
    }
    if (snd->streamed && snd->fileType == ELF_WAV)
    {
        fclose(snd->file);
        snd->file = nullptr;
    }

    snd->fileType = ELF_NONE;
    snd->length = 0;
    snd->position = 0;
    snd->eof = false;

    snd->file = fopen(filePath, "rb");
    if (!snd->file)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        return false;
    }

    snd->fileType = ELF_OGG;

    ov_open(snd->file, &snd->oggFile, NULL, 0);
    info = ov_info(&snd->oggFile, -1);

    if (info->channels == 1)
        snd->format = AL_FORMAT_MONO16;
    else if (info->channels == 2)
        snd->format = AL_FORMAT_STEREO16;
    else
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: invalid number of channels in \"%s\"\n", filePath);
        ov_clear(&snd->oggFile);
        return false;
    }

    snd->freq = info->rate;

    return true;
}

bool elfInitSoundWithWav(elfSound* snd, const char* filePath)
{
    std::vector<char> buffer;
    buffer.resize(ELF_AUDIO_STREAM_CHUNK_SIZE);

    char magic[5] = "\0\0\0\0";
    unsigned int chunkLength = 0;
    int junkData = 0;
    unsigned short int audioFormat = 0;
    unsigned short int channels = 0;
    unsigned int freq = 0;
    bool fmtFound = false;
    bool dataFound = false;

    if (snd->streamed && snd->fileType == ELF_OGG)
    {
        ov_clear(&snd->oggFile);
        snd->file = nullptr;
    }
    if (snd->streamed && snd->fileType == ELF_WAV)
    {
        fclose(snd->file);
        snd->file = nullptr;
    }

    snd->fileType = ELF_NONE;
    snd->length = 0;
    snd->position = 0;
    snd->eof = false;

    snd->file = fopen(filePath, "rb");
    if (!snd->file)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        return false;
    }

    snd->fileType = ELF_WAV;

    fread(&magic, sizeof(int), 1, snd->file);
    if (strcmp((char*)&magic, "RIFF"))
    {
        elfSetError(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, RIFF not found\n", filePath);
        return false;
    }

    fread(&chunkLength, sizeof(unsigned int), 1, snd->file);
    fread(&magic, sizeof(int), 1, snd->file);
    if (strcmp((char*)&magic, "WAVE"))
    {
        elfSetError(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, WAVE not found\n", filePath);
        return false;
    }

    while (1)
    {
        if (fmtFound && dataFound)
            break;

        if (feof(snd->file))
        {
            if (!fmtFound || !dataFound)
            {
                elfSetError(ELF_INVALID_FILE, "error: \"%s\" invalid wav file\n", filePath);
                return false;
            }
            break;
        }

        fread(&magic, sizeof(int), 1, snd->file);
        fread(&chunkLength, sizeof(unsigned int), 1, snd->file);
        if (strcmp((char*)&magic, "fmt ") == 0)
        {
            fread(&audioFormat, sizeof(unsigned short int), 1, snd->file);
            fread(&channels, sizeof(unsigned short int), 1, snd->file);
            fread(&freq, sizeof(unsigned int), 1, snd->file);
            fread(&junkData, sizeof(unsigned int), 1, snd->file);
            fread(&junkData, sizeof(unsigned short int), 1, snd->file);
            fread(&junkData, sizeof(unsigned short int), 1, snd->file);

            // TODO Read but never access?
            //      Maybe just seek instead?
            if (chunkLength - 16 > 0)
                fread(buffer.data(), 1, chunkLength - 16, snd->file);

            if (audioFormat != 1)
            {
                elfSetError(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, invalid audio format\n", filePath);
                return false;
            }

            snd->freq = freq;

            if (channels == 1)
                snd->format = AL_FORMAT_MONO16;
            else if (channels == 2)
                snd->format = AL_FORMAT_STEREO16;
            else
            {
                elfSetError(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, invalid number of channels\n", filePath);
                return false;
            }

            fmtFound = true;
        }
        else if (strcmp((char*)&magic, "data") == 0)
        {
            snd->length = chunkLength;
            dataFound = true;
        }
    }

    return true;
}

static char* loadOgg(elfSound* snd)
{
    if (!elfInitSoundWithOgg(snd, snd->filePath))
    {
        elfDestroySound(snd);
        return NULL;
    }

    elfDataDump* dump = elfCreateDataDump();
    std::vector<char> buffer;
    buffer.resize(ELF_AUDIO_STREAM_CHUNK_SIZE);

    int bytesRead = 0;
    do
    {
        int endian = 0;
        int bitStream = 0;
        bytesRead = ov_read(&snd->oggFile, buffer.data(), ELF_AUDIO_STREAM_CHUNK_SIZE, endian, 2, 1, &bitStream);

        elfAddChunkToDataDump(dump, buffer.data(), bytesRead);
        snd->length += bytesRead;
    } while (bytesRead > 0);

    snd->length = elfGetDataDumpLength(dump);
    char* data = (char*)malloc(snd->length);
    elfDataDumpToBuffer(dump, data);

    elfDestroyDataDump(dump);
    ov_clear(&snd->oggFile);

    return data;
}

static char* loadWav(elfSound* snd)
{
    if (!elfInitSoundWithWav(snd, snd->filePath))
    {
        elfDestroySound(snd);
        return nullptr;
    }

    char* data = (char*)malloc(snd->length);
    fread(data, sizeof(char), snd->length, snd->file);

    fclose(snd->file);
    snd->file = nullptr;

    return data;
}

elfSound* elfLoadSound(const char* filePath)
{
    if (audioDevice == nullptr)
        return nullptr;

    elfSound* snd = elfCreateSound();
    snd->filePath = elfCreateString(filePath);

    char* data = nullptr;
    char* type = strrchr((char*)filePath, '.');
    if (strcmp(type, ".ogg") == 0)
    {
        data = loadOgg(snd);
    }
    else if (strcmp(type, ".wav") == 0)
    {
        data = loadWav(snd);
    }
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load \"%s\", unknown format\n", filePath);
        elfDestroySound(snd);
        return NULL;
    }

    // No data loaded
    if (data == nullptr)
    {
        // TODO Probably not the best error code
        elfSetError(ELF_CANT_CREATE, "error: can't load \"%s\", no data loaded\n", filePath);
        elfDestroySound(snd);
        return nullptr;
    }

    // Load data into OpenAL
    alGenBuffers(1, &snd->buffer[0]);
    alBufferData(snd->buffer[0], snd->format, data, snd->length, snd->freq);

    free(data);

    return snd;
}

elfSound* elfLoadStreamedSound(const char* filePath)
{
    elfSound* snd = nullptr;

    char* type = nullptr;

    if (!audioDevice)
        return NULL;

    snd = elfCreateSound();

    snd->filePath = elfCreateString(filePath);
    snd->streamed = true;

    type = strrchr((char*)filePath, '.');

    if (strcmp(type, ".ogg") == 0)
    {
        if (!elfInitSoundWithOgg(snd, snd->filePath))
        {
            elfDestroySound(snd);
            return NULL;
        }
    }
    else if (strcmp(type, ".wav") == 0)
    {
        if (!elfInitSoundWithWav(snd, snd->filePath))
        {
            elfDestroySound(snd);
            return NULL;
        }
    }
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load \"%s\", unknown format\n", filePath);
        elfDestroySound(snd);
        return NULL;
    }

    alGenBuffers(3, snd->buffer);

    return snd;
}

int elfGetSoundFileType(elfSound* sound) { return sound->fileType; }