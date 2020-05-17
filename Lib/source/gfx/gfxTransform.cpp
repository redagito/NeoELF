#include "nelf/gfx/gfxTransform.h"

#include <glad/glad.h>

#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxMath.h"

void gfxSetViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

void gfxSetScissor(int x, int y, int width, int height) { glViewport(x, y, width, height); }

void gfxEnableScissor() { glEnable(GL_SCISSOR_TEST); }

void gfxDisableScissor() { glDisable(GL_SCISSOR_TEST); }

void gfxGetPerspectiveProjectionMatrix(float fov, float aspect, float near, float far, float* mat)
{
    float top, bottom, left, right;

    top = near * (float)tan(fov * 3.14159265f / 360.0f);
    bottom = -top;
    left = bottom * aspect;
    right = top * aspect;

    memset(mat, 0x0, sizeof(float) * 16);
    mat[0] = near / right;
    mat[5] = near / top;
    mat[10] = (-(far + near)) / (far - near);
    mat[11] = -1;
    mat[14] = (-2 * far * near) / (far - near);
}

void gfxGetOrthographicProjectionMatrix(float left, float right, float bottom, float top, float near, float far,
                                        float* matrix)
{
    memset(matrix, 0x0, sizeof(float) * 16);
    matrix[0] = 2 / (right - left);
    matrix[5] = 2 / (top - bottom);
    matrix[10] = -2 / (far - near);
    matrix[12] = -((right + left) / (right - left));
    matrix[13] = -((top + bottom) / (top - bottom));
    matrix[14] = -((far + near) / (far - near));
    matrix[15] = 1;
}

void gfxProject(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float winCoord[3])
{
    float ftempo[8];

    ftempo[0] = modl[0] * x + modl[4] * y + modl[8] * z + modl[12];
    ftempo[1] = modl[1] * x + modl[5] * y + modl[9] * z + modl[13];
    ftempo[2] = modl[2] * x + modl[6] * y + modl[10] * z + modl[14];
    ftempo[3] = modl[3] * x + modl[7] * y + modl[11] * z + modl[15];
    ftempo[4] = proj[0] * ftempo[0] + proj[4] * ftempo[1] + proj[8] * ftempo[2] + proj[12] * ftempo[3];
    ftempo[5] = proj[1] * ftempo[0] + proj[5] * ftempo[1] + proj[9] * ftempo[2] + proj[13] * ftempo[3];
    ftempo[6] = proj[2] * ftempo[0] + proj[6] * ftempo[1] + proj[10] * ftempo[2] + proj[14] * ftempo[3];
    ftempo[7] = -ftempo[2];

    if (ftempo[7] == 0.0f)
        return;
    ftempo[7] = 1.0f / ftempo[7];
    ftempo[4] *= ftempo[7];
    ftempo[5] *= ftempo[7];
    ftempo[6] *= ftempo[7];

    winCoord[0] = viewp[0] + viewp[2] * (ftempo[4] + 1.0f) * 0.5f;
    winCoord[1] = viewp[1] + viewp[3] * (ftempo[5] + 1.0f) * 0.5f;
    winCoord[2] = (ftempo[6] + 1.0f) * 0.5f;
}

void gfxUnProject(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float objCoord[3])
{
    float m[16], m2[16], in[4], out[4];

    gfxMulMatrix4Matrix4(proj, modl, m2);
    gfxMatrix4GetInverse(m2, m);

    in[0] = (x - (float)viewp[0]) / (float)viewp[2] * 2.0f - 1.0f;
    in[1] = (y - (float)viewp[1]) / (float)viewp[3] * 2.0f - 1.0f;
    in[2] = 2.0f * z - 1.0f;
    in[3] = 1.0f;

    gfxMulMatrix4Vec4(m, in, out);
    if (out[3] == 0.0f)
        return;

    out[3] = 1.0f / out[3];
    objCoord[0] = out[0] * out[3];
    objCoord[1] = out[1] * out[3];
    objCoord[2] = out[2] * out[3];
}

void gfxGetFrustum(float proj[16], float modl[16], float frustum[6][4])
{
    float clip[16];
    int i;
    float length;

    gfxMulMatrix4Matrix4(modl, proj, clip);

    // right plane
    frustum[0][0] = clip[3] - clip[0];
    frustum[0][1] = clip[7] - clip[4];
    frustum[0][2] = clip[11] - clip[8];
    frustum[0][3] = clip[15] - clip[12];
    // left plane
    frustum[1][0] = clip[3] + clip[0];
    frustum[1][1] = clip[7] + clip[4];
    frustum[1][2] = clip[11] + clip[8];
    frustum[1][3] = clip[15] + clip[12];
    // bottom plane
    frustum[2][0] = clip[3] + clip[1];
    frustum[2][1] = clip[7] + clip[5];
    frustum[2][2] = clip[11] + clip[9];
    frustum[2][3] = clip[15] + clip[13];
    // top plane
    frustum[3][0] = clip[3] - clip[1];
    frustum[3][1] = clip[7] - clip[5];
    frustum[3][2] = clip[11] - clip[9];
    frustum[3][3] = clip[15] - clip[13];
    // back plane
    frustum[4][0] = clip[3] - clip[2];
    frustum[4][1] = clip[7] - clip[6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];
    // front plane
    frustum[5][0] = clip[3] + clip[2];
    frustum[5][1] = clip[7] + clip[6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];
    // normalize planes
    for (i = 0; i < 6; i++)
    {
        length =
            (float)sqrt(frustum[i][0] * frustum[i][0] + frustum[i][1] * frustum[i][1] + frustum[i][2] * frustum[i][2]);
        frustum[i][0] /= length;
        frustum[i][1] /= length;
        frustum[i][2] /= length;
        frustum[i][3] /= length;
    }
}

void gfxRecalcTransformMatrix(gfxTransform* transform)
{
    float tempMatrix1[16];
    float tempMatrix2[16];
    float invQua[4];

    if (transform->cameraMode == false)
    {
        gfxMatrix4SetIdentity(transform->matrix);

        transform->matrix[12] = transform->position[0];
        transform->matrix[13] = transform->position[1];
        transform->matrix[14] = transform->position[2];
        gfxQuaGetInverse(transform->orient, invQua);
        gfxQuaToMatrix4(invQua, tempMatrix1);
        gfxMulMatrix4Matrix4(tempMatrix1, transform->matrix, tempMatrix2);

        memcpy(&transform->normalMatrix[0], &tempMatrix2[0], sizeof(float) * 3);
        memcpy(&transform->normalMatrix[3], &tempMatrix2[4], sizeof(float) * 3);
        memcpy(&transform->normalMatrix[6], &tempMatrix2[8], sizeof(float) * 3);

        gfxMatrix4SetIdentity(tempMatrix1);

        tempMatrix1[0] = transform->scale[0];
        tempMatrix1[5] = transform->scale[1];
        tempMatrix1[10] = transform->scale[2];
        gfxMulMatrix4Matrix4(tempMatrix1, tempMatrix2, transform->matrix);
    }
    else
    {
        gfxMatrix4SetIdentity(tempMatrix1);

        tempMatrix1[12] = -transform->position[0];
        tempMatrix1[13] = -transform->position[1];
        tempMatrix1[14] = -transform->position[2];

        gfxQuaToMatrix4(transform->orient, tempMatrix2);

        gfxMulMatrix4Matrix4(tempMatrix1, tempMatrix2, transform->matrix);
    }
}

float* gfxGetTransformMatrix(gfxTransform* transform)
{
    if (transform->recalcMatrix == true)
    {
        gfxRecalcTransformMatrix(transform);
        transform->recalcMatrix = false;
    }

    return transform->matrix;
}

float* gfxGetTransformNormalMatrix(gfxTransform* transform)
{
    if (transform->recalcMatrix == true)
    {
        gfxRecalcTransformMatrix(transform);
        transform->recalcMatrix = false;
    }

    return transform->normalMatrix;
}

gfxTransform* gfxCreateCameraTransform()
{
    gfxTransform* transform;

    transform = (gfxTransform*)malloc(sizeof(gfxTransform));
    memset(transform, 0x0, sizeof(gfxTransform));

    gfxQuaSetIdentity(transform->orient);
    gfxMatrix4SetIdentity(transform->matrix);

    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;

    transform->cameraMode = true;

    return transform;
}

gfxTransform* gfxCreateObjectTransform()
{
    gfxTransform* transform;

    transform = (gfxTransform*)malloc(sizeof(gfxTransform));
    memset(transform, 0x0, sizeof(gfxTransform));

    gfxQuaSetIdentity(transform->orient);
    gfxMatrix4SetIdentity(transform->matrix);

    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;

    return transform;
}

void gfxDestroyTransform(gfxTransform* transform) { free(transform); }

void gfxSetTransformPosition(gfxTransform* transform, float x, float y, float z)
{
    transform->position[0] = x;
    transform->position[1] = y;
    transform->position[2] = z;

    transform->recalcMatrix = true;
}

void gfxSetTransformRotation(gfxTransform* transform, float x, float y, float z)
{
    transform->rotation[0] = x;
    transform->rotation[1] = y;
    transform->rotation[2] = z;

    gfxQuaFromEuler(x, y, z, transform->orient);

    transform->recalcMatrix = true;
}

void gfxSetTransformScale(gfxTransform* transform, float x, float y, float z)
{
    transform->scale[0] = x;
    transform->scale[1] = y;
    transform->scale[2] = z;

    transform->recalcMatrix = true;
}

void gfxSetTransformOrientation(gfxTransform* transform, float x, float y, float z, float w)
{
    transform->orient[0] = x;
    transform->orient[1] = y;
    transform->orient[2] = z;
    transform->orient[3] = w;

    gfxQuaToEuler(transform->orient, transform->rotation);

    transform->recalcMatrix = true;
}

void gfxRotateTransform(gfxTransform* transform, float x, float y, float z)
{
    transform->rotation[0] += x;
    transform->rotation[1] += y;
    transform->rotation[2] += z;

    gfxRotateQua(x, y, z, transform->orient);

    transform->recalcMatrix = true;
}

void gfxRotateTransformLocal(gfxTransform* transform, float x, float y, float z)
{
    transform->rotation[0] += x;
    transform->rotation[1] += y;
    transform->rotation[2] += z;

    gfxRotateQuaLocal(x, y, z, transform->orient);

    transform->recalcMatrix = true;
}

void gfxMoveTransform(gfxTransform* transform, float x, float y, float z)
{
    transform->position[0] += x;
    transform->position[1] += y;
    transform->position[2] += z;

    transform->recalcMatrix = true;
}

void gfxMoveTransformLocal(gfxTransform* transform, float x, float y, float z)
{
    float tempVec[3];
    float vec[3];

    tempVec[0] = x;
    tempVec[1] = y;
    tempVec[2] = z;

    gfxMulQuaVec(transform->orient, tempVec, vec);

    transform->position[0] += vec[0];
    transform->position[1] += vec[1];
    transform->position[2] += vec[2];

    transform->recalcMatrix = true;
}

bool gfxGetTransformCameraMode(gfxTransform* transform) { return transform->cameraMode; }

void gfxGetTransformPosition(gfxTransform* transform, float* params)
{
    memcpy(params, transform->position, sizeof(float) * 3);
}

void gfxGetTransformRotation(gfxTransform* transform, float* params)
{
    memcpy(params, transform->rotation, sizeof(float) * 3);
}

void gfxGetTransformScale(gfxTransform* transform, float* params)
{
    memcpy(params, transform->scale, sizeof(float) * 3);
}

void gfxGetTransformOrientation(gfxTransform* transform, float* params)
{
    memcpy(params, transform->orient, sizeof(float) * 4);
}
