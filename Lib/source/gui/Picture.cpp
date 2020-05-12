#include "nelf/gui/Picture.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/objectType.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Texture.h"

elfPicture* elfCreatePicture(const char* name)
{
    elfPicture* picture;

    picture = (elfPicture*)malloc(sizeof(elfPicture));
    memset(picture, 0x0, sizeof(elfPicture));
    picture->objType = ELF_PICTURE;
    picture->objDestr = elfDestroyPicture;

    picture->color.r = picture->color.g = picture->color.b = picture->color.a = 1.0f;
    picture->scale.x = picture->scale.y = 1.0f;
    picture->visible = true;

    if (name)
        picture->name = elfCreateString(name);

    elfIncObj(ELF_PICTURE);

    return picture;
}

elfPicture* elfCreatePicture(elfGuiObject* parent, const char* name, int x, int y, const char* path)
{
    auto picture = elfCreatePicture(name);

    elfAddGuiObject(parent, (elfGuiObject*)picture);
    elfSetGuiObjectPosition((elfGuiObject*)picture, x, y);
    auto texture = elfGetOrLoadResourcesTexture(path, NULL);
    if (texture)
        elfSetPictureTexture(picture, texture);

    return picture;
}

void elfDestroyPicture(void* data)
{
    elfPicture* picture = (elfPicture*)data;

    if (picture->name)
        elfDestroyString(picture->name);
    if (picture->texture)
        elfDecRef((elfObject*)picture->texture);

    free(picture);

    elfDecObj(ELF_PICTURE);
}

void elfDrawPicture(elfPicture* picture, gfxShaderParams* shaderParams)
{
    if (!picture->visible || !picture->texture)
        return;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, picture->color.r, picture->color.g, picture->color.b,
                picture->color.a);

    shaderParams->textureParams[0].texture = picture->texture->texture;
    gfxSetShaderParams(shaderParams);
    elfDrawTextured2dQuad((float)picture->pos.x, (float)picture->pos.y, (float)picture->width, (float)picture->height);
    shaderParams->textureParams[0].texture = NULL;
}

void elfRecalcPicture(elfPicture* picture)
{
    if (picture->texture)
    {
        picture->width = elfGetTextureWidth(picture->texture) * picture->scale.x;
        picture->height = elfGetTextureHeight(picture->texture) * picture->scale.y;
    }
    else
    {
        picture->height = 0;
        picture->width = 0;
    }
}

elfTexture* elfGetPictureTexture(elfPicture* picture) { return picture->texture; }

elfVec2f elfGetPictureScale(elfPicture* picture) { return picture->scale; }

void elfSetPictureTexture(elfPicture* picture, elfTexture* texture)
{
    if (picture->texture)
        elfDecRef((elfObject*)picture->texture);
    picture->texture = texture;
    if (picture->texture)
        elfIncRef((elfObject*)picture->texture);
    elfRecalcGuiObject((elfGuiObject*)picture);
}

void elfSetPictureScale(elfPicture* picture, float x, float y)
{
    picture->scale.x = x;
    picture->scale.y = y;
    elfRecalcGuiObject((elfGuiObject*)picture);
}