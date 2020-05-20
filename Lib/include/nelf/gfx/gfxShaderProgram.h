#pragma once

#include <string>
#include <unordered_map>

#include "nelf/gfx/gfxShaderConfig.h"

struct gfxShaderProgram
{
    gfxShaderProgram* next = nullptr;
    unsigned int id = 0;

    // Uniform locations
    int projectionMatrixLoc = 0;
    int invProjectionMatrixLoc = 0;
    int modelviewMatrixLoc = 0;
    int normalMatrixLoc = 0;

    int texture0Loc = 0;
    int texture1Loc = 0;
    int texture2Loc = 0;
    int texture3Loc = 0;

    int colorMapLoc = 0;
    int normalMapLoc = 0;
    int heightMapLoc = 0;
    int specularMapLoc = 0;
    int colorRampMapLoc = 0;
    int lightMapLoc = 0;
    int cubeMapLoc = 0;

    int shadowProjectionMatrixLoc = 0;
    int shadowMapLoc = 0;

    int ambientColorLoc = 0;
    int diffuseColorLoc = 0;
    int specularColorLoc = 0;
    int shininessLoc = 0;

    int lightPositionLoc = 0;
    int lightColorLoc = 0;
    int lightSpotDirectionLoc = 0;
    int lightRangeLoc = 0;
    int lightFadeRangeLoc = 0;
    int lightInnerConeCosLoc = 0;
    int lightOuterConeCosLoc = 0;

    int cameraPositionLoc = 0;

    int clipStartLoc = 0;
    int clipEndLoc = 0;

    int viewportWidthLoc = 0;
    int viewportHeightLoc = 0;

    int parallaxScaleLoc = 0;
    int alphaThresholdLoc = 0;

    int fogStartLoc = 0;
    int fogEndLoc = 0;
    int fogColorLoc = 0;

    // Uniform locations cache, maps name to index
    std::unordered_map<std::string, int> uniformLocations;

    gfxShaderConfig config;
};

gfxShaderProgram* gfxCreateShaderProgram(const char* vertex, const char* fragment);
void gfxDestroyShaderProgram(gfxShaderProgram* shaderProgram);
void gfxDestroyShaderPrograms(gfxShaderProgram* shaderProgram);

// Uniform location index by name with caching
int gfxGetUniformLocation(gfxShaderProgram* shaderProgram, const char* name);

// Sets as active shader
void gfxSetShaderProgram(gfxShaderProgram* shaderProgram);

// Sets uniform data for active shader
void gfxSetShaderProgramUniform1i(const char* name, int i);
void gfxSetShaderProgramUniform1f(const char* name, float f);
void gfxSetShaderProgramUniformVec2(const char* name, float x, float y);
void gfxSetShaderProgramUniformVec3(const char* name, float x, float y, float z);
void gfxSetShaderProgramUniformVec4(const char* name, float x, float y, float z, float w);
void gfxSetShaderProgramUniformMat4(const char* name, float* matrix);