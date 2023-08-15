#include "nelf/actor/Sprite.h"

#include <cstdlib>
#include <cstring>

#include "nelf/FramePlayer.h"
#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/drawMode.h"
#include "nelf/gfx/gfxDrawMode.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxQuery.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTransform.h"
#include "nelf/gfx/gfxVertexArray.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/objectType.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Scene.h"
#include "nelf/resource/Texture.h"

elfSprite* elfCreateSprite(const char* name)
{
    elfSprite* sprite;

    sprite = (elfSprite*)malloc(sizeof(elfSprite));
    memset(sprite, 0x0, sizeof(elfSprite));
    sprite->objType = ELF_SPRITE;
    sprite->objDestr = elfDestroySprite;

    elfInitActor((elfActor*)sprite, false);

    sprite->scale.x = sprite->scale.y = 1.0f;
    sprite->size.x = sprite->size.y = 0.6f;
    sprite->query = gfxCreateQuery();
    sprite->visible = true;
    sprite->culled = true;

    sprite->dobject = elfCreatePhysicsObjectBox(0.3f, 0.3f, 0.01f, 0.0f, 0.0f, 0.0f, 0.0f);
    elfSetPhysicsObjectActor(sprite->dobject, (elfActor*)sprite);
    elfIncRef((elfObject*)sprite->dobject);

    sprite->pbbLengths.x = sprite->pbbLengths.y = 0.6f;
    sprite->pbbLengths.z = 0.01f;

    sprite->framePlayer = elfCreateFramePlayer();
    elfIncRef((elfObject*)sprite->framePlayer);

    if (name)
        sprite->name = elfCreateString(name);

    sprite->id = ++res->spriteIdCounter;

    elfIncObj(ELF_SPRITE);

    return sprite;
}

void elfUpdateSprite(elfSprite* sprite) { elfUpdateActor((elfActor*)sprite); }

void elfSpritePreDraw(elfSprite* sprite, elfCamera* camera)
{
    elfVec4f orient;

    elfActorPreDraw((elfActor*)sprite);

    elfGetActorPosition_((elfActor*)sprite, &sprite->position.x);

    if (sprite->faceCamera && camera)
    {
        elfGetActorOrientation_((elfActor*)camera, &orient.x);
        elfSetActorOrientation((elfActor*)sprite, orient.x, orient.y, orient.z, orient.w);
    }
}

void elfSpritePostDraw(elfSprite* sprite) { elfActorPostDraw((elfActor*)sprite); }

void elfDestroySprite(void* data)
{
    elfSprite* sprite = (elfSprite*)data;

    elfCleanActor((elfActor*)sprite);

    if (sprite->query)
        gfxDestroyQuery(sprite->query);

    if (sprite->material)
        elfDecRef((elfObject*)sprite->material);
    elfDecRef((elfObject*)sprite->framePlayer);

    free(sprite);

    elfDecObj(ELF_SPRITE);
}

void elfCalcSpriteBounds(elfSprite* sprite)
{
    if (sprite->material)
    {
        if (sprite->material->diffuseMap && sprite->material->diffuseMap->texture)
        {
            sprite->size.x = (float)gfxGetTextureWidth(sprite->material->diffuseMap->texture) / 100.0f;
            sprite->size.y = (float)gfxGetTextureHeight(sprite->material->diffuseMap->texture) / 100.0f;
        }
    }
    else
    {
        sprite->scale.x = sprite->scale.y = 1.0f;
        sprite->size.x = sprite->size.y = 0.6f;
        sprite->pbbLengths.x = sprite->pbbLengths.y = 0.6f;
        sprite->pbbLengths.z = 0.01f;
        sprite->cullRadius = 0.3f;
        return;
    }

    sprite->pbbLengths.x = sprite->size.x;
    sprite->pbbLengths.y = sprite->size.y;
    sprite->pbbLengths.z = 0.01f;

    sprite->cullRadius = elfGetVec2fLength(elfMulVec2fVec2f(sprite->size, sprite->scale)) / 2.0f;
}

void elfResetSpriteDebugPhysicsObject(elfSprite* sprite)
{
    float position[3];
    float orient[4];
    float scale[3];

    if (sprite->dobject)
    {
        elfSetPhysicsObjectActor(sprite->dobject, NULL);
        elfSetPhysicsObjectWorld(sprite->dobject, NULL);
        elfDecRef((elfObject*)sprite->dobject);
    }

    sprite->dobject =
        elfCreatePhysicsObjectBox(sprite->size.x / 2.0f, sprite->size.y / 2.0f, 0.01f / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    elfSetPhysicsObjectActor(sprite->dobject, (elfActor*)sprite);
    elfIncRef((elfObject*)sprite->dobject);

    gfxGetTransformPosition(sprite->transform, position);
    gfxGetTransformOrientation(sprite->transform, orient);
    gfxGetTransformScale(sprite->transform, scale);

    elfSetPhysicsObjectPosition(sprite->dobject, position[0], position[1], position[2]);
    elfSetPhysicsObjectOrientation(sprite->dobject, orient[0], orient[1], orient[2], orient[3]);
    elfSetPhysicsObjectScale(sprite->dobject, scale[0], scale[1], scale[2]);

    if (sprite->scene)
        elfSetPhysicsObjectWorld(sprite->dobject, sprite->scene->dworld);
}

void elfSetSpriteMaterial(elfSprite* sprite, elfMaterial* material)
{
    if (sprite->material)
        elfDecRef((elfObject*)sprite->material);
    sprite->material = material;
    if (sprite->material)
        elfIncRef((elfObject*)sprite->material);

    elfCalcSpriteBounds(sprite);

    if (sprite->object)
        elfSetActorPhysics((elfActor*)sprite, true);

    elfResetSpriteDebugPhysicsObject(sprite);
}

void elfSetSpriteScale(elfSprite* sprite, float x, float y)
{
    sprite->scale.x = x;
    sprite->scale.y = y;

    elfCalcSpriteBounds(sprite);

    gfxSetTransformScale(sprite->transform, sprite->scale.x, sprite->scale.y, 1.0);

    if (sprite->object)
        elfSetPhysicsObjectScale(sprite->object, sprite->scale.x, sprite->scale.y, 1.0f);
    if (sprite->dobject)
        elfSetPhysicsObjectScale(sprite->dobject, sprite->scale.x, sprite->scale.y, 1.0f);
}

void elfSetSpriteFaceCamera(elfSprite* sprite, bool faceCamera) { sprite->faceCamera = faceCamera; }

elfMaterial* elfGetSpriteMaterial(elfSprite* sprite) { return sprite->material; }

elfVec2f elfGetSpriteScale(elfSprite* sprite) { return sprite->scale; }

bool elfGetSpriteFaceCamera(elfSprite* sprite) { return sprite->faceCamera; }

void elfSetSpriteVisible(elfSprite* sprite, bool visible)
{
    // No change
    if (sprite->visible == visible)
        return;

    sprite->visible = visible;
    sprite->moved = true;
}

bool elfGetSpriteVisible(elfSprite* sprite) { return sprite->visible; }

void elfSetSpriteOccluder(elfSprite* sprite, bool occluder) { sprite->occluder = occluder; }

bool elfGetSpriteOccluder(elfSprite* sprite) { return sprite->occluder; }

bool elfCullSprite(elfSprite* sprite, elfCamera* camera)
{
    if (!sprite->material || !sprite->visible)
        return true;

    return !elfSphereInsideFrustum(camera, &sprite->position.x, sprite->cullRadius);
}

void elfDrawSprite(elfSprite* sprite, int mode, gfxShaderParams* shaderParams)
{
    float* vertexBuffer = nullptr;
    float* texCoordBuffer = nullptr;
    float* normalBuffer = nullptr;
    float sizex, sizey;

    if (!sprite->material || !sprite->visible || (mode == ELF_DRAW_WITHOUT_LIGHTING && sprite->material->lighting))
        return;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(sprite->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);
    gfxMulMatrix3Matrix4(gfxGetTransformNormalMatrix(sprite->transform), shaderParams->cameraMatrix,
                         shaderParams->normalMatrix);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(renderStation->quadVertexData);
    texCoordBuffer = (float*)gfxGetVertexDataBuffer(renderStation->quadTexCoordData);
    normalBuffer = (float*)gfxGetVertexDataBuffer(renderStation->quadNormalData);

    sizex = sprite->size.x / 2.0f;
    sizey = sprite->size.y / 2.0f;

    vertexBuffer[0] = -sizex;
    vertexBuffer[1] = sizey;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = -sizex;
    vertexBuffer[4] = -sizey;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = sizex;
    vertexBuffer[7] = sizey;
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = sizex;
    vertexBuffer[10] = -sizey;
    vertexBuffer[11] = 0.0f;

    texCoordBuffer[0] = 0.0f;
    texCoordBuffer[1] = 1.0f;
    texCoordBuffer[2] = 0.0f;
    texCoordBuffer[3] = 0.0f;
    texCoordBuffer[4] = 1.0f;
    texCoordBuffer[5] = 1.0f;
    texCoordBuffer[6] = 1.0f;
    texCoordBuffer[7] = 0.0f;

    normalBuffer[0] = 0.0f;
    normalBuffer[1] = 0.0f;
    normalBuffer[2] = 1.0f;
    normalBuffer[3] = 0.0f;
    normalBuffer[4] = 0.0f;
    normalBuffer[5] = 1.0f;
    normalBuffer[6] = 0.0f;
    normalBuffer[7] = 0.0f;
    normalBuffer[8] = 1.0f;
    normalBuffer[9] = 0.0f;
    normalBuffer[10] = 0.0f;
    normalBuffer[11] = 1.0f;

    gfxUpdateVertexData(renderStation->quadVertexData);
    gfxUpdateVertexData(renderStation->quadTexCoordData);
    gfxUpdateVertexData(renderStation->quadNormalData);

    elfSetMaterial(sprite->material, mode, shaderParams);
    gfxSetShaderParams(shaderParams);

    gfxDrawVertexArray(renderStation->quadVertexArray, 4, GFX_TRIANGLE_STRIP);
}

void elfDrawSpriteDebug(elfSprite* sprite, gfxShaderParams* shaderParams)
{
    float* vertexBuffer;
    float sizex, sizey;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(sprite->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);

    sizex = sprite->size.x / 2.0f;
    sizey = sprite->size.y / 2.0f;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(renderStation->lines);

    vertexBuffer[0] = -sizex;
    vertexBuffer[1] = sizey;
    vertexBuffer[2] = 0.0f;

    vertexBuffer[3] = -sizex;
    vertexBuffer[4] = -sizey;
    vertexBuffer[5] = 0.0f;

    vertexBuffer[6] = sizex;
    vertexBuffer[7] = -sizey;
    vertexBuffer[8] = 0.0f;

    vertexBuffer[9] = sizex;
    vertexBuffer[10] = sizey;
    vertexBuffer[11] = 0.0f;

    vertexBuffer[12] = -sizex;
    vertexBuffer[13] = sizey;
    vertexBuffer[14] = 0.0f;

    if (!sprite->selected)
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.6f, 0.2f, 0.6f, 1.0f);
    else
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);
    elfDrawLineLoop(5, renderStation->lines);

    elfDrawActorDebug((elfActor*)sprite, shaderParams);
}
