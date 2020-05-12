#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfTexture;
struct gfxShaderParams;

struct elfCheckBox
{
    ELF_GUI_OBJECT_HEADER;
    unsigned char state;
    elfTexture* off = nullptr;
    elfTexture* on = nullptr;
};

elfCheckBox* elfCreateCheckBox(const char* name);
void elfDestroyCheckBox(void* data);

void elfDrawCheckBox(elfCheckBox* checkBox, gfxShaderParams* shaderParams);

void elfRecalcCheckBox(elfCheckBox* checkBox);

unsigned char elfGetCheckBoxState(elfCheckBox* checkBox);
elfTexture* elfGetCheckBoxOffTexture(elfCheckBox* checkBox);
elfTexture* elfGetCheckBoxOnTexture(elfCheckBox* checkBox);

void elfSetCheckBoxOffTexture(elfCheckBox* checkBox, elfTexture* off);
void elfSetCheckBoxOnTexture(elfCheckBox* checkBox, elfTexture* on);
void elfSetCheckBoxState(elfCheckBox* checkBox, unsigned char state);
