#pragma once

#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/objectHeader.h"

struct elfScene;
struct gfxTexture;
struct gfxRenderTarget;
struct gfxShaderProgram;
struct gfxTransform;

struct elfPostProcess
{
    ELF_OBJECT_HEADER;

    gfxTexture* mainRtColor[2];
    gfxTexture* mainRtDepth = nullptr;
    gfxRenderTarget* mainRt = nullptr;

    gfxTexture* rtTexHigh_1 = nullptr;
    gfxTexture* rtTexHigh_2 = nullptr;
    gfxTexture* rtTexHighDepth = nullptr;

    gfxTexture* rtTexMed_1 = nullptr;
    gfxTexture* rtTexMed_2 = nullptr;
    gfxTexture* rtTexMed_3 = nullptr;

    gfxTexture* rtTexLow_1 = nullptr;
    gfxTexture* rtTexLow_2 = nullptr;

    gfxTexture* rtTexTiny_1 = nullptr;
    gfxTexture* rtTexTiny_2 = nullptr;

    gfxRenderTarget* rtHigh = nullptr;
    gfxRenderTarget* rtMed = nullptr;
    gfxRenderTarget* rtLow = nullptr;
    gfxRenderTarget* rtTiny = nullptr;

    gfxShaderProgram* hipassShdr = nullptr;
    gfxShaderProgram* blurShdr = nullptr;
    gfxShaderProgram* bloomCombineShdr = nullptr;
    gfxShaderProgram* dofCombineShdr = nullptr;
    gfxShaderProgram* ssaoShdr = nullptr;
    gfxShaderProgram* ssaoCombineShdr = nullptr;
    gfxShaderProgram* lightShaftShdr = nullptr;

    bool bloom = true;
    float bloomThreshold = 0.f;

    bool dof = true;
    float dofFocalRange = 0.f;
    float dofFocalDistance = 0.f;
    bool dofAutoFocus = true;

    // Screen space ambient occlusion
    bool ssao = true;
    float ssaoAmount = 0.f;

    bool lightShafts = true;
    float lightShaftsIntensity = 0.f;
    gfxTransform* lightShaftTransform = nullptr;

    int bufferWidth = 0.f;
    int bufferHeight = 0.f;

    gfxShaderParams shaderParams;
};

elfPostProcess* elfCreatePostProcess();
void elfDestroyPostProcess(void* data);

void elfInitPostProcessBuffers(elfPostProcess* postProcess);

void elfRunPostProcess(elfPostProcess* postProcess, elfScene* scene);

// Bloom
void elfSetPostProcessBloom(elfPostProcess* postProcess, float threshold);
void elfDisablePostProcessBloom(elfPostProcess* postProcess);
float elfGetPostProcessBloomThreshold(elfPostProcess* postProcess);

// Depth of field
void elfSetPostProcessDof(elfPostProcess* postProcess, float focalRange, float focalDistance);
void elfDisablePostProcessDof(elfPostProcess* postProcess);
float elfGetPostProcessDofFocalRange(elfPostProcess* postProcess);
float elfGetPostProcessDofFocalDistance(elfPostProcess* postProcess);

// Screen space ambient occlusion
void elfSetPostProcessSsao(elfPostProcess* postProcess, float amount);
void elfDisablePostProcessSsao(elfPostProcess* postProcess);
float elfGetPostProcessSsaoAmount(elfPostProcess* postProcess);

// Light shafts
void elfSetPostProcessLightShafts(elfPostProcess* postProcess, float intensity);
void elfDisablePostProcessLightShafts(elfPostProcess* postProcess);
float elfGetPostProcessLightShaftsIntensity(elfPostProcess* postProcess);

bool elfIsPostProcessBloom(elfPostProcess* postProcess);
bool elfIsPostProcessSsao(elfPostProcess* postProcess);
bool elfIsPostProcessDof(elfPostProcess* postProcess);
bool elfIsPostProcessLightShafts(elfPostProcess* postProcess);
