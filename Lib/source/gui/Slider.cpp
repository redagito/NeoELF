#include "nelf/gui/Slider.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/objectType.h"
#include "nelf/resource/Texture.h"

elfSlider* elfCreateSlider(const char* name)
{
    elfSlider* slider;

    slider = (elfSlider*)malloc(sizeof(elfSlider));
    memset(slider, 0x0, sizeof(elfSlider));
    slider->objType = ELF_SLIDER;
    slider->objDestr = elfDestroySlider;

    slider->color.r = slider->color.g = slider->color.b = slider->color.a = 1.0f;
    slider->visible = true;
    slider->active = true;
    slider->value = 1.0f;

    if (name)
        slider->name = elfCreateString(name);

    elfIncObj(ELF_SLIDER);

    return slider;
}

elfSlider* elfCreateSlider(elfGuiObject* parent, const char* name, int x, int y, int width, int height, float value)
{
    auto slider = elfCreateSlider(name);

    elfAddGuiObject(parent, (elfGuiObject*)slider);
    elfSetGuiObjectPosition((elfGuiObject*)slider, x, y);
    elfSetSliderSize(slider, width, height);
    elfSetSliderValue(slider, value);

    return slider;
}

void elfDestroySlider(void* data)
{
    elfSlider* slider = (elfSlider*)data;

    if (slider->name)
        elfDestroyString(slider->name);

    if (slider->background)
        elfDecRef((elfObject*)slider->background);
    if (slider->slider)
        elfDecRef((elfObject*)slider->slider);

    if (slider->script)
        elfDecRef((elfObject*)slider->script);

    free(slider);

    elfDecObj(ELF_SLIDER);
}

void elfDrawSlider(elfSlider* slider, gfxShaderParams* shaderParams)
{
    if (!slider->visible)
        return;

    if (!slider->background)
    {
        elfColor col1, col2;

        shaderParams->renderParams.vertexColor = true;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
        gfxSetShaderParams(shaderParams);

        if (slider->width > slider->height)
        {
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.25f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x, slider->pos.y + 2, slider->width, 2, col1, col2);
            col1.r = col1.g = col1.b = 0.20f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.10f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x, slider->pos.y, slider->width, 2, col1, col2);

            col1.r = col1.g = col1.b = 0.60f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.45f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x + slider->width * slider->value - 4, slider->pos.y + 2, 8, 4, col1, col2);
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.30f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x + slider->width * slider->value - 4, slider->pos.y - 2, 8, 4, col1, col2);
        }
        else
        {
            col1.r = col1.g = col1.b = 0.20f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.40f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x, slider->pos.y + slider->height / 2, slider->width, slider->height / 2,
                               col1, col2);
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.20f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x, slider->pos.y, slider->width, slider->height / 2, col1, col2);

            col1.r = col1.g = col1.b = 0.60f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.45f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x - 2, slider->pos.y + slider->height * slider->value, 8, 4, col1, col2);
            col1.r = col1.g = col1.b = 0.40f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.30f;
            col2.a = 1.0f;
            elfDrawHorGradient(slider->pos.x - 2, slider->pos.y + slider->height * slider->value - 4, 8, 4, col1, col2);
        }
        // TODO Bug?
        shaderParams->renderParams.vertexColor = false;
    }
    else
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, slider->color.r, slider->color.g, slider->color.b,
                    slider->color.a);
        shaderParams->textureParams[0].texture = slider->background->texture;
        if (shaderParams->textureParams[0].texture)
        {
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)slider->pos.x, (float)slider->pos.y, (float)slider->width,
                                  (float)slider->height);
            shaderParams->textureParams[0].texture = NULL;
        }

        if (slider->slider)
        {
            gfxSetColor(&shaderParams->materialParams.diffuseColor, slider->color.r, slider->color.g, slider->color.b,
                        slider->color.a);
            shaderParams->textureParams[0].texture = slider->slider->texture;
            gfxSetShaderParams(shaderParams);
            if (slider->width > slider->height)
            {
                elfDrawTextured2dQuadRegion((float)slider->pos.x, (float)slider->pos.y,
                                            (float)slider->width * slider->value, (float)slider->height, 0.0f, 0.0f,
                                            slider->value, 1.0f);
            }
            else
            {
                elfDrawTextured2dQuadRegion((float)slider->pos.x, (float)slider->pos.y, (float)slider->width,
                                            (float)slider->height * slider->value, 0.0f, 0.0f, 1.0f, slider->value);
            }
            shaderParams->textureParams[0].texture = NULL;
        }
    }
}

elfTexture* elfGetSliderBackgroundTexture(elfSlider* slider) { return slider->background; }

elfTexture* elfGetSliderSliderTexture(elfSlider* slider) { return slider->slider; }

float elfGetSliderValue(elfSlider* slider) { return slider->value; }

void elfRecalcSlider(elfSlider* slider)
{
    if (slider->background)
    {
        slider->width = elfGetTextureWidth(slider->background);
        slider->height = elfGetTextureHeight(slider->background);
    }
}

void elfSetSliderSize(elfSlider* slider, int width, int height)
{
    if (!slider->background)
    {
        if (width > height)
        {
            slider->width = width;
            slider->height = 4;
        }
        else
        {
            slider->height = height;
            slider->width = 4;
        }
        if (slider->width < 4)
            slider->width = 4;
        if (slider->height < 4)
            slider->height = 4;
        elfRecalcGuiObject((elfGuiObject*)slider);
    }
}

void elfSetSliderBackgroundTexture(elfSlider* slider, elfTexture* background)
{
    if (slider->background)
        elfDecRef((elfObject*)slider->background);
    slider->background = background;
    if (slider->background)
        elfIncRef((elfObject*)slider->background);
    else
    {
        if (slider->width > slider->height)
            slider->height = 4;
        else
            slider->width = 4;
    }
    elfRecalcGuiObject((elfGuiObject*)slider);
}

void elfSetSliderSliderTexture(elfSlider* slider, elfTexture* sliderTexture)
{
    if (slider->slider)
        elfDecRef((elfObject*)slider->slider);
    slider->slider = sliderTexture;
    if (slider->slider)
        elfIncRef((elfObject*)slider->slider);
}

void elfSetSliderValue(elfSlider* slider, float value)
{
    slider->value = value;
    if (slider->value < 0.0f)
        slider->value = 0.0f;
    if (slider->value > 1.0f)
        slider->value = 1.0f;
}