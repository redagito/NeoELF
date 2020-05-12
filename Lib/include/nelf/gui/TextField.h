#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfTexture;
struct elfFont;
struct elfArea;
struct gfxShaderParams;

struct elfTextField
{
    ELF_GUI_OBJECT_HEADER;
    elfTexture* texture = nullptr;
    elfFont* font = nullptr;

    // Default offset
    int offsetX = 2;
    int offsetY = 2;

    elfColor textColor = colorBlack;
    int cursorPos = 0;
    int drawPos = 0;
    int drawOffset = 0;
    char* text = nullptr;
};

elfTextField* elfCreateTextField(const char* name);
elfTextField* elfCreateTextField(elfGuiObject* parent, const char* name, int x, int y, int width, const char* text);

void elfDestroyTextField(void* data);

void elfDrawTextField(elfTextField* textField, elfArea* area, gfxShaderParams* shaderParams);

void elfRecalcTextField(elfTextField* textField);

elfTexture* elfGetTextFieldTexture(elfTextField* textField);
elfFont* elfGetTextFieldFont(elfTextField* textField);
elfColor elfGetTextFieldTextColor(elfTextField* textField);
elfVec2i elfGetTextFieldOffset(elfTextField* textField);
const char* elfGetTextFieldText(elfTextField* textField);

void elfSetTextFieldFont(elfTextField* textField, elfFont* font);
void elfSetTextFieldWidth(elfTextField* textField, int width);
void elfSetTextFieldTexture(elfTextField* textField, elfTexture* texture);
void elfSetTextFieldTextColor(elfTextField* textField, float r, float g, float b, float a);
void elfSetTextFieldOffset(elfTextField* textField, int offsetX, int offsetY);

void elfMoveTextFieldCursorLeft(elfTextField* textField);
void elfMoveTextFieldCursorRight(elfTextField* textField);

void elfSetTextFieldCursorPosition(elfTextField* textField, int idx);
void elfSetTextFieldText(elfTextField* textField, const char* text);