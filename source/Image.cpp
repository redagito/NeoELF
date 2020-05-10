#include "nelf/Image.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>

// TODO Currently no conan pcakcage for freeimage library is available
//      So for now just support reading/writing png files
//      Support other file types asap
#include <lodepng.h>

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

// Uses lodepng for png loading for now
static elfImage* loadPng(const char* filePath)
{
    std::vector<unsigned char> data;
    unsigned int width = 0;
    unsigned int height = 0;

    if (lodepng::decode(data, width, height, filePath, LCT_RGBA, 8) != 0)
        return nullptr;

    elfImage* image = elfCreateImage();

    image->width = width;
    image->height = height;
    image->bpp = 32;

    int sizeBytes = image->width * image->height * (image->bpp / 8);
    assert(sizeBytes == data.size());

    image->data = (unsigned char*)malloc(sizeBytes);
    memcpy(image->data, data.data(), sizeBytes);

    return image;
}

elfImage* elfCreateImageFromFile(const char* filePath)
{
    elfImage* image;
    int sizeBytes;
    const char* type;

    type = strrchr(filePath, '.');
    if (!type)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        return NULL;
    }

    // TODO Remove later
    if ((strcmp(type, ".png") == 0))
    {
        image = loadPng(filePath);
    }
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load image \"%s\", unknown format\n", filePath);
        return NULL;
    }
    // Remove until here

    /*
    FIBITMAP* in;
    if (strcmp(type, ".jpg") == 0)
        in = FreeImage_Load(FIF_JPEG, filePath, JPEG_ACCURATE);
    else if (strcmp(type, ".png") == 0)
        in = FreeImage_Load(FIF_PNG, filePath, 0);
    else if (strcmp(type, ".tga") == 0)
        in = FreeImage_Load(FIF_TARGA, filePath, 0);
    else if (strcmp(type, ".pcx") == 0)
        in = FreeImage_Load(FIF_PCX, filePath, 0);
    else if (strcmp(type, ".bmp") == 0)
        in = FreeImage_Load(FIF_BMP, filePath, 0);
    else if (strcmp(type, ".dds") == 0)
        in = FreeImage_Load(FIF_DDS, filePath, 0);
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load image \"%s\", unknown format\n", filePath);
        return NULL;
    }

    if (!in)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        return NULL;
    }

    image = elfCreateImage();

    image->width = FreeImage_GetWidth(in);
    image->height = FreeImage_GetHeight(in);
    image->bpp = FreeImage_GetBPP(in);

    if (image->width == 0 || image->height == 0)
    {
        elfSetError(ELF_INVALID_SIZE, "error: \"%s\" has invalid size\n", filePath);
        FreeImage_Unload(in);
        elfDestroyImage(image);
        return 0;
    }

    sizeBytes = image->width * image->height * (image->bpp / 8);

    image->data = (unsigned char*)malloc(sizeBytes);

    FreeImage_ConvertToRawBits((BYTE*)image->data, in, image->width * (image->bpp / 8), image->bpp, FI_RGBA_RED_MASK,
                               FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);

    FreeImage_Unload(in);

    */

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

    // Temporary solution for png support remove later
    const char* type = strrchr(filePath, '.');
    if (strcmp(type, ".png") == 0)
        return lodepng::encode(filePath, (unsigned char*)data, width, height, LCT_RGBA, 8) == 0;
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't save image \"%s\", unknown format\n", filePath);
        return false;
    }
    // Remove until here

    /*
    FIBITMAP* out = FreeImage_ConvertFromRawBits((BYTE*)data, width, height, width * (bpp / 8), bpp, FI_RGBA_RED_MASK,
                                       FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);

    // the jpg freeimage plugin can't save a 32 bit map, and who knows what other
    // plugins can't, so for now, this should fix it...
    if (bpp != 24)
    {
        FIBITMAP* temp = FreeImage_ConvertTo24Bits(out);
        FreeImage_Unload(out);
        out = temp;
    }

    if (!out)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't save image \"%s\"\n", filePath);
        return false;
    }

    const char* type = strrchr(filePath, '.');
    if (strcmp(type, ".bmp") == 0)
        FreeImage_Save(FIF_BMP, out, filePath, 0);
    else if (strcmp(type, ".tga") == 0)
        FreeImage_Save(FIF_TARGA, out, filePath, 0);
    else if (strcmp(type, ".jpg") == 0)
        FreeImage_Save(FIF_JPEG, out, filePath, JPEG_QUALITYSUPERB);
    else if (strcmp(type, ".pcx") == 0)
        FreeImage_Save(FIF_PCX, out, filePath, 0);
    else if (strcmp(type, ".png") == 0)
        FreeImage_Save(FIF_PNG, out, filePath, 0);
    else if (strcmp(type, ".dds") == 0)
        FreeImage_Save(FIF_DDS, out, filePath, 0);
    else
    {
        FreeImage_Unload(out);
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't save image \"%s\", unknown format\n", filePath);
        return false;
    }

    FreeImage_Unload(out);
    */

    return true;
}