#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfFont;
struct gfxShaderParams;

struct elfLabel
{
    ELF_GUI_OBJECT_HEADER;
    elfFont* font = nullptr;
    char* text = nullptr;
};

elfLabel* elfCreateLabel(const char* name);

void elfDestroyLabel(void* data);

void elfDrawLabel(elfLabel* label, gfxShaderParams* shaderParams);

void elfRecalcLabel(elfLabel* label);

elfFont* elfGetLabelFont(elfLabel* label);

const char* elfGetLabelText(elfLabel* label);

void elfSetLabelFont(elfLabel* label, elfFont* font);

void elfSetLabelText(elfLabel* label, const char* text);