#include "nelf/gfx/gfxDriver.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nelf/Log.h"
#include "nelf/gfx/gfxGeneral.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTexture.h"

gfxDriver* driver = nullptr;

bool gfxInit()
{
    if (driver)
        return true;

    gfxGen = gfxCreateGeneral();

    driver = (gfxDriver*)malloc(sizeof(gfxDriver));
    memset(driver, 0x0, sizeof(gfxDriver));

    driver->formats[GFX_FLOAT] = GL_FLOAT;
    driver->formats[GFX_INT] = GL_INT;
    driver->formats[GFX_UINT] = GL_UNSIGNED_INT;
    driver->formats[GFX_SHORT] = GL_SHORT;
    driver->formats[GFX_USHORT] = GL_UNSIGNED_SHORT;
    driver->formats[GFX_BYTE] = GL_BYTE;
    driver->formats[GFX_UBYTE] = GL_UNSIGNED_BYTE;

    driver->formatSizes[GFX_FLOAT] = sizeof(float);
    driver->formatSizes[GFX_INT] = sizeof(int);
    driver->formatSizes[GFX_UINT] = sizeof(unsigned int);
    driver->formatSizes[GFX_SHORT] = sizeof(short int);
    driver->formatSizes[GFX_USHORT] = sizeof(unsigned short int);
    driver->formatSizes[GFX_BYTE] = sizeof(char);
    driver->formatSizes[GFX_UBYTE] = sizeof(unsigned char);

    driver->drawModes[GFX_POINTS] = GL_POINTS;
    driver->drawModes[GFX_LINES] = GL_LINES;
    driver->drawModes[GFX_LINE_LOOP] = GL_LINE_LOOP;
    driver->drawModes[GFX_LINE_STRIP] = GL_LINE_STRIP;
    driver->drawModes[GFX_TRIANGLES] = GL_TRIANGLES;
    driver->drawModes[GFX_TRIANGLE_STRIP] = GL_TRIANGLE_STRIP;
    driver->drawModes[GFX_TRIANGLE_FAN] = GL_TRIANGLE_FAN;

    driver->textureInternalFormats[GFX_LUMINANCE] = GL_LUMINANCE;
    driver->textureInternalFormats[GFX_LUMINANCE_ALPHA] = GL_LUMINANCE_ALPHA;
    driver->textureInternalFormats[GFX_RGB] = GL_RGB;
    driver->textureInternalFormats[GFX_RGBA] = GL_RGBA;
    driver->textureInternalFormats[GFX_BGR] = GL_BGR;
    driver->textureInternalFormats[GFX_BGRA] = GL_BGRA;
    driver->textureInternalFormats[GFX_RGB16F] = GL_RGB16F;
    driver->textureInternalFormats[GFX_RGB32F] = GL_RGB32F;
    driver->textureInternalFormats[GFX_RGBA16F] = GL_RGBA16F;
    driver->textureInternalFormats[GFX_RGBA32F] = GL_RGBA32F;
    driver->textureInternalFormats[GFX_COMPRESSED_RGB] = GL_COMPRESSED_RGB;
    driver->textureInternalFormats[GFX_COMPRESSED_RGBA] = GL_COMPRESSED_RGBA;
    driver->textureInternalFormats[GFX_DEPTH_COMPONENT] = GL_DEPTH_COMPONENT;
    driver->textureInternalFormats[GFX_R] = GL_R;
    driver->textureInternalFormats[GFX_RG] = GL_RG;
    driver->textureInternalFormats[GFX_R16F] = GL_R16F;
    driver->textureInternalFormats[GFX_R32F] = GL_R32F;
    driver->textureInternalFormats[GFX_RG16F] = GL_RG16F;
    driver->textureInternalFormats[GFX_RG32F] = GL_RG32F;

    driver->textureDataFormats[GFX_LUMINANCE] = GL_LUMINANCE;
    driver->textureDataFormats[GFX_LUMINANCE_ALPHA] = GL_LUMINANCE_ALPHA;
    driver->textureDataFormats[GFX_RGB] = GL_RGB;
    driver->textureDataFormats[GFX_RGBA] = GL_RGBA;
    driver->textureDataFormats[GFX_BGR] = GL_BGR;
    driver->textureDataFormats[GFX_BGRA] = GL_BGRA;
    driver->textureDataFormats[GFX_RGB16F] = GL_RGB;
    driver->textureDataFormats[GFX_RGB32F] = GL_RGB;
    driver->textureDataFormats[GFX_RGBA16F] = GL_RGBA;
    driver->textureDataFormats[GFX_RGBA32F] = GL_RGBA;
    driver->textureDataFormats[GFX_COMPRESSED_RGB] = GL_RGB;
    driver->textureDataFormats[GFX_COMPRESSED_RGBA] = GL_RGBA;
    driver->textureDataFormats[GFX_DEPTH_COMPONENT] = GL_DEPTH_COMPONENT;
    driver->textureDataFormats[GFX_R] = GL_R;
    driver->textureDataFormats[GFX_RG] = GL_RG;
    driver->textureDataFormats[GFX_R16F] = GL_R;
    driver->textureDataFormats[GFX_R32F] = GL_R;
    driver->textureDataFormats[GFX_RG16F] = GL_RG;
    driver->textureDataFormats[GFX_RG32F] = GL_RG;

    driver->vertexDataDrawModes[GFX_VERTEX_DATA_STATIC] = GL_STATIC_DRAW;
    driver->vertexDataDrawModes[GFX_VERTEX_DATA_DYNAMIC] = GL_DYNAMIC_DRAW;

    // just inputting with values that do not make sense
    driver->shaderConfig.textures = 255;
    driver->shaderConfig.light = 255;

    // Load opengl functions
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        elfLogWrite("Failed to init OpenGL context\n");
        return false;
    }

    driver->version = GLVersion.major * 100 + GLVersion.minor * 10;
    elfLogWrite("OpenGL %s; %s; %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

    if (driver->version < 110)
    {
        elfLogWrite("OpenGL version 1.1 not supported\n");
        return false;
    }

    if (driver->version < 140)
    {
        elfLogWrite("warning: OpenGL version 1.4 not supported, npot textures will not display correctly\n");
    }

    // checks for deferred rendering

    /*if(!glewIsSupported("GL_ARB_texture_float"))
    {
            elfLogWrite("GL_ARB_texture_float not supported!\n");
            return GFX_FALSE;
    }

    if(!glewIsSupported("GL_ARB_texture_rg"))
    {
            elfLogWrite("GL_ARB_texture_rg not supported!\n");
            return GFX_FALSE;
    }*/

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &driver->maxTextureSize);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &driver->maxTextureImageUnits);
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &driver->maxDrawBuffers);
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &driver->maxColorAttachments);
    // This is actualy in core since OpenGL 4.6
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &driver->maxAnisotropy);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return true;
}

void gfxDeinit()
{
    if (!driver)
        return;

    if (driver->shaderPrograms)
        gfxDestroyShaderPrograms(driver->shaderPrograms);

    free(driver);
    driver = NULL;

    if (gfxGetGlobalRefCount() > 0)
    {
        elfLogWrite("error: possible memory leak in GFX, [%d] references not dereferenced\n", gfxGetGlobalRefCount());
        gfxDumpRefTable();
    }

    if (gfxGetGlobalRefCount() < 0)
    {
        elfLogWrite("error: possible double free in GFX, [%d] negative reference count\n", gfxGetGlobalRefCount());
        gfxDumpRefTable();
    }

    if (gfxGetGlobalObjCount() > 0)
    {
        elfLogWrite("error: possible memory leak in GFX, [%d] objects not destroyed\n", gfxGetGlobalObjCount());
        gfxDumpObjTable();
    }

    if (gfxGetGlobalObjCount() < 0)
    {
        elfLogWrite("error: possible double free in GFX, [%d] negative object count\n", gfxGetGlobalObjCount());
        gfxDumpObjTable();
    }

    gfxDestroyGeneral(gfxGen);
}

int gfxGetVersion() { return driver->version; }

void gfxClearBuffers(float r, float g, float b, float a, float d)
{
    glClearColor(r, g, b, a);
    glClearDepth(d);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfxClearColorBuffer(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gfxClearDepthBuffer(float d)
{
    glClearDepth(d);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void gfxReadPixels(int x, int y, int width, int height, int format, int dataFormat, void* data)
{
    glReadPixels(x, y, width, height, driver->textureDataFormats[format], driver->formats[dataFormat], data);
}

void gfxCopyFrameBuffer(gfxTexture* texture, int ox, int oy, int x, int y, int width, int height)
{
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, x, y, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    driver->shaderParams.textureParams[0].texture = NULL;
}

void gfxResetVerticesDrawn()
{
    memset(driver->verticesDrawn, 0x0, sizeof(unsigned int) * GFX_MAX_DRAW_MODES);
    driver->verticesDrawn[GFX_TRIANGLES] = 0;
    driver->verticesDrawn[GFX_TRIANGLE_STRIP] = 0;
}

int gfxGetVerticesDrawn(unsigned int drawMode) { return driver->verticesDrawn[drawMode]; }

static const char* getErrorString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "No error";
    case GL_INVALID_OPERATION:
        return "INVALID OPERATION";
    case GL_INVALID_ENUM:
        return "INVALID ENUM";
    case GL_INVALID_VALUE:
        return "INVALID VALUE";
    case GL_OUT_OF_MEMORY:
        return "OUT OF MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "INVALID FRAMEBUFFER OPERATION";
    default:
        return "Unknown error";
    }
}

void gfxPrintGLError()
{
    GLenum err;
    err = glGetError();
    printf("%s\n", getErrorString(err));
}
