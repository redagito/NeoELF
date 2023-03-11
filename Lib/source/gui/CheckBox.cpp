#include "nelf/gui/CheckBox.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/gui/buttonState.h"
#include "nelf/objectType.h"
#include "nelf/resource/Texture.h"

elfCheckBox* elfCreateCheckBox(const char* name)
{
    elfCheckBox* checkBox;

    checkBox = (elfCheckBox*)malloc(sizeof(elfCheckBox));
    memset(checkBox, 0x0, sizeof(elfCheckBox));
    checkBox->objType = ELF_CHECK_BOX;
    checkBox->objDestr = elfDestroyCheckBox;

    checkBox->color.r = checkBox->color.g = checkBox->color.b = checkBox->color.a = 1.0f;
    checkBox->visible = true;
    checkBox->active = true;

    if (name)
        checkBox->name = elfCreateString(name);

    elfIncObj(ELF_CHECK_BOX);

    return checkBox;
}

elfCheckBox* elfCreateCheckBox(elfGuiObject* parent, const char* name, int x, int y, unsigned char state)
{
    auto checkBox = elfCreateCheckBox(name);

    elfAddGuiObject(parent, (elfGuiObject*)checkBox);
    elfSetGuiObjectPosition((elfGuiObject*)checkBox, x, y);
    elfSetCheckBoxState(checkBox, state);

    return checkBox;
}

void elfDestroyCheckBox(void* data)
{
    elfCheckBox* checkBox = (elfCheckBox*)data;

    if (checkBox->name)
        elfDestroyString(checkBox->name);

    if (checkBox->off)
        elfDecRef((elfObject*)checkBox->off);
    if (checkBox->on)
        elfDecRef((elfObject*)checkBox->on);

    if (checkBox->script)
        elfDecRef((elfObject*)checkBox->script);

    free(checkBox);

    elfDecObj(ELF_CHECK_BOX);
}

void elfDrawCheckBox(elfCheckBox* checkBox, gfxShaderParams* shaderParams)
{
    if (!checkBox->visible)
        return;

    if (!checkBox->off)
    {
        elfColor col1, col2;
        float* vertexBuffer;

        shaderParams->renderParams.vertexColor = true;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
        gfxSetShaderParams(shaderParams);

        col1.r = col1.g = col1.b = 0.10f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.35f;
        col2.a = 1.0f;
        elfDrawHorGradient(checkBox->pos.x, checkBox->pos.y, checkBox->width, checkBox->height, col1, col2);

        col1.r = col1.g = col1.b = 0.25f;
        col1.a = 1.0f;
        col2.r = col2.g = col2.b = 0.35f;
        col2.a = 1.0f;
        elfDrawHorGradientBorder(checkBox->pos.x, checkBox->pos.y, checkBox->width, checkBox->height, col1, col2);

        shaderParams->renderParams.vertexColor = false;

        if (checkBox->state == ELF_ON)
        {
            shaderParams->renderParams.lineWidth = 2.0;
            gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 0.6f);
            gfxSetShaderParams(shaderParams);

            vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->lines);

            vertexBuffer[0] = checkBox->pos.x + 4;
            vertexBuffer[1] = checkBox->pos.y + 2;
            vertexBuffer[2] = 0.0;
            vertexBuffer[3] = checkBox->pos.x + 2;
            vertexBuffer[4] = checkBox->pos.y + 6;
            vertexBuffer[5] = 0.0;

            vertexBuffer[6] = checkBox->pos.x + 4;
            vertexBuffer[7] = checkBox->pos.y + 2;
            vertexBuffer[8] = 0.0;
            vertexBuffer[9] = checkBox->pos.x + 9;
            vertexBuffer[10] = checkBox->pos.y + 9;
            vertexBuffer[11] = 0.0;

            elfDrawLines(4, rnd->lines);

            shaderParams->renderParams.lineWidth = 1.0;
        }
    }
    else
    {
        shaderParams->textureParams[0].texture = checkBox->off->texture;

        if (checkBox->state == ELF_ON)
        {
            if (checkBox->on)
                shaderParams->textureParams[0].texture = checkBox->on->texture;
        }

        if (shaderParams->textureParams[0].texture)
        {
            gfxSetColor(&shaderParams->materialParams.diffuseColor, checkBox->color.r, checkBox->color.g,
                        checkBox->color.b, checkBox->color.a);
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)checkBox->pos.x, (float)checkBox->pos.y, (float)checkBox->width,
                                  (float)checkBox->height);
            shaderParams->textureParams[0].texture = nullptr;
        }
    }
}

void elfRecalcCheckBox(elfCheckBox* checkBox)
{
    if (checkBox->off)
    {
        checkBox->width = elfGetTextureWidth(checkBox->off);
        checkBox->height = elfGetTextureHeight(checkBox->off);
    }
    else
    {
        checkBox->height = 12;
        checkBox->width = 12;
    }
}

unsigned char elfGetCheckBoxState(elfCheckBox* checkBox) { return checkBox->state; }

elfTexture* elfGetCheckBoxOffTexture(elfCheckBox* checkBox) { return checkBox->off; }

elfTexture* elfGetCheckBoxOnTexture(elfCheckBox* checkBox) { return checkBox->on; }

void elfSetCheckBoxOffTexture(elfCheckBox* checkBox, elfTexture* off)
{
    if (checkBox->off)
        elfDecRef((elfObject*)checkBox->off);
    checkBox->off = off;
    if (checkBox->off)
        elfIncRef((elfObject*)checkBox->off);
    elfRecalcGuiObject((elfGuiObject*)checkBox);
}

void elfSetCheckBoxOnTexture(elfCheckBox* checkBox, elfTexture* on)
{
    if (checkBox->on)
        elfDecRef((elfObject*)checkBox->on);
    checkBox->on = on;
    if (checkBox->on)
        elfIncRef((elfObject*)checkBox->on);
}

void elfSetCheckBoxState(elfCheckBox* checkBox, unsigned char state) { checkBox->state = !state == ELF_OFF; }