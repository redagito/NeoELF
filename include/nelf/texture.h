
elfTexture* elfCreateTexture()
{
    elfTexture* texture;

    texture = (elfTexture*)malloc(sizeof(elfTexture));
    memset(texture, 0x0, sizeof(elfTexture));
    texture->objType = ELF_TEXTURE;
    texture->objDestr = elfDestroyTexture;

    texture->id = ++res->textureIdCounter;

    elfIncObj(ELF_TEXTURE);

    return texture;
}

elfTexture* elfCreateTextureFromFile(const char* name, const char* filePath)
{
    elfImage* image;
    elfTexture* texture;
    int format;
    int internalFormat;

    image = elfCreateImageFromFile(filePath);
    if (!image)
        return NULL;

    switch (elfGetImageBitsPerPixel(image))
    {
    case 8:
        format = GFX_LUMINANCE;
        internalFormat = GFX_LUMINANCE;
        break;
    case 16:
        format = GFX_LUMINANCE_ALPHA;
        internalFormat = GFX_LUMINANCE_ALPHA;
        break;
    case 24:
        format = GFX_BGR;
        if (gfxGetVersion() >= 130)
            internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGB : GFX_RGB;
        else
            internalFormat = GFX_RGB;
        break;
    case 32:
        format = GFX_BGRA;
        if (gfxGetVersion() >= 130)
            internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGBA : GFX_RGBA;
        else
            internalFormat = GFX_RGBA;
        break;
    default:
        elfSetError(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] in image \"%s\"\n",
                    elfGetImageBitsPerPixel(image), filePath);
        elfDestroyImage(image);
        return NULL;
    }

    texture = elfCreateTexture();

    texture->name = elfCreateString(name);
    texture->filePath = elfCreateString(filePath);

    texture->texture =
        gfxCreate2dTexture(elfGetImageWidth(image), elfGetImageHeight(image), eng->config->textureAnisotropy,
                           GFX_REPEAT, GFX_LINEAR, format, internalFormat, GFX_UBYTE, elfGetImageData(image));

    if (!texture->texture)
    {
        elfSetError(ELF_CANT_CREATE, "error: failed to create texture \"%s\"\n", filePath);
        elfDestroyTexture(texture);
        elfDestroyImage(image);
        return NULL;
    }

    elfDestroyImage(image);

    return texture;
}

elfTexture* elfCreateTextureFromImage(const char* name, elfImage* image)
{
    elfTexture* texture;
    int format;
    int internalFormat;

    switch (elfGetImageBitsPerPixel(image))
    {
    case 8:
        format = GFX_LUMINANCE;
        internalFormat = GFX_LUMINANCE;
        break;
    case 16:
        format = GFX_LUMINANCE_ALPHA;
        internalFormat = GFX_LUMINANCE_ALPHA;
        break;
    case 24:
        format = GFX_BGR;
        if (gfxGetVersion() >= 130)
            internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGB : GFX_RGB;
        else
            internalFormat = GFX_RGB;
        break;
    case 32:
        format = GFX_BGRA;
        if (gfxGetVersion() >= 130)
            internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGBA : GFX_RGBA;
        else
            internalFormat = GFX_RGBA;
        break;
    default:
        elfSetError(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] for image\n",
                    elfGetImageBitsPerPixel(image));
        return NULL;
    }

    texture = elfCreateTexture();

    texture->name = elfCreateString(name);
    texture->texture =
        gfxCreate2dTexture(elfGetImageWidth(image), elfGetImageHeight(image), eng->config->textureAnisotropy,
                           GFX_REPEAT, GFX_LINEAR, format, internalFormat, GFX_UBYTE, elfGetImageData(image));

    if (!texture->texture)
    {
        elfSetError(ELF_CANT_CREATE, "error: failed to create texture\n");
        elfDestroyTexture(texture);
        return NULL;
    }

    return texture;
}

elfTexture* elfCreateCubeMapFromFiles(const char* name, const char* xpos, const char* xneg, const char* ypos,
                                      const char* yneg, const char* zpos, const char* zneg)
{
    elfImage* xposi;
    elfImage* xnegi;
    elfImage* yposi;
    elfImage* ynegi;
    elfImage* zposi;
    elfImage* znegi;

    elfTexture* texture;
    int format;
    int internalFormat;

    xposi = elfCreateImageFromFile(xpos);
    xnegi = elfCreateImageFromFile(xneg);
    yposi = elfCreateImageFromFile(ypos);
    ynegi = elfCreateImageFromFile(yneg);
    zposi = elfCreateImageFromFile(zpos);
    znegi = elfCreateImageFromFile(zneg);

    if (!xposi || !xnegi || !yposi || !ynegi || !zposi || !znegi)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open all of the cube map images\n");
        if (xposi)
            elfDestroyImage(xposi);
        if (xnegi)
            elfDestroyImage(xnegi);
        if (yposi)
            elfDestroyImage(yposi);
        if (ynegi)
            elfDestroyImage(ynegi);
        if (zposi)
            elfDestroyImage(zposi);
        if (znegi)
            elfDestroyImage(znegi);
        return NULL;
    }

    if (elfGetImageBitsPerPixel(xposi) != elfGetImageBitsPerPixel(xnegi) ||
        elfGetImageBitsPerPixel(xposi) != elfGetImageBitsPerPixel(yposi) ||
        elfGetImageBitsPerPixel(xposi) != elfGetImageBitsPerPixel(ynegi) ||
        elfGetImageBitsPerPixel(xposi) != elfGetImageBitsPerPixel(zposi) ||
        elfGetImageBitsPerPixel(xposi) != elfGetImageBitsPerPixel(znegi))
    {
        elfSetError(ELF_INVALID_FILE, "error: cube map images have different bits per pixel count\n");
        elfDestroyImage(xposi);
        elfDestroyImage(xnegi);
        elfDestroyImage(yposi);
        elfDestroyImage(ynegi);
        elfDestroyImage(zposi);
        elfDestroyImage(znegi);
        return NULL;
    }

    if (elfGetImageWidth(xposi) != elfGetImageWidth(xnegi) || elfGetImageWidth(xposi) != elfGetImageWidth(yposi) ||
        elfGetImageWidth(xposi) != elfGetImageWidth(ynegi) || elfGetImageWidth(xposi) != elfGetImageWidth(zposi) ||
        elfGetImageWidth(xposi) != elfGetImageWidth(znegi))
    {
        elfSetError(ELF_INVALID_FILE, "error: cube map images have different dimensions, \"%s\"\n", xpos);
        elfDestroyImage(xposi);
        elfDestroyImage(xnegi);
        elfDestroyImage(yposi);
        elfDestroyImage(ynegi);
        elfDestroyImage(zposi);
        elfDestroyImage(znegi);
        return NULL;
    }

    if (elfGetImageHeight(xposi) != elfGetImageHeight(xnegi) || elfGetImageHeight(xposi) != elfGetImageHeight(yposi) ||
        elfGetImageHeight(xposi) != elfGetImageHeight(ynegi) || elfGetImageHeight(xposi) != elfGetImageHeight(zposi) ||
        elfGetImageHeight(xposi) != elfGetImageHeight(znegi))
    {
        elfSetError(ELF_INVALID_FILE, "error: cube map images have different dimensions, \"%s\"\n", xpos);
        elfDestroyImage(xposi);
        elfDestroyImage(xnegi);
        elfDestroyImage(yposi);
        elfDestroyImage(ynegi);
        elfDestroyImage(zposi);
        elfDestroyImage(znegi);
        return NULL;
    }

    switch (elfGetImageBitsPerPixel(xposi))
    {
    case 8:
        format = GFX_LUMINANCE;
        internalFormat = GFX_LUMINANCE;
        break;
    case 16:
        format = GFX_LUMINANCE_ALPHA;
        internalFormat = GFX_LUMINANCE_ALPHA;
        break;
    case 24:
        format = GFX_BGR;
        internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGB : GFX_RGB;
        break;
    case 32:
        format = GFX_BGRA;
        internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGBA : GFX_RGBA;
        break;
    default:
        elfSetError(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] in cube map image \"%s\"\n",
                    elfGetImageBitsPerPixel(xposi), xpos);
        elfDestroyImage(xposi);
        elfDestroyImage(xnegi);
        elfDestroyImage(yposi);
        elfDestroyImage(ynegi);
        elfDestroyImage(zposi);
        elfDestroyImage(znegi);
        return NULL;
    }

    texture = elfCreateTexture();

    texture->name = elfCreateString(name);
    texture->filePath = elfCreateString(xpos);

    // z negative first and then z positive... lol didn't bother to figure out why it works like this
    texture->texture = gfxCreateCubeMap(
        elfGetImageWidth(xposi), elfGetImageHeight(xposi), eng->config->textureAnisotropy, GFX_REPEAT, GFX_LINEAR,
        format, internalFormat, GFX_UBYTE, elfGetImageData(xposi), elfGetImageData(xnegi), elfGetImageData(yposi),
        elfGetImageData(ynegi), elfGetImageData(znegi), elfGetImageData(zposi));

    if (!texture->texture)
    {
        elfSetError(ELF_CANT_CREATE, "error: failed to create cube map \"%s\"\n", xposi);
        elfDestroyTexture(texture);
        elfDestroyImage(xposi);
        elfDestroyImage(xnegi);
        elfDestroyImage(yposi);
        elfDestroyImage(ynegi);
        elfDestroyImage(zposi);
        elfDestroyImage(znegi);
        return NULL;
    }

    elfDestroyImage(xposi);
    elfDestroyImage(xnegi);
    elfDestroyImage(yposi);
    elfDestroyImage(ynegi);
    elfDestroyImage(zposi);
    elfDestroyImage(znegi);

    return texture;
}

void elfDestroyTexture(void* data)
{
    elfTexture* texture = (elfTexture*)data;

    if (texture->name)
        elfDestroyString(texture->name);
    if (texture->filePath)
        elfDestroyString(texture->filePath);

    if (texture->texture)
        gfxDestroyTexture(texture->texture);
    if (texture->data)
        free(texture->data);

    elfDecObj(ELF_TEXTURE);

    free(texture);
}

void elfSetTextureName(elfTexture* texture, const char* name)
{
    if (texture->name)
        elfDestroyString(texture->name);
    texture->name = elfCreateString(name);
}

const char* elfGetTextureName(elfTexture* texture) { return texture->name; }

const char* elfGetTextureFilePath(elfTexture* texture) { return texture->filePath; }

int elfGetTextureWidth(elfTexture* texture) { return gfxGetTextureWidth(texture->texture); }

int elfGetTextureHeight(elfTexture* texture) { return gfxGetTextureHeight(texture->texture); }

int elfGetTextureFormat(elfTexture* texture) { return gfxGetTextureFormat(texture->texture); }

int elfGetTextureDataFormat(elfTexture* texture) { return gfxGetTextureDataFormat(texture->texture); }

gfxTexture* elfGetGfxTexture(elfTexture* texture) { return texture->texture; }

void elfSetTexture(int slot, elfTexture* texture, gfxShaderParams* shaderParams)
{
    if (!texture->texture || slot < 0 || slot > GFX_MAX_TEXTURES - 1)
        return;

    shaderParams->textureParams[slot].texture = texture->texture;
}

unsigned char elfLoadTextureData(elfTexture* texture)
{
    const char* fileType;
    FILE* file;
    elfPak* pak;
    elfPakIndex* index;
    int magic;
    char name[ELF_NAME_LENGTH];
    unsigned char type;

    if (texture->data)
        return ELF_TRUE;

    fileType = strrchr(texture->filePath, '.');

    if (!strcmp(fileType, ".pak"))
    {
        pak = elfCreatePakFromFile(texture->filePath);
        if (!pak)
            return ELF_FALSE;

        index = elfGetPakIndexByName(pak, texture->name, ELF_TEXTURE);
        if (!index)
        {
            elfSetError(ELF_INVALID_FILE, "error: couldn't fine index for \"%s//%s\"\n", texture->filePath,
                        texture->name);
            elfDestroyPak(pak);
            return ELF_FALSE;
        }

        file = fopen(pak->filePath, "rb");
        fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
        if (feof(file))
            return ELF_FALSE;

        fread((char*)&magic, sizeof(int), 1, file);

        if (magic != 179532108)
        {
            elfSetError(ELF_INVALID_FILE, "error: invalid texture \"%s//%s\", wrong magic number\n", texture->filePath,
                        texture->name);
            elfDestroyPak(pak);
            fclose(file);
            return ELF_FALSE;
        }

        fread(name, sizeof(char), ELF_NAME_LENGTH, file);
        fread((char*)&type, sizeof(unsigned char), 1, file);

        if (type == 1)
        {
            fread((char*)&texture->dataSize, sizeof(int), 1, file);

            texture->data = (char*)malloc(texture->dataSize);
            fread((char*)texture->data, 1, texture->dataSize, file);
        }
        else
        {
            elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load texture \"%s//%s\", unknown format\n", texture->filePath,
                        texture->name);
            elfDestroyPak(pak);
            fclose(file);
            return ELF_FALSE;
        }

        elfDestroyPak(pak);
        fclose(file);
    }
    else
    {
        file = fopen(texture->filePath, "rb");
        if (!file)
        {
            elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"", texture->filePath);
            return ELF_FALSE;
        }

        fseek(file, 0, SEEK_END);
        texture->dataSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        texture->data = malloc(texture->dataSize);
        fread((char*)texture->data, 1, texture->dataSize, file);

        fclose(file);
    }

    return ELF_TRUE;
}

void elfUnloadTextureData(elfTexture* texture)
{
    if (texture->data)
        free(texture->data);
    texture->data = NULL;
    texture->dataSize = 0;
}
