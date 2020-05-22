#pragma once

#include "nelf/gui/buttonState.h"
#include "nelf/gui/guiObjectHeader.h"

struct elfFont;
struct elfTexture;
struct gfxShaderParams;

/**
 * Represents a clickable button
 */
struct elfButton
{
    ELF_GUI_OBJECT_HEADER;

    // Text on the button
    char* text = nullptr;
    // Font for the text
    elfFont* font = nullptr;
    // Text color
    elfColor textColor = colorBlack;

    // ButtonState
    ButtonState state = ELF_OFF;
    // Texture when off
    elfTexture* off = nullptr;
    // Texture when mouse over
    elfTexture* over = nullptr;
    // Texture when on
    elfTexture* on = nullptr;
};

elfButton* elfCreateButton(const char* name);
elfButton* elfCreateButton(elfGuiObject* parent, const char* name, int x, int y, int width, int height,
                           const char* text);
void elfDestroyButton(void* data);

void elfDrawButton(elfButton* button, gfxShaderParams* shaderParams);

void elfRecalcButton(elfButton* button);

// ButtonState
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