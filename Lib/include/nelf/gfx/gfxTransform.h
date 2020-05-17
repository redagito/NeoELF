#pragma once

struct gfxTransform
{
    float position[3];
    float rotation[3];
    float scale[3];
    float orient[4];
    float matrix[16];
    float normalMatrix[9];

    bool recalcMatrix = false;
    bool cameraMode = false;
};

void gfxSetViewport(int x, int y, int width, int height);
void gfxSetScissor(int x, int y, int width, int height);

void gfxEnableScissor();
void gfxDisableScissor();

void gfxGetPerspectiveProjectionMatrix(float fov, float aspect, float near, float far, float* mat);
void gfxGetOrthographicProjectionMatrix(float left, float right, float bottom, float top, float near, float far,
                                        float* matrix);

void gfxProject(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float winCoord[3]);
void gfxUnProject(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float objCoord[3]);

void gfxGetFrustum(float proj[16], float modl[16], float frustum[6][4]);

void gfxRecalcTransformMatrix(gfxTransform* transform);

float* gfxGetTransformMatrix(gfxTransform* transform);
float* gfxGetTransformNormalMatrix(gfxTransform* transform);

gfxTransform* gfxCreateCameraTransform();

gfxTransform* gfxCreateObjectTransform();
void gfxDestroyTransform(gfxTransform* transform);

void gfxSetTransformPosition(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformRotation(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformScale(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformOrientation(gfxTransform* transform, float x, float y, float z, float w);

void gfxRotateTransform(gfxTransform* transform, float x, float y, float z);
void gfxRotateTransformLocal(gfxTransform* transform, float x, float y, float z);

void gfxMoveTransform(gfxTransform* transform, float x, float y, float z);
void gfxMoveTransformLocal(gfxTransform* transform, float x, float y, float z);

bool gfxGetTransformCameraMode(gfxTransform* transform);

void gfxGetTransformPosition(gfxTransform* transform, float* params);
void gfxGetTransformRotation(gfxTransform* transform, float* params);
void gfxGetTransformScale(gfxTransform* transform, float* params);
void gfxGetTransformOrientation(gfxTransform* transform, float* params);