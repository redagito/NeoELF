#pragma once

#include "nelf/Vector.h"
#include "nelf/actorHeader.h"

struct gfxShaderParams;

struct elfCamera
{
    ELF_ACTOR_HEADER;
    int mode;
    int viewpX, viewpY;
    int viewpWidth, viewpHeight;
    elfVec3f position;
    float fov;
    float aspect;
    int orthoX, orthoY;
    int orthoWidth, orthoHeight;
    float clipFar, clipNear;
    float farPlaneWidth, farPlaneHeight;
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

unsigned char elfAabbInsideFrustum(elfCamera* camera, float* min, float* max);

unsigned char elfSphereInsideFrustum(elfCamera* camera, float* pos, float radius);

unsigned char elfCameraInsideAabb(elfCamera* camera, float* min, float* max);

unsigned char elfCameraInsideSphere(elfCamera* camera, float* pos, float radius);

void elfDrawCameraDebug(elfCamera* camera, gfxShaderParams* shaderParams);

elfVec3f elfProjectCameraPoint(elfCamera* camera, float x, float y, float z);

elfVec3f elfUnProjectCameraPoint(elfCamera* camera, float x, float y, float z);