#include "nelf/Image.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(filePath, &width, &height, &channels, 3);
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
        stbi_image_free(data);
        elfDestroyImage(image);
        return 0;
    }

    // Copy image data
    int sizeBytes = image->width * image->height * (image->bpp / 8);
    image->data = (unsigned char*)malloc(sizeBytes);
    memcpy(image->data, data, sizeBytes);
    stbi_image_free(data);

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

    // Supported types to write
    int result = 0;
    const char* type = strrchr(filePath, '.');
    if (strcmp(type, ".bmp") == 0)
        result = stbi_write_bmp(filePath, width, height, bpp / 8, data);
    else if (strcmp(type, ".tga") == 0)
        result = stbi_write_tga(filePath, width, height, bpp / 8, data);
    else if (strcmp(type, ".jpg") == 0)
        result = stbi_write_jpg(filePath, width, height, bpp / 8, data, 85);
    else if (strcmp(type, ".png") == 0)
        result = stbi_write_png(filePath, width, height, bpp / 8, data, width * (bpp % 8));
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't save image \"%s\", unknown format\n", filePath);
        return false;
    }

    // Check result
    if (result != 1)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't save image \"%s\"\n", filePath);
        return false;
    }
    return true;
}