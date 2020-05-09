#pragma once

#include "nelf/gfx/gfxDrawMode.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxShaderConfig.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxVertexDataType.h"

struct gfxRenderTarget;
struct gfxShaderProgram;

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

    unsigned char dirtyVertexArrays;
    unsigned int verticesDrawn[GFX_MAX_DRAW_MODES];

    gfxShaderConfig shaderConfig;
};