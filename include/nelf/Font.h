#pragma once

#include "nelf/objectHeader.h"

struct gfxTexture;
struct gfxShaderParams;

// Single font character
struct elfCharacter
{
    char code = 0;
    gfxTexture* texture = nullptr;
    int offsetX = 0;
    int offsetY = 0;
};

// Font
struct elfFont
{
    ELF_OBJECT_HEADER;
    char* name = nullptr;
    // TODO Resource type with hot reload?
    char* filePath = nullptr;
    int size = 0;
    elfCharacter chars[128];
    int offsetY = 0;
};

elfFont* elfCreateFont();

elfFont* elfCreateFontFromFile(const char* filePath, int size);

elfFont* elfGetDefaultFont();

void elfDestroyFont(void* data);

const char* elfGetFontName(elfFont* font);

const char* elfGetFontFilePath(elfFont* font);

int elfGetFontSize(elfFont* font);

int elfGetStringWidth(elfFont* font, const char* str);

int elfGetStringHeight(elfFont* font, const char* str);

// TODO Should this be here?
void elfDrawString(elfFont* font, const char* str, int x, int y, gfxShaderParams* shaderParams);