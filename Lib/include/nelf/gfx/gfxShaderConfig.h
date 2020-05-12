#pragma once

struct gfxShaderConfig
{
    // Bitfield GfxTextureMapType
    unsigned short int textures = 0;

    // Number of lights?
    unsigned char light = 0;

    // GfxGBufferMode
    unsigned char gbuffer;

    bool specular = false;
    bool vertexColor = false;
    bool fog = false;

    // GfxBlendMode
    unsigned char blend;
};