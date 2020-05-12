#pragma once

#include "nelf/gfx/gfxShaderConfig.h"

struct gfxShaderProgram
{
    gfxShaderProgram* next = nullptr;
    unsigned int id;

    int projectionMatrixLoc;
    int invProjectionMatrixLoc;
    int modelviewMatrixLoc;
    int normalMatrixLoc;
    int texture0Loc;
    int texture1Loc;
    int texture2Loc;
    int texture3Loc;
    int colorMapLoc;
    int normalMapLoc;
    int heightMapLoc;
    int specularMapLoc;
    int colorRampMapLoc;
    int lightMapLoc;
    int cubeMapLoc;
    int shadowProjectionMatrixLoc;
    int shadowMapLoc;
    int ambientColorLoc;
    int diffuseColorLoc;
    int specularColorLoc;
    int shininessLoc;
    int lightPositionLoc;
    int lightColorLoc;
    int lightSpotDirectionLoc;
    int lightRangeLoc;
    int lightFadeRangeLoc;
    int lightInnerConeCosLoc;
    int lightOuterConeCosLoc;
    int cameraPositionLoc;
    int clipStartLoc;
    int clipEndLoc;
    int viewportWidthLoc;
    int viewportHeightLoc;
    int parallaxScaleLoc;
    int alphaThresholdLoc;
    int fogStartLoc;
    int fogEndLoc;
    int fogColorLoc;

    gfxShaderConfig config;
};

gfxShaderProgram* gfxCreateShaderProgram(const char* vertex, const char* fragment);
void gfxDestroyShaderProgram(gfxShaderProgram* shaderProgram);
void gfxDestroyShaderPrograms(gfxShaderProgram* shaderProgram);

void gfxSetShaderProgram(gfxShaderProgram* shaderProgram);

void gfxSetShaderProgramUniform1i(const char* name, int i);
void gfxSetShaderProgramUniform1f(const char* name, float f);
void gfxSetShaderProgramUniformVec2(const char* name, float x, float y);
void gfxSetShaderProgramUniformVec3(const char* name, float x, float y, float z);
void gfxSetShaderProgramUniformVec4(const char* name, float x, float y, float z, float w);
void gfxSetShaderProgramUniformMat4(const char* name, float* matrix);