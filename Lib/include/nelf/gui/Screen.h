#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfTexture;
struct elfArea;
struct gfxShaderParams;

struct elfScreen
{
    ELF_GUI_OBJECT_HEADER;
    elfTexture* texture = nullptr;
    // TODO ?????
    int hack = 0;
};

elfScreen* elfCreateScreen(const char* name);
elfScreen* elfCreateScreen(elfGuiObject* parent, const char* name, int x, int y, int width, int height);
void elfDestroyScreen(void* data);

void elfDrawScreen(elfScreen* screen, elfArea* area, gfxShaderParams* shaderParams);

elfTexture* elfGetScreenTexture(elfScreen* screen);

void elfRecalcScreen(elfScreen* screen);

void elfSetScreenSize(elfScreen* screen, int width, int height);
void elfSetScreenTexture(elfScreen* screen, elfTexture* texture);
void elfSetScreenToTop(elfScreen* screen);

void elfForceScreenFocus(elfScreen* screen);
void elfReleaseScreenFocus(elfScreen* screen);