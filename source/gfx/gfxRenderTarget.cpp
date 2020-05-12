#include "nelf/gfx/gfxRenderTarget.h"

#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxGeneral.h"
#include "nelf/gfx/gfxObjectType.h"
#include "nelf/gfx/gfxTexture.h"

unsigned char gfxCheckRenderTarget()
{
    GLenum status;

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;

    case GL_FRAMEBUFFER_UNDEFINED:
        printf("fbo: undefined framebuffer");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        printf("fbo: incomplete attachment\n");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        printf("fbo: missing attachment\n");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        // TODO I guess we just ignore this?
        printf("fbo: incomplete draw buffer\n");
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        printf("fbo: incomplete read buffer\n");
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        printf("fbo: unsupported");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        printf("fbo: incomplete multi sample");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        printf("fbo: incomplete layer targets");
        break;

    default:
        printf("fbo: unsupported value returned");
    }

    return false;
}

gfxRenderTarget* gfxCreateRenderTarget(unsigned int width, unsigned int height)
{
    gfxRenderTarget* renderTarget;

    if (!width || !height)
        return NULL;

    renderTarget = (gfxRenderTarget*)malloc(sizeof(gfxRenderTarget));
    memset(renderTarget, 0x0, sizeof(gfxRenderTarget));
    renderTarget->objType = GFX_RENDER_TARGET;
    renderTarget->objDestr = gfxDestroyRenderTarget;

    glGenFramebuffers(1, &renderTarget->fb);

    gfxIncObj(GFX_RENDER_TARGET);

    return renderTarget;
}

void gfxDestroyRenderTarget(void* data)
{
    gfxRenderTarget* renderTarget = (gfxRenderTarget*)data;

    if (renderTarget->fb)
        glDeleteFramebuffers(1, &renderTarget->fb);

    free(renderTarget);

    gfxDecObj(GFX_RENDER_TARGET);
}

gfxRenderTarget* gfxGetCurRenderTarget() { return driver->renderTarget; }

void gfxSetRenderTargetColorTexture(gfxRenderTarget* renderTarget, unsigned int n, gfxTexture* color)
{
    gfxRenderTarget* rt;

    if ((int)n > driver->maxDrawBuffers - 1)
        return;

    if (driver->renderTarget != renderTarget)
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->fb);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + n, GL_TEXTURE_2D, color->id, 0);

    renderTarget->targets[n] = true;

    if (!driver->renderTarget)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else if (driver->renderTarget != renderTarget)
    {
        rt = driver->renderTarget;
        driver->renderTarget = NULL;
        gfxSetRenderTarget(rt);
    }
}

void gfxSetRenderTargetDepthTexture(gfxRenderTarget* renderTarget, gfxTexture* depth)
{
    gfxRenderTarget* rt;

    if (driver->renderTarget != renderTarget)
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->fb);

    if (depth)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->id, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->id);
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0);
    }

    if (!driver->renderTarget)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else if (driver->renderTarget != renderTarget)
    {
        rt = driver->renderTarget;
        driver->renderTarget = NULL;
        gfxSetRenderTarget(rt);
    }
}

bool gfxSetRenderTarget(gfxRenderTarget* renderTarget)
{
    int i, j;
    GLenum drawBuffers[16];

    if (driver->renderTarget == renderTarget)
        return true;

    for (i = 0, j = 0; i < driver->maxDrawBuffers; i++)
    {
        if (renderTarget->targets[i])
        {
            drawBuffers[j] = GL_COLOR_ATTACHMENT0 + i;
            j++;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->fb);

    if (j)
        glDrawBuffers(j, drawBuffers);
    else
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (!gfxCheckRenderTarget())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    driver->renderTarget = renderTarget;

    return true;
}

void gfxDisableRenderTarget()
{
    int i, j;

    if (!driver->renderTarget)
        return;

    for (i = 0, j = 0; i < driver->maxDrawBuffers; i++)
    {
        if (driver->renderTarget->targets[i])
        {
            j++;
        }
    }

    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

    driver->renderTarget = NULL;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
