#pragma once

#include "nelf/actor/actorHeader.h"

struct elfMaterial;
struct elfCamera;
struct gfxQuery;
struct gfxShaderParams;

struct elfSprite
{
    ELF_ACTOR_HEADER;

    elfMaterial* material = nullptr;
    elfFramePlayer* framePlayer = nullptr;

    bool faceCamera = false;

    elfVec3f position;
    elfVec2f scale;
    elfVec2f size;
    float cullRadius = 0.f;

    gfxQuery* query = nullptr;
    bool visible = true;
    bool occluder = false;
    bool culled = true;
};

elfSprite* elfCreateSprite(const char* name);

void elfUpdateSprite(elfSprite* sprite);

void elfSpritePreDraw(elfSprite* sprite, elfCamera* camera);
void elfSpritePostDraw(elfSprite* sprite);

// Destructor
void elfDestroySprite(void* data);

void elfCalcSpriteBounds(elfSprite* sprite);

void elfResetSpriteDebugPhysicsObject(elfSprite* sprite);

void elfSetSpriteMaterial(elfSprite* sprite, elfMaterial* material);
void elfSetSpriteScale(elfSprite* sprite, float x, float y);
void elfSetSpriteFaceCamera(elfSprite* sprite, bool faceCamera);

elfMaterial* elfGetSpriteMaterial(elfSprite* sprite);
elfVec2f elfGetSpriteScale(elfSprite* sprite);
bool elfGetSpriteFaceCamera(elfSprite* sprite);

void elfSetSpriteVisible(elfSprite* sprite, bool visible);
bool elfGetSpriteVisible(elfSprite* sprite);

void elfSetSpriteOccluder(elfSprite* sprite, bool occluder);
bool elfGetSpriteOccluder(elfSprite* sprite);

bool elfCullSprite(elfSprite* sprite, elfCamera* camera);

void elfDrawSprite(elfSprite* sprite, int mode, gfxShaderParams* shaderParams);
void elfDrawSpriteDebug(elfSprite* sprite, gfxShaderParams* shaderParams);