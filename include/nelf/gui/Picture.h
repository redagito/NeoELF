#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfTexture;
struct gfxShaderParams;

struct elfPicture
{
    ELF_GUI_OBJECT_HEADER;
    elfTexture* texture = nullptr;
    elfVec2f scale;
};

elfPicture* elfCreatePicture(const char* name);
void elfDestroyPicture(void* data);

void elfDrawPicture(elfPicture* picture, gfxShaderParams* shaderParams);

void elfRecalcPicture(elfPicture* picture);

elfTexture* elfGetPictureTexture(elfPicture* picture);
elfVec2f elfGetPictureScale(elfPicture* picture);

void elfSetPictureTexture(elfPicture* picture, elfTexture* texture);
void elfSetPictureScale(elfPicture* picture, float x, float y);