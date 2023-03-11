#include "nelf/Config.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "StringStream.h"
#include "nelf/objectType.h"

elfConfig* elfCreateConfig()
{
    elfConfig* config;

    config = (elfConfig*)malloc(sizeof(elfConfig));
    memset(config, 0x0, sizeof(elfConfig));
    config->objType = ELF_CONFIG;
    config->objDestr = elfDestroyConfig;

    config->windowSize.x = 1024;
    config->windowSize.y = 768;

    config->windowTitle = (char*)malloc(sizeof(char) * 9);
    memset(config->windowTitle, 0x0, sizeof(char) * 9);
    memcpy(config->windowTitle, "NeoELF", sizeof(char) * 8);

    config->multisamples = 0;
    config->fullscreen = false;
    config->textureCompress = false;
    config->textureAnisotropy = 1.0f;
    config->shadowMapSize = 1024;
    config->fpsLimit = 0.0f;
    config->tickRate = 0.0f;
    config->speed = 1.0f;
    config->f10Exit = true;

    config->start = (char*)malloc(sizeof(char));
    config->start[0] = '\0';

    config->logPath = (char*)malloc(sizeof(char) * 9);
    memset(config->logPath, 0x0, sizeof(char) * 9);
    memcpy(config->logPath, "nelf.log", sizeof(char) * 8);

    return config;
}

void elfDestroyConfig(void* data)
{
    elfConfig* config = (elfConfig*)data;

    if (config->windowTitle)
        free(config->windowTitle);
    if (config->start)
        free(config->start);
    if (config->logPath)
        free(config->logPath);

    free(config);
}

elfConfig* elfReadConfig(const char* filePath)
{
    elfConfig* config;
    FILE* file;
    int length;
    char* text;
    int pos;
    char* str;
    int scope;

    file = fopen(filePath, "r");
    if (!file)
        return nullptr;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (length > 0)
    {
        text = (char*)malloc(sizeof(char) * length + 1);
        memset(text, 0x0, sizeof(char) * length + 1);
        fread(text, sizeof(char), length, file);
        fclose(file);
    }
    else
    {
        fclose(file);
        return nullptr;
    }

    config = elfCreateConfig();

    pos = 0;
    scope = 0;
    while ((str = elfReadSstNext(text, &pos, " \t\n\r")))
    {
        if (scope > 0)
        {
            if (!strcmp(str, "{"))
            {
                scope++;
            }
            else if (!strcmp(str, "}"))
            {
                scope--;
            }
        }
        else
        {
            if (strcmp(str, "windowSize") == 0 || strcmp(str, "window_size") == 0)
            {
                elfReadSstInts(text, &pos, 2, &config->windowSize.x);
            }
            else if (strcmp(str, "windowTitle") == 0 || strcmp(str, "window_title") == 0)
            {
                if (config->windowTitle)
                    free(config->windowTitle);
                config->windowTitle = elfReadSstText(text, &pos);
            }
            else if (!strcmp(str, "multisamples"))
            {
                config->multisamples = elfReadSstInt(text, &pos);
            }
            else if (!strcmp(str, "fullscreen"))
            {
                config->fullscreen = elfReadSstBool(text, &pos);
            }
            else if (strcmp(str, "textureCompress") == 0 || strcmp(str, "texture_compress") == 0)
            {
                config->textureCompress = elfReadSstBool(text, &pos);
            }
            else if (strcmp(str, "textureAnisotropy") == 0 || strcmp(str, "texture_anisotropy") == 0)
            {
                config->textureAnisotropy = elfReadSstFloat(text, &pos);
            }
            else if (strcmp(str, "shadowMapSize") == 0 || strcmp(str, "shadow_map_size") == 0)
            {
                config->shadowMapSize = elfReadSstInt(text, &pos);
            }
            else if (!strcmp(str, "start"))
            {
                if (config->start)
                    free(config->start);
                config->start = elfReadSstString(text, &pos);
            }
            else if (strcmp(str, "logPath") == 0 || strcmp(str, "log_path") == 0)
            {
                if (config->logPath)
                    free(config->logPath);
                config->logPath = elfReadSstString(text, &pos);
            }
            else if (!strcmp(str, "{"))
            {
                scope++;
            }
            else if (!strcmp(str, "}"))
            {
                scope--;
                if (scope < 0)
                {
                    free(str);
                    break;
                }
            }
            else
            {
                printf("warning: unknown element \"%s\" in config.txt\n", str);
            }
        }
        free(str);
        str = nullptr;
    }

    free(text);

    return config;
}

void elfSetConfigWindowSize(elfConfig* config, int width, int height)
{
    config->windowSize.x = width;
    config->windowSize.y = height;

    if (config->windowSize.x < 1)
        config->windowSize.x = 1;
    if (config->windowSize.y < 1)
        config->windowSize.y = 1;
}

void elfSetConfigMultisamples(elfConfig* config, int multisamples)
{
    config->multisamples = multisamples;
    if (config->multisamples < 0)
        config->multisamples = 0;
}

void elfSetConfigFullscreen(elfConfig* config, bool fullscreen) { config->fullscreen = fullscreen; }

void elfSetConfigTextureCompress(elfConfig* config, bool textureCompress) { config->textureCompress = textureCompress; }

void elfSetConfigTextureAnisotropy(elfConfig* config, float textureAnisotropy)
{
    config->textureAnisotropy = textureAnisotropy;
    if (config->textureAnisotropy < 1.0f)
        config->textureAnisotropy = 1.0f;
}

void elfSetConfigShadowMapSize(elfConfig* config, int shadowMapSize)
{
    config->shadowMapSize = shadowMapSize;
    if (config->shadowMapSize < 1)
        config->shadowMapSize = 1;
}

void elfSetConfigStart(elfConfig* config, const char* start)
{
    if (config->start)
        free(config->start);
    config->start = (char*)malloc(sizeof(char) * (strlen(start) + 1));
    memset(config->start, 0x0, sizeof(char) * (strlen(start) + 1));
    memcpy(config->start, start, sizeof(char) * strlen(start));
}

void elfSetConfigLogPath(elfConfig* config, const char* logPath)
{
    if (config->logPath)
        free(config->logPath);
    config->logPath = (char*)malloc(sizeof(char) * (strlen(logPath) + 1));
    memset(config->logPath, 0x0, sizeof(char) * (strlen(logPath) + 1));
    memcpy(config->logPath, logPath, sizeof(char) * strlen(logPath));
}

elfVec2i elfGetConfigWindowSize(elfConfig* config) { return config->windowSize; }

int elfGetConfigMultisamples(elfConfig* config) { return config->multisamples; }

bool elfGetConfigFullscreen(elfConfig* config) { return config->fullscreen; }

bool elfGetConfigTextureCompress(elfConfig* config) { return config->textureCompress; }

float elfGetConfigTextureAnisotropy(elfConfig* config) { return config->textureAnisotropy; }

int elfGetConfigShadowMapSize(elfConfig* config) { return config->shadowMapSize; }

const char* elfGetConfigStart(elfConfig* config) { return config->start; }

const char* elfGetConfigLogPath(elfConfig* config) { return config->logPath; }
