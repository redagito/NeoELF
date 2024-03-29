#include "nelf/gfx/gfxDriver.h"

#include <glad/glad.h>
// GLAD header first
#include <GLFW/glfw3.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "nelf/Log.h"

#include "gfx/gfxGeneral.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTexture.h"

gfxDriver* driver = nullptr;

static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                   const GLchar* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
}

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

    // Luminance is greysacle without alpha
    // Luminance_alpha is greyscale with alpha
    // For luminance, use GL_RED and store the greyscale in red channel and
    // set green and blue to 0 and alpha to 1
    // For luminance_alpha, use GL_RG and store greyscale in red and alpha in green
    // channel and set blue to 0 and alpha to 1
    driver->textureInternalFormats[GFX_LUMINANCE] = GL_RED;
    driver->textureInternalFormats[GFX_LUMINANCE_ALPHA] = GL_RG;
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
    driver->textureInternalFormats[GFX_R] = GL_RED;
    driver->textureInternalFormats[GFX_RG] = GL_RG;
    driver->textureInternalFormats[GFX_R16F] = GL_R16F;
    driver->textureInternalFormats[GFX_R32F] = GL_R32F;
    driver->textureInternalFormats[GFX_RG16F] = GL_RG16F;
    driver->textureInternalFormats[GFX_RG32F] = GL_RG32F;

    driver->textureDataFormats[GFX_LUMINANCE] = GL_RED;
    driver->textureDataFormats[GFX_LUMINANCE_ALPHA] = GL_RG;
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
    driver->textureDataFormats[GFX_R] = GL_RED;
    driver->textureDataFormats[GFX_RG] = GL_RG;
    driver->textureDataFormats[GFX_R16F] = GL_RED;
    driver->textureDataFormats[GFX_R32F] = GL_RED;
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

    // Debug context
    int flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    driver->version = GLVersion.major * 100 + GLVersion.minor * 10;
    elfLogWrite("OpenGL %s; %s; %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

    if (driver->version < 110)
    {
        elfLogWrite("error: OpenGL version 1.1 not supported\n");
        return false;
    }

    if (driver->version < 140)
    {
        elfLogWrite("error: OpenGL version 1.4 not supported, npot textures will not display correctly\n");
        return false;
    }

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &driver->maxTextureSize);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &driver->maxTextureImageUnits);
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &driver->maxDrawBuffers);
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &driver->maxColorAttachments);
    // This is actually in core since OpenGL 4.6
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &driver->maxAnisotropy);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return true;
}

void gfxDeinit()
{
    if (driver == nullptr)
        return;

    if (driver->shaderPrograms)
        gfxDestroyShaderPrograms(driver->shaderPrograms);

    free(driver);
    driver = nullptr;

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

int gfxGetVersion()
{
    if (driver == nullptr)
        return 0;
    return driver->version;
}

void gfxClearBuffers(float r, float g, float b, float a, float d)
{
    assert(driver != nullptr);
    glClearColor(r, g, b, a);
    glClearDepth(d);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfxClearColorBuffer(float r, float g, float b, float a)
{
    assert(driver != nullptr);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gfxClearDepthBuffer(float d)
{
    assert(driver != nullptr);
    glClearDepth(d);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void gfxReadPixels(int x, int y, int width, int height, int format, int dataFormat, void* data)
{
    assert(driver != nullptr);
    glReadPixels(x, y, width, height, driver->textureDataFormats[format], driver->formats[dataFormat], data);
}

void gfxCopyFrameBuffer(gfxTexture* texture, int ox, int oy, int x, int y, int width, int height)
{
    assert(driver != nullptr);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    // Copies from current GL_READ_BUFFER to texture
    // TODO Check if this is used correctly
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, x, y, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    driver->shaderParams.textureParams[0].texture = nullptr;
}

void gfxResetVerticesDrawn()
{
    assert(driver != nullptr);
    memset(driver->verticesDrawn, 0x0, sizeof(unsigned int) * GFX_MAX_DRAW_MODES);
    driver->verticesDrawn[GFX_TRIANGLES] = 0;
    driver->verticesDrawn[GFX_TRIANGLE_STRIP] = 0;
}

int gfxGetVerticesDrawn(unsigned int drawMode)
{
    assert(driver != nullptr);
    return driver->verticesDrawn[drawMode];
}

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
    assert(driver != nullptr);
    GLenum err;
    err = glGetError();
    printf("%s\n", getErrorString(err));
}
