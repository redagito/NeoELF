#include "nelf/gui/Screen.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/Area.h"
#include "nelf/gui/Button.h"
#include "nelf/gui/CheckBox.h"
#include "nelf/gui/Gui.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/gui/Label.h"
#include "nelf/gui/Picture.h"
#include "nelf/gui/Slider.h"
#include "nelf/gui/TextField.h"
#include "nelf/gui/TextList.h"
#include "nelf/gui/buttonState.h"
#include "nelf/objectType.h"
#include "nelf/resource/Texture.h"

elfScreen* elfCreateScreen(const char* name)
{
    elfScreen* screen;

    screen = (elfScreen*)malloc(sizeof(elfScreen));
    memset(screen, 0x0, sizeof(elfScreen));
    screen->objType = ELF_SCREEN;
    screen->objDestr = elfDestroyScreen;

    screen->color.r = screen->color.g = screen->color.b = screen->color.a = 1.0f;
    screen->visible = true;

    screen->children = elfCreateList();
    screen->screens = elfCreateList();

    elfIncRef((elfObject*)screen->children);
    elfIncRef((elfObject*)screen->screens);

    if (name)
        screen->name = elfCreateString(name);

    elfIncObj(ELF_SCREEN);

    screen->hack = 0;

    return screen;
}

void elfDestroyScreen(void* data)
{
    elfScreen* screen = (elfScreen*)data;

    if (screen->name)
        elfDestroyString(screen->name);
    if (screen->texture)
        elfDecRef((elfObject*)screen->texture);

    elfDecRef((elfObject*)screen->children);
    elfDecRef((elfObject*)screen->screens);

    free(screen);

    elfDecObj(ELF_SCREEN);
}

void elfDrawScreen(elfScreen* screen, elfArea* area, gfxShaderParams* shaderParams)
{
    elfGuiObject* object;
    int x, y, width, height;
    elfArea narea;

    if (!screen->visible)
        return;

    x = screen->pos.x;
    y = screen->pos.y;
    width = screen->width;
    height = screen->height;
    if (screen->parent)
    {
        if (x < area->pos.x)
        {
            width -= area->pos.x - x;
            x = area->pos.x;
        }
        if (y < area->pos.y)
        {
            height -= area->pos.y - y;
            y = area->pos.y;
        }
        if (x + width > area->pos.x + area->size.x)
            width -= (x + width) - (area->pos.x + area->size.x);
        if (y + height > area->pos.y + area->size.y)
            height -= (y + height) - (area->pos.y + area->size.y);
    }

    narea.pos.x = x;
    narea.pos.y = y;
    narea.size.x = width;
    narea.size.y = height;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, screen->color.r, screen->color.g, screen->color.b,
                screen->color.a);

    if (!screen->texture)
    {
        elfColor col1, col2;

        shaderParams->renderParams.vertexColor = true;
        gfxSetShaderParams(shaderParams);

        col1.r = col1.g = col1.b = 0.10f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.10f;
        col2.a = 1.0f;
        elfDrawHorGradient(screen->pos.x, screen->pos.y + screen->height / 3, screen->width, screen->height / 3 * 2,
                           col1, col2);

        col1.r = col1.g = col1.b = 0.10f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.15f;
        col2.a = 1.0f;
        elfDrawHorGradient(screen->pos.x, screen->pos.y, screen->width, screen->height / 3, col1, col2);

        shaderParams->renderParams.vertexColor = false;
    }
    else
    {
        shaderParams->textureParams[0].texture = screen->texture->texture;
        if (shaderParams->textureParams[0].texture)
        {
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)screen->pos.x, (float)screen->pos.y, (float)screen->width,
                                  (float)screen->height);

            shaderParams->textureParams[0].texture = NULL;
        }
    }

    elfSetOrtho(x, y, width, height, shaderParams);

    for (object = (elfGuiObject*)elfBeginList(screen->children); object;
         object = (elfGuiObject*)elfGetListNext(screen->children))
    {
        if (object->objType == ELF_LABEL)
            elfDrawLabel((elfLabel*)object, shaderParams);
        else if (object->objType == ELF_BUTTON)
            elfDrawButton((elfButton*)object, shaderParams);
        else if (object->objType == ELF_PICTURE)
            elfDrawPicture((elfPicture*)object, shaderParams);
        else if (object->objType == ELF_TEXT_FIELD)
        {
            elfDrawTextField((elfTextField*)object, &narea, shaderParams);
            elfSetOrtho(x, y, width, height, shaderParams);
        }
        else if (object->objType == ELF_TEXT_LIST)
        {
            elfDrawTextList((elfTextList*)object, &narea, shaderParams);
            elfSetOrtho(x, y, width, height, shaderParams);
        }
        else if (object->objType == ELF_SLIDER)
            elfDrawSlider((elfSlider*)object, shaderParams);
        else if (object->objType == ELF_CHECK_BOX)
            elfDrawCheckBox((elfCheckBox*)object, shaderParams);
    }

    for (object = (elfGuiObject*)elfBeginList(screen->screens); object;
         object = (elfGuiObject*)elfGetListNext(screen->screens))
    {
        elfDrawScreen((elfScreen*)object, &narea, shaderParams);
        elfSetOrtho(x, y, width, height, shaderParams);
    }

    if (!screen->texture)
    {
        elfColor col1, col2;

        elfSetOrtho(area->pos.x, area->pos.y, area->size.x, area->size.y, shaderParams);

        shaderParams->renderParams.vertexColor = true;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, screen->color.r, screen->color.g, screen->color.b,
                    screen->color.a);
        gfxSetShaderParams(shaderParams);

        col1.r = col1.g = col1.b = 0.15f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.15f;
        col2.a = 1.0f;
        elfDrawHorGradientBorder(screen->pos.x, screen->pos.y, screen->width, screen->height, col1, col2);

        shaderParams->renderParams.vertexColor = false;
    }
}

elfTexture* elfGetScreenTexture(elfScreen* screen) { return screen->texture; }

void elfRecalcScreen(elfScreen* screen)
{
    if (screen->texture && screen->hack == 0)
    {
        screen->width = elfGetTextureWidth(screen->texture);
        screen->height = elfGetTextureHeight(screen->texture);
    }
}

void elfSetScreenSize(elfScreen* screen, int width, int height)
{
    // TODO ?????
    screen->hack = 1;
    screen->width = width;
    screen->height = height;
    if (screen->width < 0)
        screen->width = 0;
    if (screen->height < 0)
        screen->height = 0;
    elfRecalcGuiObject((elfGuiObject*)screen);
}

void elfSetScreenTexture(elfScreen* screen, elfTexture* texture)
{
    if (screen->texture)
        elfDecRef((elfObject*)screen->texture);
    screen->texture = texture;
    if (screen->texture)
        elfIncRef((elfObject*)screen->texture);
    elfRecalcGuiObject((elfGuiObject*)screen);
}

void elfSetScreenToTop(elfScreen* screen)
{
    if (!screen->parent)
        return;

    elfIncRef((elfObject*)screen);
    elfRemoveListObject(screen->parent->screens, (elfObject*)screen);
    elfAppendListObject(screen->parent->screens, (elfObject*)screen);
    elfDecRef((elfObject*)screen);
}

void elfForceScreenFocus(elfScreen* screen)
{
    elfButton* button;

    if (!screen->root)
        return;

    if (screen->root->target && screen->root->target->objType == ELF_BUTTON)
    {
        button = (elfButton*)screen->root->target;
        button->state = ELF_OFF;
    }

    screen->root->trace = NULL;
    screen->root->target = NULL;
    screen->root->activeTextField = NULL;

    screen->root->focusScreen = screen;
}

void elfReleaseScreenFocus(elfScreen* screen)
{
    if (!screen->root)
        return;

    screen->root->focusScreen = NULL;
}