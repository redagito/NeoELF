#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfFont;
struct elfTexture;
struct gfxShaderParams;

struct elfButton
{
    ELF_GUI_OBJECT_HEADER;
    char* text = nullptr;
    elfFont* font = nullptr;
    // TODO Override setting with active flag?
    //      Basically always off if inactive
    unsigned char state = 0;
    elfTexture* off = nullptr;
    elfTexture* over = nullptr;
    elfTexture* on = nullptr;
    elfColor textColor = colorBlack;
};

elfButton* elfCreateButton(const char* name);

void elfDestroyButton(void* data);

void elfDrawButton(elfButton* button, gfxShaderParams* shaderParams);

void elfRecalcButton(elfButton* button);

unsigned char elfGetButtonState(elfButton* button);

const char* elfGetButtonText(elfButton* button);

elfFont* elfGetButtonFont(elfButton* button);

elfVec2i elfGetButtonSize(elfButton* button);

elfTexture* elfGetButtonOffTexture(elfButton* button);

elfTexture* elfGetButtonOverTexture(elfButton* button);

elfTexture* elfGetButtonOnTexture(elfButton* button);

elfColor elfGetButtonTextColor(elfButton* button);

void elfSetButtonTextColor(elfButton* button, float r, float g, float b, float a);

void elfSetButtonText(elfButton* button, const char* text);

void elfSetButtonFont(elfButton* button, elfFont* font);

void elfSetButtonSize(elfButton* button, int width, int height);

void elfSetButtonOffTexture(elfButton* button, elfTexture* off);

void elfSetButtonOverTexture(elfButton* button, elfTexture* over);

void elfSetButtonOnTexture(elfButton* button, elfTexture* on);