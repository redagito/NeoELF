#include "nelf/gfx/gfxGBuffer.h"

#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxObjectType.h"
#include "nelf/gfx/gfxRenderTarget.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxTextureWrapMode.h"

gfxGbuffer* gfxCreateGbuffer(int width, int height)
{
    gfxGbuffer* gbuffer;

    gbuffer = (gfxGbuffer*)malloc(sizeof(gfxGbuffer));
    memset(gbuffer, 0x0, sizeof(gfxGbuffer));
    gbuffer->objType = GFX_GBUFFER;
    gbuffer->objDestr = gfxDestroyGbuffer;

    gbuffer->width = width;
    gbuffer->height = height;

    gbuffer->depthTex = gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_DEPTH_COMPONENT,
                                           GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
    gbuffer->buf1Tex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_RG, GFX_RG16F, GFX_FLOAT, NULL);
    gbuffer->buf2Tex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_RGBA, GFX_RGBA, GFX_FLOAT, NULL);
    gbuffer->buf3Tex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_RGBA, GFX_RGBA, GFX_FLOAT, NULL);
    gbuffer->diffuseTex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_RGBA, GFX_RGBA16F, GFX_FLOAT, NULL);
    gbuffer->specularTex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_RGBA, GFX_RGBA16F, GFX_FLOAT, NULL);
    gbuffer->mainTex =
        gfxCreate2dTexture(width, height, 0.0f, GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);

    gbuffer->bufRt = gfxCreateRenderTarget(width, height);
    gbuffer->lightRt = gfxCreateRenderTarget(width, height);
    gbuffer->mainRt = gfxCreateRenderTarget(width, height);

    gfxSetRenderTargetColorTexture(gbuffer->bufRt, 0, gbuffer->buf1Tex);
    gfxSetRenderTargetColorTexture(gbuffer->bufRt, 1, gbuffer->buf2Tex);
    gfxSetRenderTargetColorTexture(gbuffer->bufRt, 2, gbuffer->buf3Tex);
    gfxSetRenderTargetDepthTexture(gbuffer->bufRt, gbuffer->depthTex);

    gfxSetRenderTargetColorTexture(gbuffer->lightRt, 0, gbuffer->diffuseTex);
    gfxSetRenderTargetColorTexture(gbuffer->lightRt, 1, gbuffer->specularTex);

    gfxSetRenderTargetColorTexture(gbuffer->mainRt, 0, gbuffer->mainTex);
    gfxSetRenderTargetDepthTexture(gbuffer->mainRt, gbuffer->depthTex);

    gfxIncRef((gfxObject*)gbuffer->depthTex);
    gfxIncRef((gfxObject*)gbuffer->buf1Tex);
    gfxIncRef((gfxObject*)gbuffer->buf2Tex);
    gfxIncRef((gfxObject*)gbuffer->buf3Tex);
    gfxIncRef((gfxObject*)gbuffer->diffuseTex);
    gfxIncRef((gfxObject*)gbuffer->specularTex);
    gfxIncRef((gfxObject*)gbuffer->mainTex);

    return gbuffer;
}

void gfxDestroyGbuffer(void* data)
{
    gfxGbuffer* gbuffer = (gfxGbuffer*)data;

    gfxDestroyRenderTarget(gbuffer->bufRt);
    gfxDestroyRenderTarget(gbuffer->lightRt);
    gfxDestroyRenderTarget(gbuffer->mainRt);

    gfxDecRef((gfxObject*)gbuffer->depthTex);
    gfxDecRef((gfxObject*)gbuffer->buf1Tex);
    gfxDecRef((gfxObject*)gbuffer->buf2Tex);
    gfxDecRef((gfxObject*)gbuffer->buf3Tex);
    gfxDecRef((gfxObject*)gbuffer->diffuseTex);
    gfxDecRef((gfxObject*)gbuffer->specularTex);
    gfxDecRef((gfxObject*)gbuffer->mainTex);
}

void gfxBindGbuffer(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams) { gfxSetRenderTarget(gbuffer->bufRt); }

void gfxBindGbufferLight(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams) { gfxSetRenderTarget(gbuffer->lightRt); }

void gfxBindGbufferMain(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams) { gfxSetRenderTarget(gbuffer->mainRt); }

gfxTexture* gfxGetGbufferDepth(gfxGbuffer* gbuffer) { return gbuffer->depthTex; }

gfxTexture* gfxGetGbufferDiffuse(gfxGbuffer* gbuffer) { return gbuffer->diffuseTex; }

gfxTexture* gfxGetGbufferSpecular(gfxGbuffer* gbuffer) { return gbuffer->specularTex; }

gfxTexture* gfxGetGbufferMain(gfxGbuffer* gbuffer) { return gbuffer->mainTex; }

gfxTexture* gfxGetGbufferBuf1(gfxGbuffer* gbuffer) { return gbuffer->buf1Tex; }

gfxTexture* gfxGetGbufferBuf2(gfxGbuffer* gbuffer) { return gbuffer->buf2Tex; }

gfxTexture* gfxGetGbufferBuf3(gfxGbuffer* gbuffer) { return gbuffer->buf3Tex; }
