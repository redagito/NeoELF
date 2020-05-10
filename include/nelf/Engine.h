#pragma once

#include <string>

#include "nelf/objectHeader.h"

struct elfConfig;
struct elfTimer;
struct elfPostProcess;
struct elfFont;
struct elfScene;
struct elfGui;
struct elfObject;

// Game engine, provides runtime environment for games etc.
struct elfEngine
{
    ELF_OBJECT_HEADER;

    // Active config
    elfConfig* config = nullptr;

    // TODO ?
    std::string cwd;

    int fps = 0;
    unsigned int frames = 0;
    float sync = 0.f;

    elfTimer* fpsTimer = nullptr;
    elfTimer* fpsLimitTimer = nullptr;
    elfTimer* timeSyncTimer = nullptr;

    bool freeRun = true;
    bool quit = false;

    elfPostProcess* postProcess = nullptr;

    elfFont* guiFont = nullptr;

    elfScene* scene = nullptr;
    elfGui* gui = nullptr;
    // Current actor?
    elfObject* actor = nullptr;
};

// Global engine
extern elfEngine* eng;

elfEngine* elfCreateEngine();
void elfDestroyEngine(void* data);

bool elfInitEngine();
void elfDeinitEngine();

// TODO General application init?
bool elfInit(elfConfig* config);

void elfLimitEngineFps();

void elfUpdateEngine();

void elfCountEngineFps();

bool elfRun();

void elfDeinit();

void elfResizeWindow(int width, int height);

const char* elfGetCurrentDirectory();

void elfQuit();

void elfSetF10Exit(bool exit);
bool elfGetF10Exit();

elfScene* elfLoadScene(const char* filePath);

void elfSetScene(elfScene* scene);
elfScene* elfGetScene();

void elfSetGui(elfGui* gui);
elfGui* elfGetGui();

float elfGetSync();

int elfGetFps();

void elfSetFpsLimit(float fpsLimit);
float elfGetFpsLimit();

void elfSetTickRate(float tickRate);
float elfGetTickRate();

void elfSetSpeed(float speed);
float elfGetSpeed();

bool elfSaveScreenShot(const char* filePath);

void elfSetTextureCompress(bool compress);
bool elfGetTextureCompress();

void elfSetTextureAnisotropy(float anisotropy);
float elfGetTextureAnisotropy();

void elfSetShadowMapSize(int size);
int elfGetShadowMapSize();

int elfGetPolygonsRendered();

// Post processing settings for active engine
void elfSetBloom(float threshold);
void elfDisableBloom();
float elfGetBloomThreshold();

void elfSetDof(float focalRange, float focalDistance);
void elfDisableDof();
float elfGetDofFocalRange();
float elfGetDofFocalDistance();

void elfSetSsao(float amount);
void elfDisableSsao();
float elfGetSsaoAmount();

void elfSetLightShafts(float intensity);
void elfDisableLightShafts();
float elfGetLightShaftsIntensity();

bool elfIsBloom();
bool elfIsSsao();
bool elfIsDof();
bool elfIsLightShafts();

elfObject* elfGetActor();
