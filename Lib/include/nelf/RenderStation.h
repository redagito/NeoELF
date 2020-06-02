#pragma once

#include "nelf/Color.h"
#include "nelf/objectHeader.h"

struct gfxTexture;
struct gfxRenderTarget;
struct gfxVertexData;
struct gfxVertexArray;
struct gfxVertexIndex;

struct elfRenderStation
{
    ELF_OBJECT_HEADER;

    gfxTexture* shadowMap = nullptr;
    gfxRenderTarget* shadowTarget = nullptr;

    gfxVertexData* quadVertexData = nullptr;
    gfxVertexData* quadTexCoordData = nullptr;
    gfxVertexData* quadNormalData = nullptr;
    gfxVertexArray* quadVertexArray = nullptr;

    gfxVertexData* bbVertexData = nullptr;
    gfxVertexData* bbIndexData = nullptr;
    gfxVertexArray* bbVertexArray = nullptr;
    gfxVertexIndex* bbVertexIndex = nullptr;

    gfxVertexData* lines = nullptr;
    gfxVertexArray* linesVertexArray = nullptr;

    gfxVertexData* circleVertexData = nullptr;
    gfxVertexArray* circleVertexArray = nullptr;

    int prevCircleVerticeCount = 0;
    float prevCircleSize = 0.f;
    gfxVertexArray* spriteVertexArray = nullptr;

    gfxVertexData* gradientVertexData = nullptr;
    gfxVertexData* gradientColorData = nullptr;
    gfxVertexArray* gradientVertexArray = nullptr;
};

// Globally accessible render station
extern elfRenderStation* rnd;

elfRenderStation* elfCreateRenderStation();
void elfDestroyRenderStation(void* data);

bool elfInitRenderStation();
void elfDeinitRenderStation();

void elfDraw2dQuad(float x, float y, float width, float height);

void elfDrawTextured2dQuad(float x, float y, float width, float height);
void elfDrawTextured2dQuadRegion(float x, float y, float width, float height, float tx, float ty, float twidth,
                                 float theight);

// Draws circle at position x/y
void elfDrawCircle(float x, float y, int vertices, float size);
// Bounding box, minimal x/y/z, maximum x/y/z
void elfDrawBoundingBox(float min[3], float max[3]);

void elfDrawLines(int count, gfxVertexData* vertices);
void elfDrawLineLoop(int count, gfxVertexData* vertices);
void elfDrawHorGradient(int x, int y, int width, int height, elfColor col1, elfColor col2);
void elfDrawHorGradientBorder(int x, int y, int width, int height, elfColor col1, elfColor col2);