
gfxShaderProgram* gfxCreateShaderProgram(const char* vertex, const char* fragment)
{
    const GLchar* myStringPtrs[1];
    int shaderLength;
    int success;
    char* infoLog;
    int infoLogLength;
    GLuint myVertexShader;
    GLuint myFragmentShader;
    gfxShaderProgram* shaderProgram;

    if (driver->version < 200)
        return NULL;

    myVertexShader = glCreateShader(GL_VERTEX_SHADER);

    if (vertex)
    {
        myStringPtrs[0] = vertex;
        shaderLength = strlen(vertex);

        glShaderSource(myVertexShader, 1, (const GLchar**)myStringPtrs, &shaderLength);
        glCompileShader(myVertexShader);

        glGetShaderiv(myVertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderiv(myVertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

            infoLog = (char*)malloc(sizeof(char) * (infoLogLength + 1));
            memset(infoLog, '\0', sizeof(char) * (infoLogLength + 1));

            glGetShaderInfoLog(myVertexShader, infoLogLength, 0, infoLog);
            elfLogWrite("error: could not compile vertex shader, log message:\n%s", infoLog);
            glDeleteShader(myVertexShader);

            free(infoLog);

            return NULL;
        }
    }

    myFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    if (fragment)
    {
        myStringPtrs[0] = fragment;
        shaderLength = strlen(fragment);

        glShaderSource(myFragmentShader, 1, (const GLchar**)myStringPtrs, &shaderLength);
        glCompileShader(myFragmentShader);

        glGetShaderiv(myFragmentShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderiv(myFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

            infoLog = (char*)malloc(sizeof(char) * (infoLogLength + 1));
            memset(infoLog, '\0', sizeof(char) * (infoLogLength + 1));

            glGetShaderInfoLog(myFragmentShader, infoLogLength, 0, infoLog);
            elfLogWrite("error: could not compile fragment shader, log message:\n%s", infoLog);

            if (vertex)
                glDeleteShader(myVertexShader);
            glDeleteShader(myFragmentShader);

            free(infoLog);

            return NULL;
        }
    }

    shaderProgram = (gfxShaderProgram*)malloc(sizeof(gfxShaderProgram));
    memset(shaderProgram, 0x0, sizeof(gfxShaderProgram));

    shaderProgram->id = glCreateProgram();

    if (vertex)
        glAttachShader(shaderProgram->id, myVertexShader);
    if (fragment)
        glAttachShader(shaderProgram->id, myFragmentShader);

    glBindAttribLocation(shaderProgram->id, GFX_VERTEX, "elf_VertexAttr");
    glBindAttribLocation(shaderProgram->id, GFX_NORMAL, "elf_NormalAttr");
    glBindAttribLocation(shaderProgram->id, GFX_TEX_COORD, "elf_TexCoordAttr");
    glBindAttribLocation(shaderProgram->id, GFX_COLOR, "elf_ColorAttr");
    glBindAttribLocation(shaderProgram->id, GFX_TANGENT, "elf_TangentAttr");

    glLinkProgram(shaderProgram->id);

    if (vertex)
        glDeleteShader(myVertexShader);
    if (fragment)
        glDeleteShader(myFragmentShader);

    glGetProgramiv(shaderProgram->id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramiv(shaderProgram->id, GL_INFO_LOG_LENGTH, &infoLogLength);

        infoLog = (char*)malloc(sizeof(char) * (infoLogLength + 1));
        memset(infoLog, '\0', sizeof(char) * (infoLogLength + 1));

        glGetProgramInfoLog(shaderProgram->id, infoLogLength, 0, infoLog);
        elfLogWrite("error: could not link shader, log message:\n%s", infoLog);

        gfxDestroyShaderProgram(shaderProgram);

        free(infoLog);

        return NULL;
    }

    shaderProgram->projectionMatrixLoc = glGetUniformLocation(shaderProgram->id, "elf_ProjectionMatrix");
    shaderProgram->invProjectionMatrixLoc = glGetUniformLocation(shaderProgram->id, "elf_InvProjectionMatrix");
    shaderProgram->modelviewMatrixLoc = glGetUniformLocation(shaderProgram->id, "elf_ModelviewMatrix");
    shaderProgram->normalMatrixLoc = glGetUniformLocation(shaderProgram->id, "elf_NormalMatrix");
    shaderProgram->texture0Loc = glGetUniformLocation(shaderProgram->id, "elf_Texture0");
    shaderProgram->texture1Loc = glGetUniformLocation(shaderProgram->id, "elf_Texture1");
    shaderProgram->texture2Loc = glGetUniformLocation(shaderProgram->id, "elf_Texture2");
    shaderProgram->texture3Loc = glGetUniformLocation(shaderProgram->id, "elf_Texture3");
    shaderProgram->colorMapLoc = glGetUniformLocation(shaderProgram->id, "elf_ColorMap");
    shaderProgram->normalMapLoc = glGetUniformLocation(shaderProgram->id, "elf_NormalMap");
    shaderProgram->heightMapLoc = glGetUniformLocation(shaderProgram->id, "elf_HeightMap");
    shaderProgram->specularMapLoc = glGetUniformLocation(shaderProgram->id, "elf_SpecularMap");
    shaderProgram->colorRampMapLoc = glGetUniformLocation(shaderProgram->id, "elf_ColorRampMap");
    shaderProgram->lightMapLoc = glGetUniformLocation(shaderProgram->id, "elf_LightMap");
    shaderProgram->shadowProjectionMatrixLoc = glGetUniformLocation(shaderProgram->id, "elf_ShadowProjectionMatrix");
    shaderProgram->shadowMapLoc = glGetUniformLocation(shaderProgram->id, "elf_ShadowMap");
    shaderProgram->cubeMapLoc = glGetUniformLocation(shaderProgram->id, "elf_CubeMap");
    shaderProgram->ambientColorLoc = glGetUniformLocation(shaderProgram->id, "elf_AmbientColor");
    shaderProgram->diffuseColorLoc = glGetUniformLocation(shaderProgram->id, "elf_DiffuseColor");
    shaderProgram->specularColorLoc = glGetUniformLocation(shaderProgram->id, "elf_SpecularColor");
    shaderProgram->shininessLoc = glGetUniformLocation(shaderProgram->id, "elf_Shininess");
    shaderProgram->lightPositionLoc = glGetUniformLocation(shaderProgram->id, "elf_LightPosition");
    shaderProgram->lightColorLoc = glGetUniformLocation(shaderProgram->id, "elf_LightColor");
    shaderProgram->lightSpotDirectionLoc = glGetUniformLocation(shaderProgram->id, "elf_LightSpotDirection");
    shaderProgram->lightRangeLoc = glGetUniformLocation(shaderProgram->id, "elf_LightRange");
    shaderProgram->lightFadeRangeLoc = glGetUniformLocation(shaderProgram->id, "elf_LightFadeRange");
    shaderProgram->lightInnerConeCosLoc = glGetUniformLocation(shaderProgram->id, "elf_LightInnerConeCos");
    shaderProgram->lightOuterConeCosLoc = glGetUniformLocation(shaderProgram->id, "elf_LightOuterConeCos");
    shaderProgram->cameraPositionLoc = glGetUniformLocation(shaderProgram->id, "elf_CameraPosition");
    shaderProgram->clipStartLoc = glGetUniformLocation(shaderProgram->id, "elf_ClipStart");
    shaderProgram->clipEndLoc = glGetUniformLocation(shaderProgram->id, "elf_ClipEnd");
    shaderProgram->viewportWidthLoc = glGetUniformLocation(shaderProgram->id, "elf_ViewportWidth");
    shaderProgram->viewportHeightLoc = glGetUniformLocation(shaderProgram->id, "elf_ViewportHeight");
    shaderProgram->parallaxScaleLoc = glGetUniformLocation(shaderProgram->id, "elf_ParallaxScale");
    shaderProgram->alphaThresholdLoc = glGetUniformLocation(shaderProgram->id, "elf_AlphaThreshold");
    shaderProgram->fogStartLoc = glGetUniformLocation(shaderProgram->id, "elf_FogStart");
    shaderProgram->fogEndLoc = glGetUniformLocation(shaderProgram->id, "elf_FogEnd");
    shaderProgram->fogColorLoc = glGetUniformLocation(shaderProgram->id, "elf_FogColor");

    return shaderProgram;
}

void gfxDestroyShaderProgram(gfxShaderProgram* shaderProgram)
{
    if (shaderProgram->id)
        glDeleteShader(shaderProgram->id);

    free(shaderProgram);
}

void gfxDestroyShaderPrograms(gfxShaderProgram* shaderProgram)
{
    if (shaderProgram->next)
        gfxDestroyShaderPrograms(shaderProgram->next);

    gfxDestroyShaderProgram(shaderProgram);
}

void gfxSetShaderProgram(gfxShaderProgram* shaderProgram)
{
    if (shaderProgram != driver->shaderParams.shaderProgram)
        glUseProgram(shaderProgram->id);

    driver->shaderParams.shaderProgram = shaderProgram;

    // just inputting with values that do not make sense
    driver->shaderConfig.textures = 255;
    driver->shaderConfig.light = 255;
}

void gfxSetShaderProgramUniform1i(const char* name, int i)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniform1i(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), i);
}

void gfxSetShaderProgramUniform1f(const char* name, float f)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniform1f(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), f);
}

void gfxSetShaderProgramUniformVec2(const char* name, float x, float y)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniform2f(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), x, y);
}

void gfxSetShaderProgramUniformVec3(const char* name, float x, float y, float z)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniform3f(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), x, y, z);
}

void gfxSetShaderProgramUniformVec4(const char* name, float x, float y, float z, float w)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniform4f(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), x, y, z, w);
}

void gfxSetShaderProgramUniformMat4(const char* name, float* matrix)
{
    if (!driver->shaderParams.shaderProgram)
        return;
    glUniformMatrix4fv(glGetUniformLocation(driver->shaderParams.shaderProgram->id, name), 1, GL_FALSE, matrix);
}
