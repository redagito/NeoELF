#pragma once

#include "nelf/Color.h"
#include "nelf/actor/actorHeader.h"

struct elfCamera;
struct gfxShaderParams;

struct elfLight
{
    ELF_ACTOR_HEADER;
    int lightType = 0;
    elfColor color = colorHotPink;

    float range = 0.f;
    float fadeRange = 0.f;
    float innerCone = 0.f;
    float outerCone = 0.f;

    bool visible = true;
    bool shaft = true;

    float shaftSize = 0.f;
    float shaftIntensity = 0.f;
    float shaftFadeOff = 0.f;

    bool shadows = false;
    elfCamera* shadowCamera = nullptr;

    float projectionMatrix[16];
};

elfLight* elfCreateLight(const char* name);

void elfUpdateLight(elfLight* light);

void elfLightPreDraw(elfLight* light);
void elfLightPostDraw(elfLight* light);

void elfDestroyLight(void* data);

void elfSetLightType(elfLight* light, int type);
void elfSetLightColor(elfLight* light, float r, float g, float b, float a);
void elfSetLightRange(elfLight* light, float range, float fadeRange);
void elfSetLightShadows(elfLight* light, bool shadows);
void elfSetLightVisible(elfLight* light, bool visible);
void elfSetLightCone(elfLight* light, float innerCone, float outerCone);
void elfSetLightShaft(elfLight* light, unsigned char shaft);
void elfSetLightShaftSize(elfLight* light, float size);
void elfSetLightShaftIntensity(elfLight* light, float intensity);
void elfSetLightShaftFadeOff(elfLight* light, float fadeOff);

int elfGetLightType(elfLight* light);
elfColor elfGetLightColor(elfLight* light);
float elfGetLightRange(elfLight* light);
float elfGetLightFadeRange(elfLight* light);
unsigned char elfGetLightShadows(elfLight* light);
unsigned char elfGetLightVisible(elfLight* light);
float elfGetLightInnerCone(elfLight* light);
float elfGetLightOuterCone(elfLight* light);

unsigned char elfGetLightShaft(elfLight* light);
float elfGetLightShaftSize(elfLight* light);
float elfGetLightShaftIntensity(elfLight* light);
float elfGetLightShaftFadeOff(elfLight* light);

void elfSetLight(elfLight* light, elfCamera* camera, gfxShaderParams* shaderParams);

unsigned char elfGetLightChanged(elfLight* light);

void elfDrawLightDebug(elfLight* light, gfxShaderParams* shaderParams);