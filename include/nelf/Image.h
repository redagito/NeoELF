#pragma once

#include "nelf/Color.h"
#include "nelf/objectHeader.h"

struct elfImage
{
    ELF_OBJECT_HEADER;
    int width = 0;
    int height = 0;

    // TODO Store bit depth and format instead of bits per pixel
    //      Format: RGB, RGBA
    //      Bit depth: 8

    // Bits per pixel
    // Assumes
    // 8 = R
    // 16 = RG
    // 24 = RGB
    // 32 = RGBA
    unsigned char bpp = 0;

    // TODO byte type?
    unsigned char* data = nullptr;
};

elfImage* elfCreateImage();
elfImage* elfCreateEmptyImage(int width, int height, int bpp);
elfImage* elfCreateImageFromFile(const char* filePath);
void elfDestroyImage(void* data);

void elfSetImagePixel(elfImage* image, int x, int y, float r, float g, float b, float a);

int elfGetImageWidth(elfImage* image);
int elfGetImageHeight(elfImage* image);
int elfGetImageBitsPerPixel(elfImage* image);
elfColor elfGetImagePixel(elfImage* image, int x, int y);
void* elfGetImageData(elfImage* image);

bool elfSaveImageData(const char* filePath, int width, int height, unsigned char bpp, void* data);