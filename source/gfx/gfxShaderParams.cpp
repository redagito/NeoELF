#include "nelf/gfx/gfxShaderParams.h"

#include <glad/glad.h>

#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxCullMode.h"
#include "nelf/gfx/gfxDepthFunc.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxProjectionMode.h"
#include "nelf/gfx/gfxTextureMapType.h"
#include "nelf/gfx/gfxVertexOrder.h"

void gfxSetColor(gfxColor* color, float r, float g, float b, float a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

void gfxSetShaderParamsDefault(gfxShaderParams* shaderParams)
{
    int i;

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

    for (i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        shaderParams->textureParams[i].type = GFX_COLOR_MAP;
        shaderParams->textureParams[i].texture = NULL;
        shaderParams->textureParams[i].projectionMode = GFX_NONE;
        gfxMatrix4SetIdentity(shaderParams->textureParams[i].matrix);
    }

    gfxMatrix4SetIdentity(shaderParams->projectionMatrix);
    gfxMatrix4SetIdentity(shaderParams->invProjectionMatrix);
    gfxMatrix4SetIdentity(shaderParams->modelviewMatrix);
    gfxMatrix3SetIdentity(shaderParams->normalMatrix);

    shaderParams->shaderProgram = NULL;
}

void gfxSetMaterialParamsDefault(gfxShaderParams* shaderParams)
{
    gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    gfxSetColor(&shaderParams->materialParams.specularColor, 1.0f, 1.0f, 1.0f, 1.0f);
    shaderParams->materialParams.shininess = 0.0f;
}

void gfxSetTextureParamsDefault(gfxShaderParams* shaderParams)
{
    int i;

    for (i = 0; i < GFX_MAX_TEXTURES; i++)
    {
        shaderParams->textureParams[i].type = GFX_COLOR_MAP;
        shaderParams->textureParams[i].texture = NULL;
        shaderParams->textureParams[i].projectionMode = GFX_NONE;
        shaderParams->textureParams[i].parallaxScale = 0.25f;
        gfxMatrix4SetIdentity(shaderParams->textureParams[i].matrix);
    }
}

void gfxSetShaderParams(gfxShaderParams* shaderParams)
{
    int i;
    gfxShaderConfig shaderConfig;
    gfxShaderProgram* shaderProgram = NULL;

    if (memcmp(&driver->shaderParams.renderParams, &shaderParams->renderParams, sizeof(gfxRenderParams)))
    {
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

        if (shaderParams->renderParams.cullFace)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        if (shaderParams->renderParams.alphaTest)
        {
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, shaderParams->renderParams.alphaThreshold);
        }
        else
            glDisable(GL_ALPHA_TEST);

        switch (shaderParams->renderParams.blendMode)
        {
        case GFX_NONE:
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

    // TODO Wow, but still needed?
    if (driver->version < 200)
    {
        if (memcmp(&driver->shaderParams.textureParams, &shaderParams->textureParams,
                   sizeof(gfxTextureParams) * GFX_MAX_TEXTURES))
        {
            for (i = 0; i < GFX_MAX_TEXTURES; i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glClientActiveTexture(GL_TEXTURE0 + i);

                if (shaderParams->textureParams[i].texture && (shaderParams->textureParams[i].type == GFX_COLOR_MAP ||
                                                               shaderParams->textureParams[i].type == GFX_LIGHT_MAP ||
                                                               shaderParams->textureParams[i].type == GFX_SHADOW_MAP))
                {
                    if (!glIsEnabled(GL_TEXTURE_2D))
                        glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, shaderParams->textureParams[i].texture->id);

                    if (shaderParams->textureParams[i].type == GFX_SHADOW_MAP)
                    {
                        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                        glTexGenfv(GL_S, GL_EYE_PLANE, &shaderParams->textureParams[i].matrix[0]);
                        glEnable(GL_TEXTURE_GEN_S);

                        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                        glTexGenfv(GL_T, GL_EYE_PLANE, &shaderParams->textureParams[i].matrix[4]);
                        glEnable(GL_TEXTURE_GEN_T);

                        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                        glTexGenfv(GL_R, GL_EYE_PLANE, &shaderParams->textureParams[i].matrix[8]);
                        glEnable(GL_TEXTURE_GEN_R);

                        glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                        glTexGenfv(GL_Q, GL_EYE_PLANE, &shaderParams->textureParams[i].matrix[12]);
                        glEnable(GL_TEXTURE_GEN_Q);
                    }
                    else
                    {
                        glDisable(GL_TEXTURE_GEN_S);
                        glDisable(GL_TEXTURE_GEN_T);
                        glDisable(GL_TEXTURE_GEN_R);
                        glDisable(GL_TEXTURE_GEN_Q);
                    }

                    if (shaderParams->textureParams[i].projectionMode == GFX_SHADOW_PROJECTION)
                    {
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
                    }
                    else
                    {
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
                    }
                }
                else
                {
                    glDisable(GL_TEXTURE_2D);
                }
            }
        }

        if (memcmp(&driver->shaderParams.fogParams, &shaderParams->fogParams, sizeof(gfxFogParams)))
        {
            if (shaderParams->fogParams.mode)
            {
                glEnable(GL_FOG);
                glFogi(GL_FOG_MODE, GL_LINEAR);
                glFogf(GL_FOG_START, shaderParams->fogParams.start);
                glFogf(GL_FOG_END, shaderParams->fogParams.start);
                glFogfv(GL_FOG_COLOR, &shaderParams->fogParams.color.r);
            }
            else
            {
                glDisable(GL_FOG);
            }
        }

        if (memcmp(&driver->shaderParams.lightParams, &shaderParams->lightParams, sizeof(gfxLightParams)))
        {
            if (shaderParams->lightParams.type)
            {
                float position[4];

                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(shaderParams->cameraMatrix);

                glLightfv(GL_LIGHT0, GL_DIFFUSE, &shaderParams->lightParams.color.r);
                glLightfv(GL_LIGHT0, GL_SPECULAR, &shaderParams->lightParams.color.r);
                glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,
                         10.0f / (shaderParams->lightParams.range + shaderParams->lightParams.fadeRange));

                if (shaderParams->lightParams.type != GFX_SPOT_LIGHT)
                {
                    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f);
                    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
                }
                else
                {
                    // some really wild guesses for spot exponent here, doesn't work that well with the current system.
                    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT,
                             shaderParams->lightParams.outerCone /
                                 (shaderParams->lightParams.innerCone + shaderParams->lightParams.outerCone) * 7.5);
                    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,
                             shaderParams->lightParams.innerCone + shaderParams->lightParams.outerCone);
                    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, &shaderParams->lightParams.direction.x);
                }

                if (shaderParams->lightParams.type == GFX_SUN_LIGHT)
                {
                    position[0] = -shaderParams->lightParams.direction.x;
                    position[1] = -shaderParams->lightParams.direction.y;
                    position[2] = -shaderParams->lightParams.direction.z;
                    position[3] = 0.0f;
                }
                else
                {
                    memcpy(position, &shaderParams->lightParams.position.x, sizeof(float) * 3);
                    position[3] = 1.0f;
                }

                glLightfv(GL_LIGHT0, GL_POSITION, position);
            }
            else
            {
                glDisable(GL_LIGHTING);
            }
        }

        if (memcmp(&driver->shaderParams.materialParams, &shaderParams->materialParams, sizeof(gfxMaterialParams)))
        {
            glColor4f(shaderParams->materialParams.diffuseColor.r, shaderParams->materialParams.diffuseColor.g,
                      shaderParams->materialParams.diffuseColor.b, shaderParams->materialParams.diffuseColor.a);
            glMaterialfv(GL_FRONT, GL_AMBIENT, &shaderParams->materialParams.ambientColor.r);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, &shaderParams->materialParams.diffuseColor.r);
            glMaterialfv(GL_FRONT, GL_SPECULAR, &shaderParams->materialParams.specularColor.r);
            glMaterialf(GL_FRONT, GL_SHININESS, shaderParams->materialParams.shininess);
        }

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(shaderParams->projectionMatrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(shaderParams->modelviewMatrix);
    }
    else
    {
        if (memcmp(&driver->shaderParams.textureParams, &shaderParams->textureParams,
                   sizeof(gfxTextureParams) * GFX_MAX_TEXTURES))
        {
            for (i = 0; i < GFX_MAX_TEXTURES; i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glClientActiveTexture(GL_TEXTURE0 + i);

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
                    case GFX_NONE:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
                        break;
                    case GFX_SHADOW_PROJECTION:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
                        break;
                    }
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }

        shaderProgram = shaderParams->shaderProgram;

        if (shaderProgram)
        {
            if (shaderProgram != driver->shaderParams.shaderProgram)
                glUseProgram(shaderProgram->id);

            // just inputting with values that do not make sense
            driver->shaderConfig.textures = 255;
            driver->shaderConfig.light = 255;
        }
        else
        {
            gfxGetShaderProgramConfig(shaderParams, &shaderConfig);
            if (memcmp(&driver->shaderConfig, &shaderConfig, sizeof(gfxShaderConfig)))
            {
                memcpy(&driver->shaderConfig, &shaderConfig, sizeof(gfxShaderConfig));
                if (shaderParams->gbufferMode > 0)
                    shaderProgram = gfxGetGbufShaderProgram(&shaderConfig);
                else
                    shaderProgram = gfxGetShaderProgram(&shaderConfig);
                if (shaderProgram)
                    glUseProgram(shaderProgram->id);
                else
                    return;
            }
            else
            {
                shaderProgram = driver->shaderParams.shaderProgram;
            }
        }

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

        for (i = 0; i < GFX_MAX_TEXTURES; i++)
        {
            if (shaderParams->textureParams[i].texture)
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
    }

    memcpy(&driver->shaderParams, shaderParams, sizeof(gfxShaderParams));
    driver->shaderParams.shaderProgram = shaderProgram;
}
