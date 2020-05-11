#include "nelf/Image.h"

#include <SOIL2/SOIL2.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

elfImage* elfCreateImage()
{
    elfImage* image;

    image = (elfImage*)malloc(sizeof(elfImage));
    memset(image, 0x0, sizeof(elfImage));
    image->objType = ELF_IMAGE;
    image->objDestr = elfDestroyImage;

    elfIncObj(ELF_IMAGE);

    return image;
}

elfImage* elfCreateEmptyImage(int width, int height, int bpp)
{
    elfImage* image;

    if (width < 0 || height < 0 || (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32))
        return NULL;

    image = elfCreateImage();

    image->width = width;
    image->height = height;
    image->bpp = bpp;

    image->data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * (bpp / 8));
    memset(image->data, 0x0, sizeof(unsigned char) * width * height * (bpp / 8));

    return image;
}

elfImage* elfCreateImageFromFile(const char* filePath)
{
    // TODO For OpenGL textures, we need to flip the image
    //      Might need to fix this here or in the texture class
    //      if its not already done somewhere.

    // Load with soil2
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = SOIL_load_image(filePath, &width, &height, &channels, SOIL_LOAD_AUTO);
    if (data == nullptr)
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load image \"%s\"", filePath);
        return nullptr;
    }

    elfImage* image = elfCreateImage();
    image->width = width;
    image->height = height;
    // 1 channel = 8 bits
    image->bpp = channels * 8;

    if (image->width == 0 || image->height == 0)
    {
        elfSetError(ELF_INVALID_SIZE, "error: \"%s\" has invalid size\n", filePath);
        SOIL_free_image_data(data);
        elfDestroyImage(image);
        return 0;
    }

    // Copy image data
    int sizeBytes = image->width * image->height * (image->bpp / 8);
    image->data = (unsigned char*)malloc(sizeBytes);
    memcpy(image->data, data, sizeBytes);
    SOIL_free_image_data(data);

    return image;
}

void elfDestroyImage(void* data)
{
    elfImage* image = (elfImage*)data;

    if (image->data)
        free(image->data);

    free(image);

    elfDecObj(ELF_IMAGE);
}

void elfSetImagePixel(elfImage* image, int x, int y, float r, float g, float b, float a)
{
    int offset;

    if (x < 0 || x >= image->width || y < 0 || y >= image->height)
        return;

    offset = image->width * y + x;

    image->data[offset] = (unsigned char)(r * 255);
    if (image->bpp == 16)
        image->data[offset + 1] = (unsigned char)(g * 255);
    if (image->bpp == 24)
        image->data[offset + 2] = (unsigned char)(b * 255);
    if (image->bpp == 32)
        image->data[offset + 3] = (unsigned char)(a * 255);
}

int elfGetImageWidth(elfImage* image) { return image->width; }

int elfGetImageHeight(elfImage* image) { return image->height; }

int elfGetImageBitsPerPixel(elfImage* image) { return image->bpp; }

elfColor elfGetImagePixel(elfImage* image, int x, int y)
{
    int offset;
    elfColor color;

    memset(&color, 0x0, sizeof(elfColor));

    if (x < 0 || x >= image->width || y < 0 || y >= image->height)
        return color;

    offset = image->width * y + x;

    color.r = (float)image->data[offset] / (float)255;
    if (image->bpp == 16)
        color.g = (float)image->data[offset + 1] / (float)255;
    if (image->bpp == 24)
        color.b = (float)image->data[offset + 2] / (float)255;
    if (image->bpp == 32)
        color.a = (float)image->data[offset + 3] / (float)255;

    return color;
}

void* elfGetImageData(elfImage* image) { return image->data; }

bool elfSaveImageData(const char* filePath, int width, int height, unsigned char bpp, void* data)
{
    if (width < 1 || height < 1 || bpp % 8 != 0 || bpp > 32 || data == nullptr)
        return false;

    // Default
    int imageType = SOIL_SAVE_TYPE_JPG;

    // Supported types
    // TODO PCX no longer supported
    const char* type = strrchr(filePath, '.');
    if (strcmp(type, ".bmp") == 0)
        imageType = SOIL_SAVE_TYPE_BMP;
    else if (strcmp(type, ".tga") == 0)
        imageType = SOIL_SAVE_TYPE_TGA;
    else if (strcmp(type, ".jpg") == 0)
        imageType = SOIL_SAVE_TYPE_JPG;
    else if (strcmp(type, ".png") == 0)
        imageType = SOIL_SAVE_TYPE_PNG;
    else if (strcmp(type, ".dds") == 0)
        imageType = SOIL_SAVE_TYPE_DDS;
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't save image \"%s\", unknown format\n", filePath);
        return false;
    }

    // Save to disk
    if (SOIL_save_image(filePath, imageType, width, height, bpp / 8, (const unsigned char*)data) == 0)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't save image \"%s\"\n", filePath);
        return false;
    }
    return true;
}