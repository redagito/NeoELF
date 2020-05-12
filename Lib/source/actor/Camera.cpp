#include "nelf/actor/Camera.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Context.h"
#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/cameraMode.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/objectType.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/resource/Resources.h"

elfCamera* elfCreateCamera(const char* name)
{
    elfCamera* camera;

    camera = (elfCamera*)malloc(sizeof(elfCamera));
    memset(camera, 0x0, sizeof(elfCamera));
    camera->objType = ELF_CAMERA;
    camera->objDestr = elfDestroyCamera;

    elfInitActor((elfActor*)camera, true);

    camera->mode = ELF_PERSPECTIVE;
    camera->viewpX = 0;
    camera->viewpY = 0;
    camera->viewpWidth = -1;
    camera->viewpHeight = -1;
    camera->fov = 35.0f;
    camera->aspect = -1.0f;
    camera->orthoX = 0;
    camera->orthoY = 0;
    camera->orthoWidth = -1;
    camera->orthoHeight = -1;
    camera->clipNear = 1.0f;
    camera->clipFar = 250.0f;

    gfxMatrix4SetIdentity(camera->projectionMatrix);
    gfxMatrix4SetIdentity(camera->modelviewMatrix);

    elfSetCameraMode(camera, ELF_PERSPECTIVE);

    camera->dobject = elfCreatePhysicsObjectSphere(0.5, 0.0f, 0.0f, 0.0f, 0.0f);
    elfSetPhysicsObjectActor(camera->dobject, (elfActor*)camera);
    elfIncRef((elfObject*)camera->dobject);

    camera->pbbLengths.x = camera->pbbLengths.y = camera->pbbLengths.z = 1.0f;

    if (name)
        camera->name = elfCreateString(name);

    camera->id = ++res->cameraIdCounter;

    elfIncObj(ELF_CAMERA);

    return camera;
}

void elfUpdateCamera(elfCamera* camera) { elfUpdateActor((elfActor*)camera); }

void elfCameraPreDraw(elfCamera* camera)
{
    elfActorPreDraw((elfActor*)camera);

    gfxGetFrustum(camera->projectionMatrix, elfGetCameraModelviewMatrix(camera), camera->frustum);
    elfGetActorPosition_((elfActor*)camera, &camera->position.x);
}

void elfCameraPostDraw(elfCamera* camera) { elfActorPostDraw((elfActor*)camera); }

void elfDestroyCamera(void* data)
{
    elfCamera* camera = (elfCamera*)data;

    elfCleanActor((elfActor*)camera);

    free(camera);

    elfDecObj(ELF_CAMERA);
}

void elfRecalcCamera(elfCamera* camera)
{
    if (camera->mode == ELF_PERSPECTIVE)
    {
        gfxGetPerspectiveProjectionMatrix(camera->fov, elfGetCameraAspect(camera), camera->clipNear, camera->clipFar,
                                          camera->projectionMatrix);

        camera->farPlaneHeight = 2 * (float)tan(camera->fov * GFX_PI_DIV_180 / 2) * camera->clipFar;
        camera->farPlaneWidth = camera->farPlaneHeight * elfGetCameraAspect(camera);
    }
    else if (camera->mode == ELF_ORTHOGRAPHIC)
    {
        gfxGetOrthographicProjectionMatrix((float)camera->orthoX, (float)(camera->orthoX + camera->orthoWidth),
                                           (float)camera->orthoY, (float)(camera->orthoY + camera->orthoHeight),
                                           (float)camera->clipNear, camera->clipFar, camera->projectionMatrix);

        camera->farPlaneWidth = camera->orthoWidth;
        camera->farPlaneHeight = camera->orthoHeight;
    }
}

void elfSetCameraMode(elfCamera* camera, int mode)
{
    if (mode != ELF_PERSPECTIVE && mode != ELF_ORTHOGRAPHIC)
        return;

    camera->mode = mode;

    elfRecalcCamera(camera);
}

void elfSetCameraViewport(elfCamera* camera, int x, int y, int width, int height)
{
    camera->viewpX = x;
    camera->viewpY = y;
    camera->viewpWidth = width;
    camera->viewpHeight = height;
}

void elfSetCameraOrthoViewport(elfCamera* camera, int x, int y, int width, int height)
{
    camera->orthoX = x;
    camera->orthoY = y;
    camera->orthoWidth = width;
    camera->orthoHeight = height;
}

void elfSetCameraFov(elfCamera* camera, float fov)
{
    camera->fov = fov;

    elfRecalcCamera(camera);
}

void elfSetCameraAspect(elfCamera* camera, float aspect)
{
    camera->aspect = aspect;

    elfRecalcCamera(camera);
}

void elfSetCameraClip(elfCamera* camera, float clipNear, float clipFar)
{
    camera->clipNear = clipNear;
    camera->clipFar = clipFar;

    elfRecalcCamera(camera);
}

int elfGetCameraMode(elfCamera* camera) { return camera->mode; }

elfVec2i elfGetCameraViewportSize(elfCamera* camera)
{
    elfVec2i size;

    size.x = camera->viewpWidth;
    size.y = camera->viewpHeight;
    if (size.x <= 0.0f)
        size.x = elfGetWindowWidth();
    if (size.y <= 0.0f)
        size.x = elfGetWindowHeight();

    return size;
}

elfVec2i elfGetCameraViewportOffset(elfCamera* camera)
{
    elfVec2i offset;

    offset.x = camera->viewpX;
    offset.y = camera->viewpY;

    return offset;
}

elfVec2i elfGetCameraOrthoViewportSize(elfCamera* camera)
{
    elfVec2i size;

    size.x = camera->orthoWidth;
    size.y = camera->orthoHeight;

    return size;
}

elfVec2i elfGetCameraOrthoViewportOffset(elfCamera* camera)
{
    elfVec2i offset;

    offset.x = camera->orthoX;
    offset.y = camera->orthoY;

    return offset;
}

float elfGetCameraFov(elfCamera* camera) { return camera->fov; }

float elfGetCameraAspect(elfCamera* camera)
{
    if (camera->aspect <= 0.0f)
    {
        if ((float)elfGetWindowWidth() / (float)elfGetWindowHeight() >= 1.0f)
            return (float)elfGetWindowWidth() / (float)elfGetWindowHeight();
        else
            return (float)elfGetWindowHeight() / (float)elfGetWindowWidth();
    }

    return camera->aspect;
}

elfVec2f elfGetCameraClip(elfCamera* camera)
{
    elfVec2f clip;

    clip.x = camera->clipNear;
    clip.y = camera->clipFar;

    return clip;
}

elfVec2f elfGetCameraFarPlaneSize(elfCamera* camera)
{
    elfVec2f size;

    size.x = camera->farPlaneWidth;
    size.y = camera->farPlaneHeight;
    if (size.x <= 0.0f)
        size.x = elfGetWindowWidth();
    if (size.y <= 0.0f)
        size.x = elfGetWindowHeight();

    return size;
}

float* elfGetCameraProjectionMatrix(elfCamera* camera) { return camera->projectionMatrix; }

float* elfGetCameraModelviewMatrix(elfCamera* camera)
{
    memcpy(camera->modelviewMatrix, gfxGetTransformMatrix(camera->transform), sizeof(float) * 16);
    return camera->modelviewMatrix;
}

void elfSetCamera(elfCamera* camera, gfxShaderParams* shaderParams)
{
    int viewpWidth, viewpHeight;
    float position[3];

    viewpWidth = camera->viewpWidth;
    viewpHeight = camera->viewpHeight;
    if (camera->viewpWidth <= 0)
        viewpWidth = elfGetWindowWidth();
    if (camera->viewpHeight <= 0)
        viewpHeight = elfGetWindowHeight();

    gfxSetViewport(camera->viewpX, camera->viewpY, viewpWidth, viewpHeight);

    memcpy(shaderParams->projectionMatrix, camera->projectionMatrix, sizeof(float) * 16);
    memcpy(camera->modelviewMatrix, gfxGetTransformMatrix(camera->transform), sizeof(float) * 16);
    memcpy(shaderParams->modelviewMatrix, camera->modelviewMatrix, sizeof(float) * 16);
    memcpy(shaderParams->cameraMatrix, camera->modelviewMatrix, sizeof(float) * 16);

    gfxGetTransformPosition(camera->transform, position);
    memcpy(&shaderParams->cameraPosition.x, position, sizeof(float) * 3);

    shaderParams->clipStart = camera->clipNear;
    shaderParams->clipEnd = camera->clipFar;
    shaderParams->viewportWidth = viewpWidth;
    shaderParams->viewportHeight = viewpHeight;
}

bool elfAabbInsideFrustum(elfCamera* camera, float* min, float* max)
{
    return gfxAabbInsideFrustum(camera->frustum, min, max);
}

bool elfSphereInsideFrustum(elfCamera* camera, float* pos, float radius)
{
    return gfxSphereInsideFrustum(camera->frustum, pos, radius);
}

bool elfCameraInsideAabb(elfCamera* camera, float* min, float* max)
{
    return camera->position.x > min[0] - camera->clipNear && camera->position.y > min[1] - camera->clipNear &&
           camera->position.z > min[2] - camera->clipNear && camera->position.x < max[0] + camera->clipNear &&
           camera->position.y < max[1] + camera->clipNear && camera->position.z < max[2] + camera->clipNear;
}

bool elfCameraInsideSphere(elfCamera* camera, float* pos, float radius)
{
    return camera->position.x > pos[0] - radius && camera->position.y > pos[1] - radius &&
           camera->position.z > pos[2] - radius && camera->position.x < pos[0] + radius &&
           camera->position.y < pos[1] + radius && camera->position.z < pos[2] + radius;
}

void elfDrawCameraDebug(elfCamera* camera, gfxShaderParams* shaderParams)
{
    float position[3];
    float rotation[3];
    gfxTransform* transform;
    int i;
    float step;
    float* vertexBuffer;

    transform = gfxCreateObjectTransform();

    gfxGetTransformPosition(camera->transform, position);
    gfxGetTransformRotation(camera->transform, rotation);
    gfxSetTransformPosition(transform, position[0], position[1], position[2]);
    gfxSetTransformRotation(transform, rotation[0], rotation[1], rotation[2]);

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(transform), shaderParams->cameraMatrix, shaderParams->modelviewMatrix);

    gfxDestroyTransform(transform);

    if (!camera->selected)
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.2f, 0.6f, 0.2f, 1.0f);
    else
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->lines);

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

    vertexBuffer[0] = -1.5f;
    vertexBuffer[1] = 0.0f;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = 1.5f;
    vertexBuffer[4] = 0.0f;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = 0.0f;
    vertexBuffer[7] = -1.5f;
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = 0.0f;
    vertexBuffer[10] = 1.5f;
    vertexBuffer[11] = 0.0f;
    vertexBuffer[12] = 0.0f;
    vertexBuffer[13] = 0.0f;
    vertexBuffer[14] = 0.5f;
    vertexBuffer[15] = 0.0f;
    vertexBuffer[16] = 0.0f;
    vertexBuffer[17] = -3.0f;

    elfDrawLines(6, rnd->lines);

    elfDrawActorDebug((elfActor*)camera, shaderParams);
}

elfVec3f elfProjectCameraPoint(elfCamera* camera, float x, float y, float z)
{
    elfVec3f result;
    int viewp[4];

    viewp[0] = camera->viewpX;
    viewp[1] = camera->viewpY;
    viewp[2] = camera->viewpWidth;
    viewp[3] = camera->viewpHeight;

    if (viewp[2] <= 0)
        viewp[2] = elfGetWindowWidth();
    if (viewp[3] <= 0)
        viewp[3] = elfGetWindowHeight();

    gfxProject(x, y, z, elfGetCameraModelviewMatrix(camera), elfGetCameraProjectionMatrix(camera), viewp, &result.x);

    return result;
}

elfVec3f elfUnProjectCameraPoint(elfCamera* camera, float x, float y, float z)
{
    elfVec3f result;
    int viewp[4];

    viewp[0] = camera->viewpX;
    viewp[1] = camera->viewpY;
    viewp[2] = camera->viewpWidth;
    viewp[3] = camera->viewpHeight;

    if (viewp[2] <= 0)
        viewp[2] = elfGetWindowWidth();
    if (viewp[3] <= 0)
        viewp[3] = elfGetWindowHeight();

    gfxUnProject(x, camera->viewpHeight - y, z, elfGetCameraModelviewMatrix(camera),
                 elfGetCameraProjectionMatrix(camera), viewp, &result.x);

    return result;
}
