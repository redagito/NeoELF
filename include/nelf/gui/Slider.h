#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfTexture;
struct gfxShaderParams;

struct elfSlider
{
    ELF_GUI_OBJECT_HEADER;
    elfTexture* background = nullptr;
    elfTexture* slider = nullptr;
    float value = 0.f;
};

elfSlider* elfCreateSlider(const char* name);
void elfDestroySlider(void* data);

void elfDrawSlider(elfSlider* slider, gfxShaderParams* shaderParams);

elfTexture* elfGetSliderBackgroundTexture(elfSlider* slider);
elfTexture* elfGetSliderSliderTexture(elfSlider* slider);
float elfGetSliderValue(elfSlider* slider);

void elfRecalcSlider(elfSlider* slider);

void elfSetSliderSize(elfSlider* slider, int width, int height);
void elfSetSliderBackgroundTexture(elfSlider* slider, elfTexture* background);
void elfSetSliderSliderTexture(elfSlider* slider, elfTexture* sliderTexture);
void elfSetSliderValue(elfSlider* slider, float value);