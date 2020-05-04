#include "nelf/Font.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/String.h"
#include "nelf/objectType.h"

elfFont* elfCreateFont()
{
    elfFont* font;

    font = (elfFont*)malloc(sizeof(elfFont));
    memset(font, 0x0, sizeof(elfFont));
    font->objType = ELF_FONT;
    font->objDestr = elfDestroyFont;

    elfIncObj(ELF_FONT);

    return font;
}

elfFont* elfCreateFontFromFile(const char* filePath, int size)
{
    FT_Library library;
    FT_Face face;
    FT_GlyphSlot slot;
    elfFont* font;
    int width;
    int height;
    unsigned char* data;
    int error;
    int i, j, k;

    if (size < 1)
    {
        elfSetError(ELF_INVALID_SIZE, "error: can not load a font size smaller than 1\n");
        return NULL;
    }

    error = FT_Init_FreeType(&library);
    if (error)
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: could not initialize freetype, could not open \"%s\"\n", filePath);
        return NULL;
    }

    error = FT_New_Face(library, filePath, 0, &face);
    if (error)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        error = FT_Done_FreeType(library);
        if (error)
        {
            elfSetError(ELF_CANT_OPEN_FILE, "error: can't deinitialize freetype\n");
            return NULL;
        }
        return NULL;
    }

    error = FT_Set_Pixel_Sizes(face, 0, size);
    if (error)
    {
        elfSetError(ELF_INVALID_SIZE, "error: can't set the size of font \"%s\"\n", filePath);

        error = FT_Done_Face(face);
        error = FT_Done_FreeType(library);

        return NULL;
    }

    font = elfCreateFont();

    font->filePath = elfCreateString(filePath);
    font->size = size;

    slot = face->glyph;

    for (i = 33; i < 127; i++)
    {
        error = FT_Load_Char(face, (char)i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT);
        if (error)
        {
            elfLogWrite("warning: could not load character \"%c\" in font \"%s\"\n", (char)i, filePath);
            continue;
        }

        width = slot->bitmap.width;
        height = slot->bitmap.rows;
        if (width < 1 || height < 1)
            continue;

        data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 2);
        for (j = 0; j < height; j++)
        {
            for (k = 0; k < width; k++)
            {
                data[j * width * 2 + k * 2] = 255;
                data[j * width * 2 + k * 2 + 1] = slot->bitmap.buffer[((height - 1) - j) * width + k];
            }
        }

        font->chars[i].code = (char)i;
        font->chars[i].offsetX = width + size / 7;
        font->chars[i].offsetY = -(face->glyph->bitmap.rows - face->glyph->bitmap_top);
        font->chars[i].texture = gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_LUMINANCE_ALPHA,
                                                    GFX_LUMINANCE_ALPHA, GFX_UBYTE, data);

        if (-font->chars[i].offsetY > font->offsetY)
            font->offsetY = -font->chars[i].offsetY;

        free(data);
        data = NULL;
    }

    error = FT_Done_Face(face);
    error = FT_Done_FreeType(library);

    return font;
}

elfFont* elfGetDefaultFont() { return eng->guiFont; }

void elfDestroyFont(void* data)
{
    int i;
    elfFont* font = (elfFont*)data;

    if (font->name)
        elfDestroyString(font->name);
    if (font->filePath)
        elfDestroyString(font->filePath);

    for (i = 0; i < 128; i++)
        if (font->chars[i].texture)
            gfxDestroyTexture(font->chars[i].texture);

    free(font);

    elfDecObj(ELF_FONT);
}

const char* elfGetFontName(elfFont* font) { return font->name; }

const char* elfGetFontFilePath(elfFont* font) { return font->filePath; }

int elfGetFontSize(elfFont* font) { return font->size; }

int elfGetStringWidth(elfFont* font, const char* str)
{
    int x = 0;
    int ox = 0;
    int y = font->size;
    elfCharacter* chr = 0;
    unsigned int i;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] < 0)
            continue;
        chr = &font->chars[(unsigned int)str[i]];
        if (str[i] == ' ')
            ox += font->size / 3;
        else if (str[i] == '\t')
            ox += font->size / 3 * 5;
        else if (str[i] == '\n')
        {
            y += font->size + 1;
            ox = 0;
        }
        else
        {
            if (i != strlen(str) - 1 || !chr->texture)
                ox += chr->offsetX;
            else
                ox += gfxGetTextureWidth(chr->texture);
        }
        if (ox > x)
            x = ox;
    }

    return x;
}

int elfGetStringHeight(elfFont* font, const char* str)
{
    int ox = 0;
    int y = font->size;
    elfCharacter* chr = 0;
    unsigned int i;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] < 0)
            continue;
        chr = &font->chars[(unsigned int)str[i]];
        if (str[i] == ' ')
            ox += font->size / 3;
        else if (str[i] == '\t')
            ox += font->size / 3 * 5;
        else if (str[i] == '\n')
        {
            y += font->size + 1;
            ox = 0;
        }
        else
        {
            if (i != strlen(str) - 1 || !chr->texture)
                ox += chr->offsetX;
            else
                ox += gfxGetTextureWidth(chr->texture);
        }
    }

    return y;
}

void elfDrawString(elfFont* font, const char* str, int x, int y, gfxShaderParams* shaderParams)
{
    int ox;
    int oy;
    elfCharacter* chr;
    unsigned int i;

    ox = x;
    oy = y;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] < 0)
            continue;
        chr = &font->chars[(unsigned int)str[i]];
        if (str[i] == ' ')
            ox += font->size / 3;
        else if (str[i] == '\t')
            ox += font->size / 3 * 5;
        else if (str[i] == '\n')
        {
            oy -= font->size + 1;
            ox = x;
        }
        else
        {
            if (!chr->texture)
                continue;
            shaderParams->textureParams[0].texture = chr->texture;
            shaderParams->textureParams[0].type = GFX_COLOR_MAP;
            gfxSetShaderParams(shaderParams);
            elfDrawTextured2dQuad((float)ox, (float)(oy + chr->offsetY + font->offsetY),
                                  (float)gfxGetTextureWidth(chr->texture), (float)gfxGetTextureHeight(chr->texture));
            ox += chr->offsetX;
        }
    }
}
