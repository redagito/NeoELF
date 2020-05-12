#pragma once

#include "nelf/gfx/gfxDrawMode.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxShaderConfig.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxVertexDataType.h"

struct gfxRenderTarget;
struct gfxShaderProgram;
struct gfxTexture;

struct gfxDriver
{
    int formatSizes[GFX_MAX_FORMATS];
    int formats[GFX_MAX_FORMATS];
    int drawModes[GFX_MAX_DRAW_MODES];
    int textureInternalFormats[GFX_MAX_TEXTURE_FORMATS];
    int textureDataFormats[GFX_MAX_TEXTURE_FORMATS];
    int vertexDataDrawModes[GFX_MAX_VERTEX_DATA_TYPES];

    gfxRenderTarget* renderTarget = nullptr;
    gfxShaderProgram* shaderPrograms = nullptr;
    gfxShaderParams shaderParams;

    // Driver version?
    int version = 0;

    int maxTextureSize = 0;
    int maxTextureImageUnits = 0;
    int maxDrawBuffers = 0;
    int maxColorAttachments = 0;
    float maxAnisotropy = 0.f;

    bool dirtyVertexArrays = false;
    unsigned int verticesDrawn[GFX_MAX_DRAW_MODES];

    gfxShaderConfig shaderConfig;
};

// Globally accessible driver
// Initialized on gfx init
extern gfxDriver* driver;

bool gfxInit();
void gfxDeinit();

int gfxGetVersion();

void gfxClearBuffers(float r, float g, float b, float a, float d);
void gfxClearColorBuffer(float r, float g, float b, float a);
void gfxClearDepthBuffer(float d);

void gfxReadPixels(int x, int y, int width, int height, int format, int dataFormat, void* data);

void gfxCopyFrameBuffer(gfxTexture* texture, int ox, int oy, int x, int y, int width, int height);

void gfxResetVerticesDrawn();
int gfxGetVerticesDrawn(unsigned int drawMode);

void gfxPrintGLError();