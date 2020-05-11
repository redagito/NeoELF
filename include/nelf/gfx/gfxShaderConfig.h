#pragma once

struct gfxShaderConfig
{
    // Bitfield GfxTextureMapType
    unsigned short int textures = 0;

    bool light = false;

    // GfxGBufferMode
    unsigned char gbuffer;

    bool specular = false;
    bool vertexColor = false;
    bool fog = false;

    // GfxBlendMode
    unsigned char blend;
};