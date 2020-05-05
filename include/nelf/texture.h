#pragma once

#include "nelf/resourceHeader.h"

struct elfImage;
struct gfxTexture;
struct gfxShaderParams;

struct elfTexture
{
    ELF_RESOURCE_HEADER;
    char* filePath = nullptr;
    gfxTexture* texture = nullptr;

    void* data = nullptr;
    int dataSize = 0;
};

elfTexture* elfCreateTexture();
elfTexture* elfCreateTextureFromFile(const char* name, const char* filePath);
elfTexture* elfCreateTextureFromImage(const char* name, elfImage* image);
elfTexture* elfCreateCubeMapFromFiles(const char* name, const char* xpos, const char* xneg, const char* ypos,
                                      const char* yneg, const char* zpos, const char* zneg);

void elfDestroyTexture(void* data);

void elfSetTextureName(elfTexture* texture, const char* name);
const char* elfGetTextureName(elfTexture* texture);

const char* elfGetTextureFilePath(elfTexture* texture);

int elfGetTextureWidth(elfTexture* texture);
int elfGetTextureHeight(elfTexture* texture);
int elfGetTextureFormat(elfTexture* texture);
int elfGetTextureDataFormat(elfTexture* texture);

gfxTexture* elfGetGfxTexture(elfTexture* texture);

void elfSetTexture(int slot, elfTexture* texture, gfxShaderParams* shaderParams);

bool elfLoadTextureData(elfTexture* texture);
void elfUnloadTextureData(elfTexture* texture);