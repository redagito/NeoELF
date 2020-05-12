#pragma once

#include "nelf/Vector.h"
#include "nelf/actor/actorHeader.h"

struct gfxShaderParams;

struct elfCamera
{
    ELF_ACTOR_HEADER;
    int mode = 0;

    // Viewport
    int viewpX = 0;
    int viewpY = 0;
    int viewpWidth = 0;
    int viewpHeight = 0;

    elfVec3f position;
    float fov = 0.f;
    float aspect = 0.f;

    // Orthographic projection
    int orthoX = 0;
    int orthoY = 0;
    int orthoWidth = 0;
    int orthoHeight = 0;

    // Clipping
    float clipFar = 0.f;
    float clipNear = 0.f;
    float farPlaneWidth = 0.f;
    float farPlaneHeight = 0.f;
    float frustum[6][4];

    float projectionMatrix[16];
    float modelviewMatrix[16];
};

elfCamera* elfCreateCamera(const char* name);

void elfUpdateCamera(elfCamera* camera);

void elfCameraPreDraw(elfCamera* camera);
void elfCameraPostDraw(elfCamera* camera);

void elfDestroyCamera(void* data);

void elfRecalcCamera(elfCamera* camera);

void elfSetCameraMode(elfCamera* camera, int mode);
void elfSetCameraViewport(elfCamera* camera, int x, int y, int width, int height);
void elfSetCameraOrthoViewport(elfCamera* camera, int x, int y, int width, int height);
void elfSetCameraFov(elfCamera* camera, float fov);
void elfSetCameraAspect(elfCamera* camera, float aspect);
void elfSetCameraClip(elfCamera* camera, float clipNear, float clipFar);

int elfGetCameraMode(elfCamera* camera);
elfVec2i elfGetCameraViewportSize(elfCamera* camera);
elfVec2i elfGetCameraViewportOffset(elfCamera* camera);
elfVec2i elfGetCameraOrthoViewportSize(elfCamera* camera);
elfVec2i elfGetCameraOrthoViewportOffset(elfCamera* camera);
float elfGetCameraFov(elfCamera* camera);
float elfGetCameraAspect(elfCamera* camera);
elfVec2f elfGetCameraClip(elfCamera* camera);
elfVec2f elfGetCameraFarPlaneSize(elfCamera* camera);
float* elfGetCameraProjectionMatrix(elfCamera* camera);
float* elfGetCameraModelviewMatrix(elfCamera* camera);

void elfSetCamera(elfCamera* camera, gfxShaderParams* shaderParams);

bool elfAabbInsideFrustum(elfCamera* camera, float* min, float* max);
bool elfSphereInsideFrustum(elfCamera* camera, float* pos, float radius);
bool elfCameraInsideAabb(elfCamera* camera, float* min, float* max);
bool elfCameraInsideSphere(elfCamera* camera, float* pos, float radius);

void elfDrawCameraDebug(elfCamera* camera, gfxShaderParams* shaderParams);

elfVec3f elfProjectCameraPoint(elfCamera* camera, float x, float y, float z);
elfVec3f elfUnProjectCameraPoint(elfCamera* camera, float x, float y, float z);