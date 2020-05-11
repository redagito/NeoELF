#include "nelf/actor/Light.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/actor/lightType.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/objectType.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/resource/Resources.h"

elfLight* elfCreateLight(const char* name)
{
    elfLight* light;

    light = (elfLight*)malloc(sizeof(elfLight));
    memset(light, 0x0, sizeof(elfLight));
    light->objType = ELF_LIGHT;
    light->objDestr = elfDestroyLight;

    elfInitActor((elfActor*)light, false);

    light->range = 20.0f;
    light->fadeRange = 15.0f;
    light->innerCone = 45.0f;
    light->outerCone = 0.0f;
    light->lightType = ELF_POINT_LIGHT;
    light->visible = true;
    light->shaft = false;
    light->shaftSize = 1.0f;
    light->shaftIntensity = 1.0f;
    light->shaftFadeOff = 0.0f;

    elfSetLightColor(light, 1.0f, 1.0f, 1.0f, 1.0f);

    light->shadowCamera = elfCreateCamera("elfShadowCamera");
    elfSetCameraViewport(light->shadowCamera, 0, 0, 512, 512);
    elfSetCameraFov(light->shadowCamera, (light->innerCone + light->outerCone) * 2);
    elfSetCameraAspect(light->shadowCamera, 1.0f);
    elfSetCameraClip(light->shadowCamera, 1.0f, light->range + light->fadeRange);

    gfxMatrix4SetIdentity(light->projectionMatrix);

    light->dobject = elfCreatePhysicsObjectSphere(0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
    elfSetPhysicsObjectActor(light->dobject, (elfActor*)light);
    elfIncRef((elfObject*)light->dobject);

    light->pbbLengths.x = light->pbbLengths.y = light->pbbLengths.z = 1.0f;

    if (name)
        light->name = elfCreateString(name);

    light->id = ++res->lightIdCounter;

    elfIncObj(ELF_LIGHT);

    return light;
}

void elfUpdateLight(elfLight* light) { elfUpdateActor((elfActor*)light); }

void elfLightPreDraw(elfLight* light)
{
    elfActorPreDraw((elfActor*)light);

    elfCameraPreDraw(light->shadowCamera);
}

void elfLightPostDraw(elfLight* light) { elfActorPostDraw((elfActor*)light); }

void elfDestroyLight(void* data)
{
    elfLight* light = (elfLight*)data;

    elfCleanActor((elfActor*)light);

    elfDestroyCamera(light->shadowCamera);

    free(light);

    elfDecObj(ELF_LIGHT);
}

void elfSetLightType(elfLight* light, int type)
{
    if (type != ELF_POINT_LIGHT && type != ELF_SPOT_LIGHT && type != ELF_SUN_LIGHT)
        return;
    light->lightType = type;
}

void elfSetLightColor(elfLight* light, float r, float g, float b, float a)
{
    light->color.r = r;
    light->color.g = g;
    light->color.b = b;
    light->color.a = a;
}

void elfSetLightRange(elfLight* light, float range, float fadeRange)
{
    light->range = range;
    light->fadeRange = fadeRange;
    if (light->range < 0.001f)
        light->range = 0.001f;
    if (light->fadeRange < 0.001f)
        light->fadeRange = 0.001f;
    elfSetCameraClip(light->shadowCamera, 1.0f, light->range + light->fadeRange);
}

void elfSetLightShadows(elfLight* light, bool shadows)
{
    if (light->shadows == shadows)
        return;

    light->shadows = shadows;
    light->moved = true;
}

void elfSetLightVisible(elfLight* light, bool visible) { light->visible = visible; }

void elfSetLightCone(elfLight* light, float innerCone, float outerCone)
{
    light->innerCone = innerCone;
    light->outerCone = outerCone;
    if (light->innerCone < 0.0f)
        light->innerCone = 0.0f;
    if (light->outerCone < 0.0f)
        light->outerCone = 0.0f;
    elfSetCameraFov(light->shadowCamera, (light->innerCone + light->outerCone) * 2);
}

void elfSetLightShaft(elfLight* light, bool shaft) { light->shaft = shaft; }

void elfSetLightShaftSize(elfLight* light, float size)
{
    light->shaftSize = size;
    if (light->shaftSize < 0.0f)
        light->shaftSize = 0.0f;
}

void elfSetLightShaftIntensity(elfLight* light, float intensity)
{
    light->shaftIntensity = intensity;
    if (light->shaftIntensity < 0.0f)
        light->shaftIntensity = 0.0f;
}

void elfSetLightShaftFadeOff(elfLight* light, float fadeOff)
{
    light->shaftFadeOff = fadeOff;
    if (light->shaftFadeOff < 0.0f)
        light->shaftFadeOff = 0.0f;
}

int elfGetLightType(elfLight* light) { return light->lightType; }

elfColor elfGetLightColor(elfLight* light) { return light->color; }

float elfGetLightRange(elfLight* light) { return light->range; }
float elfGetLightFadeRange(elfLight* light) { return light->fadeRange; }

bool elfGetLightShadows(elfLight* light) { return light->shadows; }

bool elfGetLightVisible(elfLight* light) { return light->visible; }

float elfGetLightInnerCone(elfLight* light) { return light->innerCone; }

float elfGetLightOuterCone(elfLight* light) { return light->outerCone; }

bool elfGetLightShaft(elfLight* light) { return light->shaft; }

float elfGetLightShaftSize(elfLight* light) { return light->shaftSize; }

float elfGetLightShaftIntensity(elfLight* light) { return light->shaftIntensity; }

float elfGetLightShaftFadeOff(elfLight* light) { return light->shaftFadeOff; }

void elfSetLight(elfLight* light, elfCamera* camera, gfxShaderParams* shaderParams)
{
    float axis[3];
    float finalAxis[3] = {0.0f, 0.0f, -1.0f};
    float matrix[16];
    float matrix2[16];
    float orient[4];
    float finalPos[4];

    shaderParams->lightParams.type = light->lightType;

    if (gfxGetVersion() < 200)
    {
        gfxGetTransformPosition(light->transform, finalPos);
        gfxGetTransformOrientation(light->transform, orient);
        gfxMulQuaVec(orient, finalAxis, axis);

        memcpy(&shaderParams->lightParams.position.x, finalPos, sizeof(float) * 3);
        memcpy(&shaderParams->lightParams.direction.x, axis, sizeof(float) * 3);
    }
    else
    {
        gfxMulMatrix4Matrix4(gfxGetTransformMatrix(light->transform), elfGetCameraModelviewMatrix(camera), matrix);
        finalPos[0] = matrix[12];
        finalPos[1] = matrix[13];
        finalPos[2] = matrix[14];

        gfxGetTransformOrientation(light->transform, orient);
        gfxMulQuaVec(orient, finalAxis, axis);
        gfxMatrix4GetInverse(elfGetCameraModelviewMatrix(camera), matrix2);
        gfxMulMatrix4Vec3(matrix2, axis, finalAxis);

        memcpy(&shaderParams->lightParams.position.x, finalPos, sizeof(float) * 3);
        memcpy(&shaderParams->lightParams.direction.x, finalAxis, sizeof(float) * 3);
    }

    memcpy(&shaderParams->lightParams.color.r, &light->color.r, sizeof(float) * 4);
    shaderParams->lightParams.range = light->range;
    shaderParams->lightParams.fadeRange = light->fadeRange;
    shaderParams->lightParams.innerCone = light->innerCone;
    shaderParams->lightParams.outerCone = light->outerCone;
}

unsigned char elfGetLightChanged(elfLight* light) { return light->moved; }

void elfDrawLightDebug(elfLight* light, gfxShaderParams* shaderParams)
{
    float* vertexBuffer;
    float step;
    float size;
    int i;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(light->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->lines);

    if (!light->selected)
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.5f, 0.5f, 0.2f, 1.0f);
    else
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    step = (360.0f / 32.0) * GFX_PI_DIV_180;

    for (i = 0; i < 32; i++)
    {
        vertexBuffer[i * 3] = -((float)sin((float)(step * i))) * 0.5f;
        vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * i))) * 0.5f;
        vertexBuffer[i * 3 + 2] = 0.0f;
    }

    elfDrawLineLoop(32, rnd->lines);

    for (i = 0; i < 32; i++)
    {
        vertexBuffer[i * 3] = 0.0f;
        vertexBuffer[i * 3 + 1] = -((float)sin(step * i)) * 0.5f;
        vertexBuffer[i * 3 + 2] = ((float)cos(step * i)) * 0.5f;
    }

    elfDrawLineLoop(32, rnd->lines);

    for (i = 0; i < 32; i++)
    {
        vertexBuffer[i * 3] = -((float)sin(step * i)) * 0.5f;
        vertexBuffer[i * 3 + 1] = 0.0f;
        vertexBuffer[i * 3 + 2] = ((float)cos(step * i)) * 0.5f;
    }

    elfDrawLineLoop(32, rnd->lines);

    if (!light->selected)
        return;

    else
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    if (light->lightType == ELF_POINT_LIGHT)
    {
        step = (360.0f / 128.0f) * GFX_PI_DIV_180;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin(step * i)) * light->range;
            vertexBuffer[i * 3 + 1] = ((float)cos(step * i)) * light->range;
            vertexBuffer[i * 3 + 2] = 0.0f;
        }

        elfDrawLines(128, rnd->lines);

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = 0.0f;
            vertexBuffer[i * 3 + 1] = -((float)sin(step * i)) * light->range;
            vertexBuffer[i * 3 + 2] = ((float)cos(step * i)) * light->range;
        }

        elfDrawLines(128, rnd->lines);

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -((float)sin(step * i)) * light->range;
            vertexBuffer[i * 3 + 1] = 0.0f;
            vertexBuffer[i * 3 + 2] = ((float)cos(step * i)) * light->range;
        }

        elfDrawLines(128, rnd->lines);

        vertexBuffer[0] = 0.0f;
        vertexBuffer[1] = 0.0f;
        vertexBuffer[2] = 0.5f;
        vertexBuffer[3] = 0.0f;
        vertexBuffer[4] = 0.0f;
        vertexBuffer[5] = -0.5f;
        vertexBuffer[6] = -0.5f;
        vertexBuffer[7] = 0.0f;
        vertexBuffer[8] = 0.0f;
        vertexBuffer[9] = 0.5f;
        vertexBuffer[10] = 0.0f;
        vertexBuffer[11] = 0.0f;
        vertexBuffer[12] = 0.0f;
        vertexBuffer[13] = -0.5f;
        vertexBuffer[14] = 0.0f;
        vertexBuffer[15] = 0.0f;
        vertexBuffer[16] = 0.5f;
        vertexBuffer[17] = 0.0f;

        elfDrawLines(6, rnd->lines);
    }
    else if (light->lightType == ELF_SPOT_LIGHT)
    {
        step = (360.0f / 128.0) * GFX_PI_DIV_180;
        size = sin((float)GFX_PI_DIV_180 * light->innerCone) * light->range;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -(float)sin(step * i) * size;
            vertexBuffer[i * 3 + 1] = (float)cos(step * i) * size;
            vertexBuffer[i * 3 + 2] = -light->range;
        }

        elfDrawLines(128, rnd->lines);

        size = sin((float)GFX_PI_DIV_180 * (light->innerCone + light->outerCone)) * light->range;

        for (i = 0; i < 128; i++)
        {
            vertexBuffer[i * 3] = -(float)sin(step * i) * size;
            vertexBuffer[i * 3 + 1] = (float)cos(step * i) * size;
            vertexBuffer[i * 3 + 2] = -light->range;
        }

        elfDrawLines(128, rnd->lines);

        vertexBuffer[0] = 0.0f;
        vertexBuffer[1] = 0.0f;
        vertexBuffer[2] = 0.5f;
        vertexBuffer[3] = 0.0f;
        vertexBuffer[4] = 0.0f;
        vertexBuffer[5] = -0.5f;
        vertexBuffer[6] = -0.5f;
        vertexBuffer[7] = 0.0f;
        vertexBuffer[8] = 0.0f;
        vertexBuffer[9] = 0.5f;
        vertexBuffer[10] = 0.0f;
        vertexBuffer[11] = 0.0f;
        vertexBuffer[12] = 0.0f;
        vertexBuffer[13] = -0.5f;
        vertexBuffer[14] = 0.0f;
        vertexBuffer[15] = 0.0f;
        vertexBuffer[16] = 0.5f;
        vertexBuffer[17] = 0.0f;
        vertexBuffer[18] = 0.0f;
        vertexBuffer[19] = 0.0f;
        vertexBuffer[20] = 0.0f;
        vertexBuffer[21] = size;
        vertexBuffer[22] = 0.0f;
        vertexBuffer[23] = -light->range;
        vertexBuffer[24] = 0.0f;
        vertexBuffer[25] = 0.0f;
        vertexBuffer[26] = 0.0f;
        vertexBuffer[27] = -size;
        vertexBuffer[28] = 0.0f;
        vertexBuffer[29] = -light->range;

        elfDrawLines(13, rnd->lines);
    }
    else if (light->lightType == ELF_SUN_LIGHT)
    {
        vertexBuffer[0] = 0.0f;
        vertexBuffer[1] = 0.0f;
        vertexBuffer[2] = 0.5f;
        vertexBuffer[3] = 0.0f;
        vertexBuffer[4] = 0.0f;
        vertexBuffer[5] = -light->range;
        vertexBuffer[6] = -0.5f;
        vertexBuffer[7] = 0.0f;
        vertexBuffer[8] = 0.0f;
        vertexBuffer[9] = 0.5f;
        vertexBuffer[10] = 0.0f;
        vertexBuffer[11] = 0.0f;
        vertexBuffer[12] = 0.0f;
        vertexBuffer[13] = -0.5f;
        vertexBuffer[14] = 0.0f;
        vertexBuffer[15] = 0.0f;
        vertexBuffer[16] = 0.5f;
        vertexBuffer[17] = 0.0f;

        elfDrawLines(6, rnd->lines);
    }

    elfDrawActorDebug((elfActor*)light, shaderParams);
}