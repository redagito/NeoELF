#include "nelf/gfx/gfxShaderParams.h"

#include <glad/glad.h>

#include <cassert>
#include <cstdlib>
#include <cstring>

#include "gfx/gfxLightType.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gfx/gfxDepthFunc.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxProjectionMode.h"
#include "nelf/gfx/gfxShaderGen.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureMapType.h"

enum VertexOrder
{
    GFX_COUNTER_CLOCK_WISE,
    GFX_CLOCK_WISE
};

enum CullMode
{
    GFX_BACK,
    GFX_FRONT
};

void gfxSetColor(gfxColor* color, float r, float g, float b, float a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

void gfxSetShaderParamsDefault(gfxShaderParams* shaderParams)
{
    memset(shaderParams, 0x0, sizeof(gfxShaderParams));

    shaderParams->renderParams.depthTest = true;
    shaderParams->renderParams.depthWrite = true;
    shaderParams->renderParams.depthFunc = GFX_LESS;
    shaderParams->renderParams.colorWrite = true;
    shaderParams->renderParams.alphaWrite = true;
    shaderParams->renderParams.alphaThreshold = 0.99f;
    shaderParams->renderParams.cullFace = true;
    shaderParams->renderParams.lineWidth = 1.0f;
    shaderParams->renderParams.cullFaceMode = GFX_BACK;
    shaderParams->renderParams.frontFace = GFX_COUNTER_CLOCK_WISE;
    shaderParams->renderParams.multisample = true;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    gfxSetColor(&shaderParams->materialParams.specularColor, 1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        shaderParams->textureParams[i].type = GFX_COLOR_MAP;
        shaderParams->textureParams[i].texture = nullptr;
        shaderParams->textureParams[i].projectionMode = GFX_PROJECTION_NONE;
        gfxMatrix4SetIdentity(shaderParams->textureParams[i].matrix);
    }

    gfxMatrix4SetIdentity(shaderParams->projectionMatrix);
    gfxMatrix4SetIdentity(shaderParams->invProjectionMatrix);
    gfxMatrix4SetIdentity(shaderParams->modelviewMatrix);
    gfxMatrix3SetIdentity(shaderParams->normalMatrix);

    shaderParams->shaderProgram = nullptr;
}

void gfxSetMaterialParamsDefault(gfxShaderParams* shaderParams)
{
    gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    gfxSetColor(&shaderParams->materialParams.specularColor, 1.0f, 1.0f, 1.0f, 1.0f);
    shaderParams->materialParams.shininess = 0.0f;
}

void gfxSetTextureParamsDefault(gfxShaderParams* shaderParams)
{
    for (int i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        shaderParams->textureParams[i].type = GFX_COLOR_MAP;
        shaderParams->textureParams[i].texture = nullptr;
        shaderParams->textureParams[i].projectionMode = GFX_PROJECTION_NONE;
        shaderParams->textureParams[i].parallaxScale = 0.25f;
        gfxMatrix4SetIdentity(shaderParams->textureParams[i].matrix);
    }
}

void gfxSetShaderParams(gfxShaderParams* shaderParams)
{
    assert(driver != nullptr);

    // TODO Compares the WHOLE state?
    if (memcmp(&driver->shaderParams.renderParams, &shaderParams->renderParams, sizeof(gfxRenderParams)))
    {
        // Depth state
        if (shaderParams->renderParams.depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        if (shaderParams->renderParams.depthWrite)
            glDepthMask(1);
        else
            glDepthMask(0);

        switch (shaderParams->renderParams.depthFunc)
        {
        case GFX_NEVER:
            glDepthFunc(GL_NEVER);
            break;
        case GFX_LESS:
            glDepthFunc(GL_LESS);
            break;
        case GFX_EQUAL:
            glDepthFunc(GL_EQUAL);
            break;
        case GFX_LEQUAL:
            glDepthFunc(GL_LEQUAL);
            break;
        case GFX_GREATER:
            glDepthFunc(GL_GREATER);
            break;
        case GFX_NOTEQUAL:
            glDepthFunc(GL_NOTEQUAL);
            break;
        case GFX_GEQUAL:
            glDepthFunc(GL_GEQUAL);
            break;
        case GFX_ALWAYS:
            glDepthFunc(GL_ALWAYS);
            break;
        }

        glColorMask(shaderParams->renderParams.colorWrite, shaderParams->renderParams.colorWrite,
                    shaderParams->renderParams.colorWrite, shaderParams->renderParams.alphaWrite);

        // Culling mode
        if (shaderParams->renderParams.cullFace)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        // TODO Deprecated, use "discard" in fragment shader instead
        /*
        if (shaderParams->renderParams.alphaTest)
        {
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, shaderParams->renderParams.alphaThreshold);
        }
        else
            glDisable(GL_ALPHA_TEST);
        */

        // Blending
        switch (shaderParams->renderParams.blendMode)
        {
        case GFX_BLEND_NONE:
            glDisable(GL_BLEND);
            break;
        case GFX_TRANSPARENT:
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case GFX_ADD:
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case GFX_MULTIPLY:
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case GFX_SUBTRACT:
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_SUBTRACT);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        }

        if (!shaderParams->renderParams.offsetScale || !shaderParams->renderParams.offsetBias)
            glDisable(GL_POLYGON_OFFSET_FILL);
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(shaderParams->renderParams.offsetScale, shaderParams->renderParams.offsetBias);
        }

        glLineWidth(shaderParams->renderParams.lineWidth);
        if (shaderParams->renderParams.lineSmooth)
            glEnable(GL_LINE_SMOOTH);
        else
            glDisable(GL_LINE_SMOOTH);

        // Cull settings
        if (shaderParams->renderParams.cullFaceMode == GFX_BACK)
            glCullFace(GL_BACK);
        else
            glCullFace(GL_FRONT);

        if (shaderParams->renderParams.frontFace == GFX_COUNTER_CLOCK_WISE)
            glFrontFace(GL_CCW);
        else
            glFrontFace(GL_CW);

        if (shaderParams->renderParams.wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (shaderParams->renderParams.multisample)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);
    }

    // Transfers texture state, binds required textures to their units
    if (memcmp(&driver->shaderParams.textureParams, &shaderParams->textureParams,
               sizeof(gfxTextureParams) * GFX_MAX_TEXTURES))
    {
        for (int i = 0; i < GFX_MAX_TEXTURES; i++)
        {
            // TODO Maybe skip if texture is nullptr?
            glActiveTexture(GL_TEXTURE0 + i);

            if (shaderParams->textureParams[i].texture)
            {
                if (shaderParams->textureParams[i].type != GFX_CUBE_MAP)
                {
                    glBindTexture(GL_TEXTURE_2D, shaderParams->textureParams[i].texture->id);
                }
                else
                {
                    glBindTexture(GL_TEXTURE_CUBE_MAP, shaderParams->textureParams[i].texture->id);
                }

                switch (shaderParams->textureParams[i].projectionMode)
                {
                case GFX_PROJECTION_NONE:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
                    break;
                case GFX_SHADOW_PROJECTION:
                    // TODO For now shadow mapping is disabled, need to be reimplemented
                    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
                    break;
                }
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }

    // Set active shader program, either supplied or retrieved from elsewhere
    gfxShaderProgram* shaderProgram = shaderParams->shaderProgram;
    if (shaderProgram != nullptr)
    {
        // Check if is active
        if (shaderProgram != driver->shaderParams.shaderProgram)
            glUseProgram(shaderProgram->id);

        // just inputting with values that do not make sense
        // TODO What are these values anyway, and why not use boolean flags to signal invalid values?
        driver->shaderConfig.textures = 255;
        driver->shaderConfig.light = 255;
    }
    else
    {
        // No shader set
        // TODO Looks like this falls back to a default shader?
        gfxShaderConfig shaderConfig;
        gfxGetShaderProgramConfig(shaderParams, &shaderConfig);
        if (memcmp(&driver->shaderConfig, &shaderConfig, sizeof(gfxShaderConfig)))
        {
            memcpy(&driver->shaderConfig, &shaderConfig, sizeof(gfxShaderConfig));
            // GBuffer shader or normal fallback?
            if (shaderParams->gbufferMode > 0)
                shaderProgram = gfxGetGbufShaderProgram(&shaderConfig);
            else
                shaderProgram = gfxGetShaderProgram(&shaderConfig);

            if (shaderProgram)
                glUseProgram(shaderProgram->id);
            else
                return;  // TODO Probably error at this point
        }
        else
        {
            shaderProgram = driver->shaderParams.shaderProgram;
        }
    }

    // Transfer uniform data to shader
    if (shaderProgram->projectionMatrixLoc != -1)
        glUniformMatrix4fv(shaderProgram->projectionMatrixLoc, 1, GL_FALSE, shaderParams->projectionMatrix);
    if (shaderProgram->invProjectionMatrixLoc != -1)
        glUniformMatrix4fv(shaderProgram->invProjectionMatrixLoc, 1, GL_FALSE, shaderParams->invProjectionMatrix);
    if (shaderProgram->modelviewMatrixLoc != -1)
        glUniformMatrix4fv(shaderProgram->modelviewMatrixLoc, 1, GL_FALSE, shaderParams->modelviewMatrix);
    if (shaderProgram->normalMatrixLoc != -1)
        glUniformMatrix3fv(shaderProgram->normalMatrixLoc, 1, GL_FALSE, shaderParams->normalMatrix);

    if (shaderProgram->fogStartLoc != -1)
        glUniform1f(shaderProgram->fogStartLoc, shaderParams->fogParams.start);
    if (shaderProgram->fogEndLoc != -1)
        glUniform1f(shaderProgram->fogEndLoc, shaderParams->fogParams.end);
    if (shaderProgram->fogColorLoc != -1)
        glUniform3fv(shaderProgram->fogColorLoc, 1, &shaderParams->fogParams.color.r);

    if (shaderProgram->ambientColorLoc != -1)
        glUniform4fv(shaderProgram->ambientColorLoc, 1, &shaderParams->materialParams.ambientColor.r);
    if (shaderProgram->diffuseColorLoc != -1)
        glUniform4fv(shaderProgram->diffuseColorLoc, 1, &shaderParams->materialParams.diffuseColor.r);
    if (shaderProgram->specularColorLoc != -1)
        glUniform3fv(shaderProgram->specularColorLoc, 1, &shaderParams->materialParams.specularColor.r);
    if (shaderProgram->shininessLoc != -1)
        glUniform1f(shaderProgram->shininessLoc, shaderParams->materialParams.shininess);

    if (shaderProgram->texture0Loc != -1)
        glUniform1i(shaderProgram->texture0Loc, 0);
    if (shaderProgram->texture1Loc != -1)
        glUniform1i(shaderProgram->texture1Loc, 1);
    if (shaderProgram->texture2Loc != -1)
        glUniform1i(shaderProgram->texture2Loc, 2);
    if (shaderProgram->texture3Loc != -1)
        glUniform1i(shaderProgram->texture3Loc, 3);
    if (shaderProgram->alphaThresholdLoc != -1)
        glUniform1f(shaderProgram->alphaThresholdLoc, shaderParams->renderParams.alphaThreshold);

    // Texture type specific uniform data
    for (int i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        if (shaderParams->textureParams[i].texture != nullptr)
        {
            switch (shaderParams->textureParams[i].type)
            {
            case GFX_COLOR_MAP:
                if (shaderProgram->colorMapLoc != -1)
                    glUniform1i(shaderProgram->colorMapLoc, i);
                break;
            case GFX_NORMAL_MAP:
                if (shaderProgram->normalMapLoc != -1)
                    glUniform1i(shaderProgram->normalMapLoc, i);
                break;
            case GFX_HEIGHT_MAP:
                if (shaderProgram->parallaxScaleLoc != -1)
                    glUniform1f(shaderProgram->parallaxScaleLoc, shaderParams->textureParams[i].parallaxScale);
                if (shaderProgram->heightMapLoc != -1)
                    glUniform1i(shaderProgram->heightMapLoc, i);
                break;
            case GFX_SPECULAR_MAP:
                if (shaderProgram->specularMapLoc != -1)
                    glUniform1i(shaderProgram->specularMapLoc, i);
                break;
            case GFX_COLOR_RAMP_MAP:
                if (shaderProgram->colorRampMapLoc != -1)
                    glUniform1i(shaderProgram->colorRampMapLoc, i);
                break;
            case GFX_LIGHT_MAP:
                if (shaderProgram->lightMapLoc != -1)
                    glUniform1i(shaderProgram->lightMapLoc, i);
                break;
            case GFX_SHADOW_MAP:
                if (shaderProgram->shadowMapLoc != -1)
                    glUniform1i(shaderProgram->shadowMapLoc, i);
                if (shaderProgram->shadowProjectionMatrixLoc != -1)
                    glUniformMatrix4fv(shaderProgram->shadowProjectionMatrixLoc, 1, GL_FALSE,
                                       shaderParams->textureParams[i].matrix);
                break;
            case GFX_CUBE_MAP:
                if (shaderProgram->cubeMapLoc != -1)
                    glUniform1i(shaderProgram->cubeMapLoc, i);
                break;
            }
        }
    }

    // Light parameters
    // TODO Does this only allow a limited amount of lights?
    //      Is this rendering system not using a gbuffer / deferred rendering?
    if (shaderParams->lightParams.type)
    {
        if (shaderProgram->lightPositionLoc != -1)
            glUniform3fv(shaderProgram->lightPositionLoc, 1, &shaderParams->lightParams.position.x);
        if (shaderProgram->lightColorLoc != -1)
            glUniform3fv(shaderProgram->lightColorLoc, 1, &shaderParams->lightParams.color.r);
        if (shaderProgram->lightRangeLoc != -1)
            glUniform1f(shaderProgram->lightRangeLoc, shaderParams->lightParams.range);
        if (shaderProgram->lightFadeRangeLoc != -1)
            glUniform1f(shaderProgram->lightFadeRangeLoc, shaderParams->lightParams.fadeRange);
        if (shaderProgram->lightSpotDirectionLoc != -1)
            glUniform3fv(shaderProgram->lightSpotDirectionLoc, 1, &shaderParams->lightParams.direction.x);
        if (shaderProgram->lightInnerConeCosLoc != -1)
            glUniform1f(shaderProgram->lightInnerConeCosLoc,
                        (float)cos(shaderParams->lightParams.innerCone * GFX_PI_DIV_180));
        if (shaderProgram->lightOuterConeCosLoc != -1)
            glUniform1f(shaderProgram->lightOuterConeCosLoc,
                        (float)cos((shaderParams->lightParams.innerCone + shaderParams->lightParams.outerCone) *
                                   GFX_PI_DIV_180));
    }

    // Camera settings
    // TODO These should be common for all shaders, maybe only set once?
    if (shaderProgram->cameraPositionLoc != -1)
        glUniform3fv(shaderProgram->cameraPositionLoc, 1, &shaderParams->cameraPosition.x);
    if (shaderProgram->clipStartLoc != -1)
        glUniform1f(shaderProgram->clipStartLoc, shaderParams->clipStart);
    if (shaderProgram->clipEndLoc != -1)
        glUniform1f(shaderProgram->clipEndLoc, shaderParams->clipEnd);
    if (shaderProgram->viewportWidthLoc != -1)
        glUniform1i(shaderProgram->viewportWidthLoc, shaderParams->viewportWidth);
    if (shaderProgram->viewportHeightLoc != -1)
        glUniform1i(shaderProgram->viewportHeightLoc, shaderParams->viewportHeight);

    // Set as active config in global driver
    memcpy(&driver->shaderParams, shaderParams, sizeof(gfxShaderParams));
    driver->shaderParams.shaderProgram = shaderProgram;
}
