#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

// Config for engine startup
struct elfConfig
{
    ELF_OBJECT_HEADER;
    elfVec2i windowSize;
    char* windowTitle = nullptr;
    int multisamples = 0;
    bool fullscreen = false;
    bool textureCompress = false;
    float textureAnisotropy = 1.f;
    int shadowMapSize = 1024;

    // TODO What is this?
    char* start = nullptr;

    char* logPath = nullptr;
    float fpsLimit = 0.f;
    float tickRate = 0.f;
    float speed = 1.f;
    bool f10Exit = true;
};

elfConfig* elfCreateConfig();

void elfDestroyConfig(void* data);

elfConfig* elfReadConfig(const char* filePath);

void elfSetConfigWindowSize(elfConfig* config, int width, int height);

void elfSetConfigMultisamples(elfConfig* config, int multisamples);

void elfSetConfigFullscreen(elfConfig* config, bool fullscreen);

void elfSetConfigTextureCompress(elfConfig* config, bool textureCompress);

void elfSetConfigTextureAnisotropy(elfConfig* config, float textureAnisotropy);

void elfSetConfigShadowMapSize(elfConfig* config, int shadowMapSize);

void elfSetConfigStart(elfConfig* config, const char* start);

void elfSetConfigLogPath(elfConfig* config, const char* logPath);

elfVec2i elfGetConfigWindowSize(elfConfig* config);

int elfGetConfigMultisamples(elfConfig* config);

bool elfGetConfigFullscreen(elfConfig* config);

bool elfGetConfigTextureCompress(elfConfig* config);

float elfGetConfigTextureAnisotropy(elfConfig* config);

int elfGetConfigShadowMapSize(elfConfig* config);

const char* elfGetConfigStart(elfConfig* config);

const char* elfGetConfigLogPath(elfConfig* config);