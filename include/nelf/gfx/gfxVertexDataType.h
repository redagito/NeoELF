#pragma once

// TODO Data type sounds like it would be float, int.., maybe this should be called storage mode or buffer type?
using GfxVertexDataType = int;

#define GFX_VERTEX_DATA_STATIC 0x0000
#define GFX_VERTEX_DATA_DYNAMIC 0x0001
#define GFX_MAX_VERTEX_DATA_TYPES 0x0002