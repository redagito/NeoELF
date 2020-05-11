#include "nelf/gui/TextField.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/Area.h"
#include "nelf/gui/Gui.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/objectType.h"
#include "nelf/resource/Texture.h"

elfTextField* elfCreateTextField(const char* name)
{
    elfTextField* textField;

    textField = (elfTextField*)malloc(sizeof(elfTextField));
    memset(textField, 0x0, sizeof(elfTextField));
    textField->objType = ELF_TEXT_FIELD;
    textField->objDestr = elfDestroyTextField;

    textField->color.r = textField->color.g = textField->color.b = textField->color.a = 1.0f;
    textField->textColor.r = textField->textColor.g = textField->textColor.b = 1.0f;
    textField->textColor.a = 0.6f;
    textField->visible = true;
    textField->text = elfCreateString("");

    if (name)
        textField->name = elfCreateString(name);

    elfIncObj(ELF_TEXT_FIELD);

    return textField;
}

void elfDestroyTextField(void* data)
{
    elfTextField* textField = (elfTextField*)data;

    if (textField->name)
        elfDestroyString(textField->name);

    if (textField->texture)
        elfDecRef((elfObject*)textField->texture);
    if (textField->font)
        elfDecRef((elfObject*)textField->font);
    if (textField->text)
        elfDestroyString(textField->text);
    if (textField->script)
        elfDecRef((elfObject*)textField->script);

    free(textField);

    elfDecObj(ELF_TEXT_FIELD);
}

void elfDrawTextField(elfTextField* textField, elfArea* area, gfxShaderParams* shaderParams)
{
    char* str;
    int x, y, width, height;

    if (!textField->visible)
        return;

    x = textField->pos.x;
    y = textField->pos.y;
    width = textField->width;
    height = textField->height;
    if (textField->parent)
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

    if (!textField->texture)
    {
        elfColor col1, col2;

        shaderParams->renderParams.vertexColor = true;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
        gfxSetShaderParams(shaderParams);

        col1.r = col1.g = col1.b = 0.15f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.20f;
        col2.a = 1.0f;
        elfDrawHorGradient(textField->pos.x, textField->pos.y, textField->width, textField->height, col1, col2);

        col1.r = col1.g = col1.b = 0.25f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.30f;
        col2.a = 1.0f;
        elfDrawHorGradientBorder(textField->pos.x, textField->pos.y, textField->width, textField->height, col1, col2);

        shaderParams->renderParams.vertexColor = false;
    }
    else
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, textField->color.r, textField->color.g,
                    textField->color.b, textField->color.a);
        shaderParams->textureParams[0].texture = textField->texture->texture;

        if (shaderParams->textureParams[0].texture)
        {
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)textField->pos.x, (float)textField->pos.y, (float)textField->width,
                                  (float)textField->height);
            shaderParams->textureParams[0].texture = NULL;
        }
    }

    if (textField->font && textField->text && strlen(textField->text) > 0)
    {
        elfSetOrtho(x, y, width, height, shaderParams);

        gfxSetColor(&shaderParams->materialParams.diffuseColor, textField->textColor.r, textField->textColor.g,
                    textField->textColor.b, textField->textColor.a * textField->color.a);

        str = elfSubString(textField->text, textField->drawPos, strlen(textField->text) - textField->drawPos);
        elfDrawString(textField->font, str, textField->pos.x + textField->offsetX,
                      textField->pos.y + textField->offsetY - textField->font->offsetY / 2, shaderParams);
        elfDestroyString(str);

        shaderParams->textureParams[0].texture = NULL;
    }

    if (textField->text && textField->root && textField->root->activeTextField == textField)
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, textField->textColor.r, textField->textColor.g,
                    textField->textColor.b, textField->textColor.a * textField->color.a);
        gfxSetShaderParams(shaderParams);

        str = elfSubString(textField->text, textField->drawPos, textField->cursorPos - textField->drawPos);
        elfDraw2dQuad(textField->pos.x + textField->offsetX + elfGetStringWidth(textField->font, str),
                      textField->pos.y + textField->offsetY, 1, textField->height - textField->offsetY * 2);
        elfDestroyString(str);
    }
}

void elfRecalcTextField(elfTextField* textField)
{
    if (textField->texture)
    {
        textField->width = elfGetTextureWidth(textField->texture);
        textField->height = elfGetTextureHeight(textField->texture);
    }
}

elfTexture* elfGetTextFieldTexture(elfTextField* textField) { return textField->texture; }

elfFont* elfGetTextFieldFont(elfTextField* textField) { return textField->font; }

elfColor elfGetTextFieldTextColor(elfTextField* textField) { return textField->textColor; }

elfVec2i elfGetTextFieldOffset(elfTextField* textField)
{
    elfVec2i offset;

    offset.x = textField->offsetX;
    offset.y = textField->offsetY;

    return offset;
}

const char* elfGetTextFieldText(elfTextField* textField) { return textField->text; }

void elfSetTextFieldFont(elfTextField* textField, elfFont* font)
{
    if (textField->font)
        elfDecRef((elfObject*)textField->font);
    textField->font = font;
    if (textField->font)
    {
        elfIncRef((elfObject*)textField->font);
        if (!textField->texture)
            textField->height = textField->font->size + textField->font->size / 2;
    }
    else if (!textField->texture)
        textField->height = 0;
    elfRecalcGuiObject((elfGuiObject*)textField);
}

void elfSetTextFieldWidth(elfTextField* textField, int width)
{
    textField->width = width;
    elfRecalcGuiObject((elfGuiObject*)textField);
}

void elfSetTextFieldTexture(elfTextField* textField, elfTexture* texture)
{
    if (textField->texture)
        elfDecRef((elfObject*)textField->texture);
    textField->texture = texture;
    if (textField->texture)
        elfIncRef((elfObject*)textField->texture);
    else if (textField->font)
        textField->height = textField->font->size + textField->font->size / 2;
    elfRecalcGuiObject((elfGuiObject*)textField);
}

void elfSetTextFieldTextColor(elfTextField* textField, float r, float g, float b, float a)
{
    textField->textColor.r = r;
    textField->textColor.g = g;
    textField->textColor.b = b;
    textField->textColor.a = a;
}

void elfSetTextFieldOffset(elfTextField* textField, int offsetX, int offsetY)
{
    textField->offsetX = offsetX;
    textField->offsetY = offsetY;
}

void elfMoveTextFieldCursorLeft(elfTextField* textField)
{
    if (textField->cursorPos == 0)
        return;

    textField->cursorPos--;

    if (textField->cursorPos == textField->drawPos && textField->drawPos > 0)
    {
        textField->drawPos--;
    }
}

void elfMoveTextFieldCursorRight(elfTextField* textField)
{
    char* str;

    if (textField->cursorPos >= (int)strlen(textField->text))
        return;

    textField->cursorPos++;

    if (!textField->font)
        return;

    str = elfSubString(textField->text, textField->drawPos, textField->cursorPos - textField->drawPos);
    if (elfGetStringWidth(textField->font, str) + 2 > textField->width - textField->offsetX * 2)
    {
        if (textField->drawPos + 5 > textField->cursorPos)
            textField->drawPos += textField->cursorPos - 1;
        else
            textField->drawPos += 5;
    }

    elfDestroyString(str);
}

void elfSetTextFieldCursorPosition(elfTextField* textField, int idx)
{
    if (idx < 0)
        return;
    if (idx > (int)strlen(textField->text))
        idx = strlen(textField->text);

    textField->cursorPos = 0;
    textField->drawPos = 0;

    while (textField->cursorPos < idx) elfMoveTextFieldCursorRight(textField);
}

void elfSetTextFieldText(elfTextField* textField, const char* text)
{
    if (textField->text)
        elfDestroyString(textField->text);
    textField->text = elfCreateString(text);
    textField->cursorPos = 0;
    textField->drawPos = 0;

    while (textField->cursorPos < (int)strlen(textField->text)) elfMoveTextFieldCursorRight(textField);
}