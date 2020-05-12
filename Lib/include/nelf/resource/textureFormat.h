#pragma once

using TextureFormat = int;

// <mdoc> TEXTURE FORMATS <mdocc> The texture formats returned by elf.GetTextureFormat
#define ELF_LUMINANCE 0x0000
#define ELF_LUMINANCE_ALPHA 0x0001
#define ELF_RGB 0x0002
#define ELF_RGBA 0x0003
#define ELF_BGR 0x0004
#define ELF_BGRA 0x0005
#define ELF_RGB16F 0x0006
#define ELF_RGB32F 0x0007
#define ELF_RGBA16F 0x0008
#define ELF_RGBA32F 0x0009
#define ELF_ALPHA32F 0x000A
#define ELF_DEPTH_COMPONENT 0x000B
#define ELF_COMPRESSED_RGB 0x000C
#define ELF_COMPRESSED_RGBA 0x000D