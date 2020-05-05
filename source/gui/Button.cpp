#include "nelf/gui/Button.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Engine.h"
#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/String.h"
#include "nelf/Texture.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/gui/buttonState.h"
#include "nelf/objectType.h"

elfButton* elfCreateButton(const char* name)
{
    elfButton* button;
    int sizeX;
    int sizeY;

    button = (elfButton*)malloc(sizeof(elfButton));
    memset(button, 0x0, sizeof(elfButton));
    button->objType = ELF_BUTTON;
    button->objDestr = elfDestroyButton;

    button->color.r = button->color.g = button->color.b = button->color.a = 1.0f;
    button->textColor.r = button->textColor.g = button->textColor.b = 1.0f;
    button->textColor.a = 0.9f;
    elfSetButtonFont(button, eng->guiFont);
    button->visible = true;
    button->active = true;

    if (name)
        button->name = elfCreateString(name);

    // TODO: add method size
    sizeX = 10;
    sizeY = 10;
    button->width = sizeX;
    button->height = sizeY;

    elfIncObj(ELF_BUTTON);

    return button;
}

void elfDestroyButton(void* data)
{
    elfButton* button = (elfButton*)data;

    if (button->name)
        elfDestroyString(button->name);

    if (button->text)
        elfDestroyString(button->text);
    if (button->font)
        elfDecRef((elfObject*)button->font);
    if (button->off)
        elfDecRef((elfObject*)button->off);
    if (button->over)
        elfDecRef((elfObject*)button->over);
    if (button->on)
        elfDecRef((elfObject*)button->on);

    if (button->script)
        elfDecRef((elfObject*)button->script);

    free(button);

    elfDecObj(ELF_BUTTON);
}

void elfDrawButton(elfButton* button, gfxShaderParams* shaderParams)
{
    if (!button->visible)
        return;

    if (!button->off)
    {
        elfColor col1, col2;
        col1.r = col1.g = col1.b = col1.a = 1.0f;
        col2.r = col2.g = col2.b = col2.a = 1.0f;

        // TODO Bug?
        shaderParams->renderParams.vertexColor = ELF_TRUE;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
        gfxSetShaderParams(shaderParams);

        if (button->state == ELF_OFF)
        {
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.25f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_OVER)
        {
            col1.r = col1.g = col1.b = 0.50f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.35f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_ON)
        {
            col1.r = col1.g = col1.b = 0.35f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.20f;
            col2.a = 1.0f;
        }
        elfDrawHorGradient(button->pos.x, button->pos.y + button->height / 2, button->width, button->height / 2, col1,
                           col2);

        if (button->state == ELF_OFF)
        {
            col1.r = col1.g = col1.b = 0.20f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.10f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_OVER)
        {
            col1.r = col1.g = col1.b = 0.30f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.20f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_ON)
        {
            col1.r = col1.g = col1.b = 0.15f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.05f;
            col2.a = 1.0f;
        }
        elfDrawHorGradient(button->pos.x, button->pos.y, button->width, button->height / 2, col1, col2);

        if (button->state == ELF_OFF)
        {
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.20f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_OVER)
        {
            col1.r = col1.g = col1.b = 0.50f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.30f;
            col2.a = 1.0f;
        }
        else if (button->state == ELF_ON)
        {
            col1.r = col1.g = col1.b = 0.35f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.15f;
            col2.a = 1.0f;
        }
        elfDrawHorGradientBorder(button->pos.x, button->pos.y, button->width, button->height, col1, col2);

        shaderParams->renderParams.vertexColor = ELF_FALSE;
        if (button->active)
            gfxSetColor(&shaderParams->materialParams.diffuseColor, button->textColor.r, button->textColor.g,
                        button->textColor.b, button->textColor.a);
        else
            gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.5f, 0.5f, 0.5f, 0.6f);
        gfxSetShaderParams(shaderParams);

        elfDrawString(button->font, button->text,
                      button->pos.x + (button->width - elfGetStringWidth(button->font, button->text)) / 2,
                      button->pos.y + (button->height - elfGetStringHeight(button->font, button->text)) / 2 -
                          button->font->offsetY / 2,
                      shaderParams);

        shaderParams->textureParams[0].texture = NULL;
    }
    else
    {
        shaderParams->textureParams[0].texture = button->off->texture;

        if (button->state == ELF_OVER)
        {
            if (button->over)
                shaderParams->textureParams[0].texture = button->over->texture;
        }
        else if (button->state == ELF_ON)
        {
            if (button->on)
                shaderParams->textureParams[0].texture = button->on->texture;
        }

        if (shaderParams->textureParams[0].texture)
        {
            gfxSetColor(&shaderParams->materialParams.diffuseColor, button->color.r, button->color.g, button->color.b,
                        button->color.a);
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)button->pos.x, (float)button->pos.y, (float)button->width,
                                  (float)button->height);
            shaderParams->textureParams[0].texture = NULL;
        }
        if (button->text)
        {
            shaderParams->renderParams.vertexColor = ELF_FALSE;
            gfxSetColor(&shaderParams->materialParams.diffuseColor, button->textColor.r, button->textColor.g,
                        button->textColor.b, button->textColor.a);
            gfxSetShaderParams(shaderParams);
            elfDrawString(button->font, button->text,
                          button->pos.x + (button->width - elfGetStringWidth(button->font, button->text)) / 2,
                          button->pos.y + (button->height - elfGetStringHeight(button->font, button->text)) / 2 -
                              button->font->offsetY / 2,
                          shaderParams);
        }
    }
}

void elfRecalcButton(elfButton* button)
{
    if (button->off)
    {
        button->width = elfGetTextureWidth(button->off);
        button->height = elfGetTextureHeight(button->off);
    }
}

unsigned char elfGetButtonState(elfButton* button) { return button->state; }

const char* elfGetButtonText(elfButton* button) { return button->text; }

elfFont* elfGetButtonFont(elfButton* button) { return button->font; }

elfVec2i elfGetButtonSize(elfButton* button)
{
    elfVec2i size;

    size.x = button->width;
    size.y = button->height;

    return size;
}

elfTexture* elfGetButtonOffTexture(elfButton* button) { return button->off; }

elfTexture* elfGetButtonOverTexture(elfButton* button) { return button->over; }

elfTexture* elfGetButtonOnTexture(elfButton* button) { return button->on; }

elfColor elfGetButtonTextColor(elfButton* button) { return button->textColor; }

void elfSetButtonTextColor(elfButton* button, float r, float g, float b, float a)
{
    button->textColor.r = r;
    button->textColor.g = g;
    button->textColor.b = b;
    button->textColor.a = a;
}

void elfSetButtonText(elfButton* button, const char* text)
{
    if (button->text)
        elfDestroyString(button->text);
    button->text = elfCreateString(text);
    elfRecalcGuiObject((elfGuiObject*)button);
}

void elfSetButtonFont(elfButton* button, elfFont* font)
{
    if (button->font)
        elfDecRef((elfObject*)button->font);
    button->font = font;
    if (button->font)
        elfIncRef((elfObject*)button->font);
    elfRecalcGuiObject((elfGuiObject*)button);
}

void elfSetButtonSize(elfButton* button, int width, int height)
{
    button->width = width;
    button->height = height;
    if (button->width < 1)
        button->width = 1;
    if (button->height < 1)
        button->height = 1;
    elfRecalcGuiObject((elfGuiObject*)button);
}

void elfSetButtonOffTexture(elfButton* button, elfTexture* off)
{
    if (button->off)
        elfDecRef((elfObject*)button->off);
    button->off = off;
    if (button->off)
        elfIncRef((elfObject*)button->off);
    elfSetButtonSize(button, elfGetTextureWidth(off), elfGetTextureHeight(off));
    elfRecalcGuiObject((elfGuiObject*)button);
}

void elfSetButtonOverTexture(elfButton* button, elfTexture* over)
{
    if (button->over)
        elfDecRef((elfObject*)button->over);
    button->over = over;
    if (button->over)
        elfIncRef((elfObject*)button->over);
}

void elfSetButtonOnTexture(elfButton* button, elfTexture* on)
{
    if (button->on)
        elfDecRef((elfObject*)button->on);
    button->on = on;
    if (button->on)
        elfIncRef((elfObject*)button->on);
}