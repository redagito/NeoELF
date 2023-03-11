#include "nelf/gui/Label.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Engine.h"
#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/objectType.h"

elfLabel* elfCreateLabel(const char* name)
{
    elfLabel* label;

    label = (elfLabel*)malloc(sizeof(elfLabel));
    memset(label, 0x0, sizeof(elfLabel));
    label->objType = ELF_LABEL;
    label->objDestr = elfDestroyLabel;

    label->color.r = label->color.g = label->color.b = label->color.a = 1.0f;
    label->visible = true;

    if (name)
        label->name = elfCreateString(name);

    elfSetLabelFont(label, eng->guiFont);
    elfIncObj(ELF_LABEL);

    return label;
}

elfLabel* elfCreateLabel(elfGuiObject* parent, const char* name, int x, int y, const char* text)
{
    auto label = elfCreateLabel(name);

    elfAddGuiObject(parent, (elfGuiObject*)label);
    elfSetGuiObjectPosition((elfGuiObject*)label, x, y);
    elfSetLabelText(label, text);

    return label;
}

void elfDestroyLabel(void* data)
{
    elfLabel* label = (elfLabel*)data;

    if (label->name)
        elfDestroyString(label->name);
    if (label->text)
        elfDestroyString(label->text);

    if (label->font)
        elfDecRef((elfObject*)label->font);

    free(label);

    elfDecObj(ELF_LABEL);
}

void elfDrawLabel(elfLabel* label, gfxShaderParams* shaderParams)
{
    if (!label->visible || !label->font || !label->text)
        return;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, label->color.r, label->color.g, label->color.b,
                label->color.a);
    elfDrawString(label->font, label->text, label->pos.x, label->pos.y, shaderParams);
    shaderParams->textureParams[0].texture = nullptr;
}

void elfRecalcLabel(elfLabel* label)
{
    if (label->font)
    {
        label->height = label->font->size;
        if (label->text)
        {
            label->width = elfGetStringWidth(label->font, label->text);
        }
    }
    else
    {
        label->height = 0;
        label->width = 0;
    }
}

elfFont* elfGetLabelFont(elfLabel* label) { return label->font; }

const char* elfGetLabelText(elfLabel* label) { return label->text; }

void elfSetLabelFont(elfLabel* label, elfFont* font)
{
    if (label->font)
        elfDecRef((elfObject*)label->font);
    label->font = font;
    if (label->font)
        elfIncRef((elfObject*)label->font);
    elfRecalcGuiObject((elfGuiObject*)label);
}

void elfSetLabelText(elfLabel* label, const char* text)
{
    if (label->text)
        elfDestroyString(label->text);
    label->text = elfCreateString(text);
    elfRecalcGuiObject((elfGuiObject*)label);
}