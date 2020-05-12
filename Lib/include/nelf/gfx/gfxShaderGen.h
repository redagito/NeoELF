#pragma once

// Shader program generation
// This will probably be hell to pick apart

struct gfxShaderParams;
struct gfxShaderConfig;
struct gfxShaderProgram;

char* gfxCreateString(const char* str);
void gfxDestroyString(char* str);

// Line

// Linked list of lines
struct gfxLine
{
    gfxLine* next = nullptr;
    char* text = nullptr;
};

gfxLine* gfxCreateLine();
void gfxDestroyLine(gfxLine* line);
void gfxDestroyLines(gfxLine* line);

// Document

struct gfxDocument
{
    unsigned int numLines = 0;
    unsigned int numChars = 0;
    gfxLine* lines = nullptr;
};

gfxDocument* gfxCreateDocument();
void gfxDestroyDocument(gfxDocument* document);

void gfxClearDocument(gfxDocument* document);

void gfxAddDocumentLine(gfxDocument* document, const char* str);

unsigned int gfxGetDocumentLines(gfxDocument* document);
unsigned int gfxGetDocumentChars(gfxDocument* document);

// Write to buffer?
void gfxDocumentToBuffer(gfxDocument* document, char* buf);

void gfxGetShaderProgramConfig(gfxShaderParams* shaderParams, gfxShaderConfig* shaderConfig);

// Add general
// For building general shader programs

// Add vertex
void gfxAddVertexAttributes(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexVaryings(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexInit(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexTextureCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexLightingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddVertexEnd(gfxDocument* document, gfxShaderConfig* config);

// Add fragment
void gfxAddFragmentTextureUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentFogUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentMaterialUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentLightingUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentVaryings(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentInit(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentShadowCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentParallaxMappingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentPreLightingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentLightingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentTextureCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentPostLightingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddFragmentEnd(gfxDocument* document, gfxShaderConfig* config);

gfxShaderProgram* gfxGetShaderProgram(gfxShaderConfig* config);

// Add GBuffer
// For building shader programs that deal with the gbuffer

// Add vertex
void gfxAddGbufVertexAttributes(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexVaryings(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexInit(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexTextureCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexLightingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufVertexEnd(gfxDocument* document, gfxShaderConfig* config);

// Add fragment
void gfxAddGbufFragmentTextureUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufFragmentMaterialUniforms(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufFragmentVaryings(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufFragmentInit(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufFragmentParallaxMappingCalcs(gfxDocument* document, gfxShaderConfig* config);
void gfxAddGbufFragmentEnd(gfxDocument* document, gfxShaderConfig* config);

gfxShaderProgram* gfxGetGbufShaderProgram(gfxShaderConfig* config);