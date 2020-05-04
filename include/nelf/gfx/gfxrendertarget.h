
unsigned char gfxCheckRenderTarget()
{
    GLenum status;

    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        printf("fbo: incomplete attachment\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        printf("fbo: missing attachment\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("fbo: incomplete dimensions\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("fbo: incomplete formats\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        // printf("fbo: incomplete draw buffer\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        printf("fbo: incomplete read buffer\n");
        break;
    }

    if (status != GL_FRAMEBUFFER_COMPLETE_EXT && status != GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)
        return GFX_FALSE;

    return GFX_TRUE;
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

    glGenFramebuffersEXT(1, &renderTarget->fb);

    gfxIncObj(GFX_RENDER_TARGET);

    return renderTarget;
}

void gfxDestroyRenderTarget(void* data)
{
    gfxRenderTarget* renderTarget = (gfxRenderTarget*)data;

    if (renderTarget->fb)
        glDeleteFramebuffersEXT(1, &renderTarget->fb);

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
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderTarget->fb);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + n, GL_TEXTURE_2D, color->id, 0);

    renderTarget->targets[n] = GFX_TRUE;

    if (!driver->renderTarget)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderTarget->fb);

    if (depth)
    {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth->id, 0);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth->id);
    }
    else
    {
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0);
    }

    if (!driver->renderTarget)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    else if (driver->renderTarget != renderTarget)
    {
        rt = driver->renderTarget;
        driver->renderTarget = NULL;
        gfxSetRenderTarget(rt);
    }
}

unsigned char gfxSetRenderTarget(gfxRenderTarget* renderTarget)
{
    int i, j;
    GLenum drawBuffers[16];

    if (driver->renderTarget == renderTarget)
        return GFX_TRUE;

    for (i = 0, j = 0; i < driver->maxDrawBuffers; i++)
    {
        if (renderTarget->targets[i])
        {
            drawBuffers[j] = GL_COLOR_ATTACHMENT0_EXT + i;
            j++;
        }
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderTarget->fb);

    if (j)
        glDrawBuffers(j, drawBuffers);
    else
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (!gfxCheckRenderTarget())
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        return GFX_FALSE;
    }

    driver->renderTarget = renderTarget;

    return GFX_TRUE;
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

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
