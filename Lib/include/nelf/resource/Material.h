#pragma once

#include "nelf/Color.h"
#include "nelf/resource/resourceHeader.h"

struct elfTexture;
struct gfxTextureParams;
struct gfxShaderParams;

struct elfMaterial
{
    ELF_RESOURCE_HEADER;

    char* filePath = nullptr;
    elfColor diffuseColor;
    elfColor ambientColor;
    elfColor specularColor;
    float specPower = 0.f;
    // Material not affected by lighting
    bool lighting = true;

    elfTexture* diffuseMap = nullptr;
    elfTexture* normalMap = nullptr;
    elfTexture* heightMap = nullptr;
    elfTexture* specularMap = nullptr;
    elfTexture* lightMap = nullptr;
    elfTexture* cubeMap = nullptr;

    float parallaxScale = 0.f;

    bool alphaTest = false;
    float alphaThreshold = 0.f;
};

elfMaterial* elfCreateMaterial(const char* name);

void elfDestroyMaterial(void* data);

void elfSetMaterialDiffuseColor(elfMaterial* material, float r, float g, float b, float a);
void elfSetMaterialAmbientColor(elfMaterial* material, float r, float g, float b, float a);
void elfSetMaterialSpecularColor(elfMaterial* material, float r, float g, float b, float a);
void elfSetMaterialSpecularPower(elfMaterial* material, float power);
void elfSetMaterialLighting(elfMaterial* material, bool lighting);

void elfSetMaterialName(elfMaterial* material, const char* name);

const char* elfGetMaterialName(elfMaterial* material);
const char* elfGetMaterialFilePath(elfMaterial* material);

elfColor elfGetMaterialDiffuseColor(elfMaterial* material);
elfColor elfGetMaterialAmbientColor(elfMaterial* material);
elfColor elfGetMaterialSpecularColor(elfMaterial* material);
float elfGetMaterialSpecularPower(elfMaterial* material);
unsigned char elfGetMaterialLighting(elfMaterial* material);

void elfSetMaterialDiffuseMap(elfMaterial* material, elfTexture* texture);
void elfSetMaterialNormalMap(elfMaterial* material, elfTexture* texture);
void elfSetMaterialHeightMap(elfMaterial* material, elfTexture* texture);
void elfSetMaterialSpecularMap(elfMaterial* material, elfTexture* texture);
void elfSetMaterialLightMap(elfMaterial* material, elfTexture* texture);
void elfSetMaterialCubeMap(elfMaterial* material, elfTexture* texture);

void elfClearMaterialDiffuseMap(elfMaterial* material);
void elfClearMaterialNormalMap(elfMaterial* material);
void elfClearMaterialHeightMap(elfMaterial* material);
void elfClearMaterialSpecularMap(elfMaterial* material);
void elfClearMaterialLightMap(elfMaterial* material);
void elfClearMaterialCubeMap(elfMaterial* material);

elfTexture* elfGetMaterialDiffuseMap(elfMaterial* material);
elfTexture* elfGetMaterialNormalMap(elfMaterial* material);
elfTexture* elfGetMaterialHeightMap(elfMaterial* material);
elfTexture* elfGetMaterialSpecularMap(elfMaterial* material);
elfTexture* elfGetMaterialLightMap(elfMaterial* material);
elfTexture* elfGetMaterialCubeMap(elfMaterial* material);

void elfSetMaterialParallaxScale(elfMaterial* material, float scale);
void elfSetMaterialAlphaTest(elfMaterial* material, bool alphaTest);
void elfSetMaterialAlphaThreshold(elfMaterial* material, float threshold);

float elfGetMaterialParallaxScale(elfMaterial* material);
bool elfGetMaterialAlphaTest(elfMaterial* material);
float elfGetMaterialAlphaThreshold(elfMaterial* material);

void elfSetTextureParamsDefault(gfxTextureParams* params);

void elfSetMaterial(elfMaterial* material, int mode, gfxShaderParams* shaderParams);