#include "nelf/Engine.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>

// Gfx
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxRenderTarget.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureWrapMode.h"

//
#include "nelf/Config.h"
#include "nelf/Context.h"
#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/Image.h"
#include "nelf/Log.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/PostProcess.h"
#include "nelf/RenderStation.h"
#include "nelf/Scripting.h"
#include "nelf/String.h"
#include "nelf/Timer.h"
#include "nelf/Version.h"
#include "nelf/audio/AudioDevice.h"
#include "nelf/errorCode.h"
#include "nelf/gui/Gui.h"
#include "nelf/keyCode.h"
#include "nelf/objectType.h"
#include "nelf/resource/Network.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Scene.h"

elfEngine* eng = nullptr;

elfEngine* elfCreateEngine()
{
    elfEngine* engine;

    engine = (elfEngine*)malloc(sizeof(elfEngine));
    memset(engine, 0x0, sizeof(elfEngine));
    engine->objType = ELF_ENGINE;
    engine->objDestr = elfDestroyEngine;

    engine->config = elfCreateConfig();
    elfIncRef((elfObject*)engine->config);

    engine->fpsTimer = elfCreateTimer();
    engine->fpsLimitTimer = elfCreateTimer();
    engine->timeSyncTimer = elfCreateTimer();

    elfIncRef((elfObject*)engine->fpsTimer);
    elfIncRef((elfObject*)engine->fpsLimitTimer);
    elfIncRef((elfObject*)engine->timeSyncTimer);

    engine->freeRun = true;

    elfIncObj(ELF_ENGINE);

    return engine;
}

void elfDestroyEngine(void* data)
{
    elfEngine* engine = (elfEngine*)data;

    if (engine->config)
        elfDecRef((elfObject*)engine->config);

    elfDecRef((elfObject*)engine->fpsTimer);
    elfDecRef((elfObject*)engine->fpsLimitTimer);
    elfDecRef((elfObject*)engine->timeSyncTimer);

    if (engine->postProcess)
        elfDestroyPostProcess(engine->postProcess);

    if (engine->scene)
        elfDecRef((elfObject*)engine->scene);
    if (engine->gui)
        elfDecRef((elfObject*)engine->gui);

    if (eng->guiFont)
        elfDecRef((elfObject*)eng->guiFont);

    free(engine);

    elfDecObj(ELF_ENGINE);
}

bool elfInitEngine()
{
    FILE* file = nullptr;

    if (eng)
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: can't initialize the engine twice!\n");
        return false;
    }

    eng = elfCreateEngine();
    if (!eng)
        return false;

    elfIncRef((elfObject*)eng);

    // Set current working directory
    auto path = std::filesystem::current_path();
    if (!path.empty())
    {
        eng->cwd = path.string();
    }

    // TODO Load from data directory
    // Default font
    file = fopen("data/deffont.ttf", "r");
    if (!file)
        elfLogWrite("warning: can't open default font \"deffont.ttf\"\n");
    else
    {
        fclose(file);
        eng->guiFont = elfCreateFontFromFile("data/deffont.ttf", 12);
        if (eng->guiFont)
            elfIncRef((elfObject*)eng->guiFont);
    }

    return true;
}

void elfDeinitEngine()
{
    if (!eng)
        return;

    elfDecRef((elfObject*)eng);
    eng = nullptr;
}

bool elfInit(elfConfig* config)
{
    // Fallback to default
    if (config == nullptr)
        config = elfCreateConfig();

    // Logging
    elfInitLog();

    // Runtime object statistics
    elfInitGeneral();

    // Custom logfile
    elfSetLogFilePath(config->logPath);

    // Enable log to file
    elfStartLog();
    elfLogWrite("%s\n", elfGetVersion());

    // Window with OpenGL context
    if (!elfInitContext(config->windowSize.x, config->windowSize.y, config->windowTitle, config->multisamples,
                        config->fullscreen))
        return false;

    // TODO Init gfx as part of context
    // Requires valid context
    if (!gfxInit())
    {
        elfDeinitContext();
        return false;
    }

    // TODO Check return values
    // Audio system
    elfInitAudio();
    elfInitEngine();
    elfInitRenderStation();
    elfInitResources();
    elfInitScripting();
    elfInitNetworking();

    // Global settings from config
    elfSetTextureCompress(config->textureCompress);
    elfSetTextureAnisotropy(config->textureAnisotropy);
    elfSetShadowMapSize(config->shadowMapSize);

    // Check if start-up pak file set
    if (strlen(config->start) > 0)
    {
        elfLoadScene(config->start);
    }

    // Set config, remove old if exists
    if (eng->config)
        elfDecRef((elfObject*)eng->config);
    eng->config = config;
    elfIncRef((elfObject*)eng->config);

    return true;
}

void elfLimitEngineFps()
{
    if (!elfAboutZero(eng->config->fpsLimit))
    {
        if (elfGetElapsedTime(eng->fpsLimitTimer) > 0.0f)
        {
            while (elfGetElapsedTime(eng->fpsLimitTimer) < 1.0f / (float)eng->config->fpsLimit)
                ;
            elfStartTimer(eng->fpsLimitTimer);
        }
        else
        {
            elfStartTimer(eng->fpsLimitTimer);
        }
    }
}

void elfUpdateEngine()
{
    elfUpdateAudio();

    if (elfGetElapsedTime(eng->timeSyncTimer) > 0.0f)
    {
        if (elfAboutZero(eng->config->tickRate))
            eng->sync = (eng->sync * 2.0f + ((float)elfGetElapsedTime(eng->timeSyncTimer) * eng->config->speed)) / 3.0f;
        else
            eng->sync = eng->config->tickRate;

        elfStartTimer(eng->timeSyncTimer);

        if (eng->sync > 0.0f)
        {
            if (eng->gui)
                elfUpdateGui(eng->gui, eng->sync);

            if (eng->scene)
            {
                elfUpdateScene(eng->scene, eng->sync);
            }
        }
    }
    else
    {
        elfStartTimer(eng->timeSyncTimer);
    }

    elfUpdateScripting();
}

void elfCountEngineFps()
{
    eng->frames++;

    if (elfGetElapsedTime(eng->fpsTimer) > 0.0f)
    {
        if (elfGetElapsedTime(eng->fpsTimer) >= 1.0f)
        {
            eng->fps = eng->frames;
            eng->frames = 0;
            elfStartTimer(eng->fpsTimer);
        }
    }
    else
    {
        elfStartTimer(eng->fpsTimer);
    }
}

bool elfRun()
{
    if (eng == nullptr)
        return false;
    if (!eng->freeRun)
        return false;

    eng->freeRun = false;

    // Check if engine should quit
    // TODO Why not just set eng->quit with f10 key?
    if ((eng->config->f10Exit && elfGetKeyState(ELF_KEY_F10)) || !elfIsWindowOpened() || eng->quit)
    {
        eng->freeRun = true;
        return false;
    }

    // Reset rendering statistics
    // TODO Have some kind of performance / benchmark mode and otherwise not do this
    gfxResetVerticesDrawn();

    // For post processing enabled render to offscreen render target
    if (eng->postProcess != nullptr)
    {
        if (elfGetMultisamples() < 1)
        {
            gfxSetRenderTarget(eng->postProcess->mainRt);
            gfxSetRenderTargetColorTexture(eng->postProcess->mainRt, 0, eng->postProcess->mainRtColor[0]);
        }
    }

    // Clear to black
    gfxClearBuffers(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Draw active scene
    if (eng->scene)
    {
        elfScenePreDraw(eng->scene);
        elfDrawScene(eng->scene);
        elfScenePostDraw(eng->scene);
    }

    if (eng->scene && eng->postProcess)
    {
        if (elfGetMultisamples() > 0)
        {
            gfxCopyFrameBuffer(eng->postProcess->mainRtColor[0], 0, 0, 0, 0, elfGetWindowWidth(), elfGetWindowHeight());
            if (eng->postProcess->dof || eng->postProcess->ssao)
            {
                gfxCopyFrameBuffer(eng->postProcess->mainRtDepth, 0, 0, 0, 0, elfGetWindowWidth(),
                                   elfGetWindowHeight());
            }
        }
        elfRunPostProcess(eng->postProcess, eng->scene);
    }

    // Debug draw
    if (eng->scene && eng->scene->debugDraw)
        elfDrawSceneDebug(eng->scene);

    // Draw GUI
    if (eng->gui)
        elfDrawGui(eng->gui);

    elfSwapBuffers();

    // TODO WHYYYY
    elfLimitEngineFps();
    elfUpdateEngine();
    elfCountEngineFps();

    eng->freeRun = true;

    // TODO WHYY
    elfSleep(0.001f);

    return true;
}

void elfDeinit()
{
    elfDeinitNetworking();
    elfDeinitScripting();
    elfDeinitResources();
    elfDeinitRenderStation();
    elfDeinitEngine();
    elfDeinitAudio();
    gfxDeinit();
    elfDeinitContext();
    elfDeinitGeneral();
}

void elfResizeWindow(int width, int height)
{
    elfSetError(ELF_MISSING_FEATURE, "error: resizing not yet in a usable state\n");

    if (width <= 0 || height <= 0 || (width == elfGetWindowWidth() && height == elfGetWindowHeight()))
        return;

    /*elfResizeContext(width, height);
    elfInitPostProcessBuffers(eng->postProcess);*/
}

const char* elfGetCurrentDirectory() { return eng->cwd.c_str(); }

void elfQuit() { eng->quit = true; }

void elfSetF10Exit(bool exit) { eng->config->f10Exit = exit; }

bool elfGetF10Exit() { return eng->config->f10Exit; }

elfScene* elfLoadScene(const char* filePath)
{
    elfScene* scene;

    scene = elfCreateSceneFromFile("", filePath);
    if (scene != nullptr)
    {
        if (eng->scene)
            elfDecRef((elfObject*)eng->scene);
        eng->scene = scene;
        elfIncRef((elfObject*)eng->scene);
    }

    return scene;
}

void elfSetScene(elfScene* scene)
{
    if (eng->scene)
        elfDecRef((elfObject*)eng->scene);
    eng->scene = scene;
    if (eng->scene)
        elfIncRef((elfObject*)eng->scene);
}

elfScene* elfGetScene() { return eng->scene; }

void elfSetGui(elfGui* gui)
{
    if (eng->gui)
        elfDecRef((elfObject*)eng->gui);
    eng->gui = gui;
    if (eng->gui)
        elfIncRef((elfObject*)eng->gui);
}

elfGui* elfGetGui() { return eng->gui; }

float elfGetSync() { return eng->sync; }

int elfGetFps() { return eng->fps; }

void elfSetFpsLimit(float fpsLimit)
{
    eng->config->fpsLimit = fpsLimit;
    if (eng->config->fpsLimit < 0.0f)
        eng->config->fpsLimit = 0.0f;
}

float elfGetFpsLimit() { return eng->config->fpsLimit; }

void elfSetTickRate(float tickRate)
{
    eng->config->tickRate = tickRate;
    if (eng->config->tickRate < 0.0f)
        eng->config->tickRate = 0.0f;
}

float elfGetTickRate() { return eng->config->tickRate; }

void elfSetSpeed(float speed)
{
    eng->config->speed = speed;
    if (eng->config->speed < 0.0001f)
        eng->config->speed = 0.0001f;
}

float elfGetSpeed() { return eng->config->speed; }

bool elfSaveScreenShot(const char* filePath)
{
    unsigned char* data = nullptr;

    data = (unsigned char*)malloc(sizeof(unsigned char) * elfGetWindowWidth() * elfGetWindowHeight() * 4);

    gfxReadPixels(0, 0, elfGetWindowWidth(), elfGetWindowHeight(), GFX_BGRA, GFX_UBYTE, data);

    if (!elfSaveImageData(filePath, elfGetWindowWidth(), elfGetWindowHeight(), 32, data))
    {
        free(data);
        return false;
    }

    free(data);

    return true;
}

void elfSetTextureCompress(bool compress) { eng->config->textureCompress = compress; }

bool elfGetTextureCompress() { return eng->config->textureCompress; }

void elfSetTextureAnisotropy(float anisotropy) { eng->config->textureAnisotropy = anisotropy; }

float elfGetTextureAnisotropy() { return eng->config->textureAnisotropy; }

void elfSetShadowMapSize(int size)
{
    // why would someone want a shadow map of 1 pixel?...
    if (gfxGetVersion() < 200 || size < 1 || size == eng->config->shadowMapSize)
        return;

    gfxDecRef((gfxObject*)rnd->shadowMap);
    gfxDecRef((gfxObject*)rnd->shadowTarget);

    eng->config->shadowMapSize = size;
    rnd->shadowMap = gfxCreate2dTexture(size, size, 0.0f, GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT,
                                        GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
    rnd->shadowTarget = gfxCreateRenderTarget(size, size);

    gfxIncRef((gfxObject*)rnd->shadowMap);
    gfxIncRef((gfxObject*)rnd->shadowTarget);

    gfxSetRenderTargetDepthTexture(rnd->shadowTarget, rnd->shadowMap);
}

int elfGetShadowMapSize() { return eng->config->shadowMapSize; }

int elfGetPolygonsRendered()
{
    return gfxGetVerticesDrawn(GFX_TRIANGLES) / 3 + gfxGetVerticesDrawn(GFX_TRIANGLE_STRIP) / 3;
}

void elfSetBloom(float threshold)
{
    if (gfxGetVersion() < 200)
        return;

    if (!eng->postProcess)
        eng->postProcess = elfCreatePostProcess();
    elfSetPostProcessBloom(eng->postProcess, threshold);
}

void elfDisableBloom()
{
    if (eng->postProcess)
    {
        elfDisablePostProcessBloom(eng->postProcess);
        if (!elfIsPostProcessBloom(eng->postProcess) && !elfIsPostProcessSsao(eng->postProcess) &&
            !elfIsPostProcessDof(eng->postProcess) && !elfIsPostProcessLightShafts(eng->postProcess))
        {
            elfDestroyPostProcess(eng->postProcess);
            eng->postProcess = nullptr;
        }
    }
}

float elfGetBloomThreshold()
{
    if (!eng->postProcess)
        return 0.f;
    return elfGetPostProcessBloomThreshold(eng->postProcess);
}

void elfSetDof(float focalRange, float focalDistance)
{
    if (gfxGetVersion() < 200)
        return;

    if (!eng->postProcess)
        eng->postProcess = elfCreatePostProcess();
    elfSetPostProcessDof(eng->postProcess, focalRange, focalDistance);
}

void elfDisableDof()
{
    if (eng->postProcess)
    {
        elfDisablePostProcessDof(eng->postProcess);
        if (!elfIsPostProcessBloom(eng->postProcess) && !elfIsPostProcessSsao(eng->postProcess) &&
            !elfIsPostProcessDof(eng->postProcess) && !elfIsPostProcessLightShafts(eng->postProcess))
        {
            elfDestroyPostProcess(eng->postProcess);
            eng->postProcess = nullptr;
        }
    }
}

float elfGetDofFocalRange()
{
    if (!eng->postProcess)
        return 0.f;
    return elfGetPostProcessDofFocalRange(eng->postProcess);
}

float elfGetDofFocalDistance()
{
    if (!eng->postProcess)
        return 0.f;
    return elfGetPostProcessDofFocalDistance(eng->postProcess);
}

void elfSetSsao(float amount)
{
    if (gfxGetVersion() < 200)
        return;

    if (!eng->postProcess)
        eng->postProcess = elfCreatePostProcess();
    elfSetPostProcessSsao(eng->postProcess, amount);
}

void elfDisableSsao()
{
    if (eng->postProcess)
    {
        elfDisablePostProcessSsao(eng->postProcess);
        if (!elfIsPostProcessBloom(eng->postProcess) && !elfIsPostProcessSsao(eng->postProcess) &&
            !elfIsPostProcessDof(eng->postProcess) && !elfIsPostProcessLightShafts(eng->postProcess))
        {
            elfDestroyPostProcess(eng->postProcess);
            eng->postProcess = nullptr;
        }
    }
}

float elfGetSsaoAmount()
{
    if (eng->postProcess)
        return elfGetPostProcessSsaoAmount(eng->postProcess);
    return 0.0f;
}

void elfSetLightShafts(float intensity)
{
    if (gfxGetVersion() < 200)
        return;

    if (!eng->postProcess)
        eng->postProcess = elfCreatePostProcess();
    elfSetPostProcessLightShafts(eng->postProcess, intensity);
}

void elfDisableLightShafts()
{
    if (eng->postProcess)
    {
        elfDisablePostProcessLightShafts(eng->postProcess);
        if (!elfIsPostProcessBloom(eng->postProcess) && !elfIsPostProcessSsao(eng->postProcess) &&
            !elfIsPostProcessDof(eng->postProcess) && !elfIsPostProcessLightShafts(eng->postProcess))
        {
            elfDestroyPostProcess(eng->postProcess);
            eng->postProcess = nullptr;
        }
    }
}

float elfGetLightShaftsIntensity()
{
    if (eng->postProcess)
        return elfGetPostProcessLightShaftsIntensity(eng->postProcess);
    return 0.0f;
}

bool elfIsBloom()
{
    if (eng->postProcess)
        return elfIsPostProcessBloom(eng->postProcess);
    return false;
}

bool elfIsSsao()
{
    if (eng->postProcess)
        return elfIsPostProcessSsao(eng->postProcess);
    return false;
}

bool elfIsDof()
{
    if (eng->postProcess)
        return elfIsPostProcessDof(eng->postProcess);
    return false;
}

bool elfIsLightShafts()
{
    if (eng->postProcess)
        return elfIsPostProcessLightShafts(eng->postProcess);
    return false;
}

// Current actor?
elfObject* elfGetActor() { return eng->actor; }
