#include "nelf/gfx/gfxTexture.h"

#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxGeneral.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxTextureType.h"
#include "nelf/gfx/gfxTextureWrapMode.h"

int gfxGetMaxTextureSize() { return driver->maxTextureSize; }

gfxTexture* gfxCreateTexture()
{
    gfxTexture* texture;

    texture = (gfxTexture*)malloc(sizeof(gfxTexture));
    memset(texture, 0x0, sizeof(gfxTexture));
    texture->objType = GFX_TEXTURE;
    texture->objDestr = gfxDestroyTexture;

    gfxIncObj(GFX_TEXTURE);

    return texture;
}

gfxTexture* gfxCreate2dTexture(unsigned int width, unsigned int height, float anisotropy, int mode, int filter,
                               int format, int internalFormat, int dataFormat, void* data)
{
    gfxTexture* texture;

    if (width == 0 || height == 0 || (int)width > gfxGetMaxTextureSize() || (int)height > gfxGetMaxTextureSize())
    {
        printf("error: invalid dimensions when creating texture\n");
        return NULL;
    }

    if (!(format >= GFX_LUMINANCE && format < GFX_MAX_TEXTURE_FORMATS))
    {
        printf("error: invalid format when creating texture\n");
        return NULL;
    }

    if (!(dataFormat >= GFX_FLOAT && dataFormat < GFX_MAX_FORMATS))
    {
        printf("error: invalid data format when creating texture\n");
        return NULL;
    }

    texture = gfxCreateTexture();

    texture->type = GFX_2D_MAP_TEXTURE;
    texture->width = width;
    texture->height = height;
    texture->format = format;
    texture->dataFormat = dataFormat;

    glActiveTexture(GL_TEXTURE0);
    // TODO Deprecated
    glClientActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &texture->id);

    glBindTexture(GL_TEXTURE_2D, texture->id);

    if (data && filter != GFX_NEAREST)
    {
        if (driver->version >= 140)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (filter != GFX_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (mode == GFX_REPEAT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    if (anisotropy > 1.0f && driver->version >= 200)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], data);

    glBindTexture(GL_TEXTURE_2D, 0);
    driver->shaderParams.textureParams[0].texture = NULL;

    return texture;
}

gfxTexture* gfxCreateCubeMap(unsigned int width, unsigned int height, float anisotropy, int mode, int filter,
                             int format, int internalFormat, int dataFormat, void* xpos, void* xneg, void* ypos,
                             void* yneg, void* zpos, void* zneg)
{
    gfxTexture* texture;

    if (driver->version < 130)
        return NULL;

    if (width == 0 || height == 0 || (int)width > gfxGetMaxTextureSize() || (int)height > gfxGetMaxTextureSize())
    {
        printf("error: invalid dimensions when creating texture\n");
        return NULL;
    }

    if (!(format >= GFX_LUMINANCE && format < GFX_MAX_TEXTURE_FORMATS))
    {
        printf("error: invalid format when creating texture\n");
        return NULL;
    }

    if (!(dataFormat >= GFX_FLOAT && dataFormat < GFX_MAX_FORMATS))
    {
        printf("error: invalid data format when creating texture\n");
        return NULL;
    }

    texture = gfxCreateTexture();

    texture->type = GFX_CUBE_MAP_TEXTURE;
    texture->width = width;
    texture->height = height;
    texture->format = format;
    texture->dataFormat = dataFormat;

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &texture->id);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], xpos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], xneg);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], ypos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], yneg);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], zpos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, driver->textureInternalFormats[internalFormat], width, height, 0,
                 driver->textureDataFormats[format], driver->formats[dataFormat], zneg);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    driver->shaderParams.textureParams[0].texture = NULL;

    return texture;
}

void gfxDestroyTexture(void* data)
{
    gfxTexture* texture = (gfxTexture*)data;

    if (texture->id)
        glDeleteTextures(1, &texture->id);

    free(texture);

    gfxDecObj(GFX_TEXTURE);
}

int gfxGetTextureType(gfxTexture* texture) { return texture->type; }

int gfxGetTextureWidth(gfxTexture* texture) { return texture->width; }

int gfxGetTextureHeight(gfxTexture* texture) { return texture->height; }

int gfxGetTextureFormat(gfxTexture* texture) { return texture->format; }

int gfxGetTextureDataFormat(gfxTexture* texture) { return texture->dataFormat; }

void gfxSetTexture(gfxTexture* texture, int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glClientActiveTexture(GL_TEXTURE0 + slot);

    if (driver->version < 200)
        glEnable(GL_TEXTURE_2D);

    if (texture->type == GFX_2D_MAP_TEXTURE)
        glBindTexture(GL_TEXTURE_2D, texture->id);
    else if (texture->type == GFX_CUBE_MAP_TEXTURE)
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

    driver->shaderParams.textureParams[slot].texture = texture;
}

void elfDisableTexture(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glClientActiveTexture(GL_TEXTURE0 + slot);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (driver->version < 200)
        glDisable(GL_TEXTURE_2D);

    driver->shaderParams.textureParams[slot].texture = NULL;
}
