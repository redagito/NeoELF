#include "nelf/RenderStation.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"

elfRenderStation* rnd = nullptr;

elfRenderStation* elfCreateRenderStation()
{
    elfRenderStation* rs;
    gfxVertexData* vertexData;
    unsigned int* indexBuffer;
    float* vertexBuffer;

    rs = (elfRenderStation*)malloc(sizeof(elfRenderStation));
    memset(rs, 0x0, sizeof(elfRenderStation));
    rs->objType = ELF_RENDER_STATION;
    rs->objDestr = elfDestroyRenderStation;

    rs->shadowMap = gfxCreate2dTexture(1024, 1024, 0.0f, GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT,
                                       GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
    rs->shadowTarget = gfxCreateRenderTarget(1024, 1024);

    gfxIncRef((gfxObject*)rs->shadowMap);
    gfxIncRef((gfxObject*)rs->shadowTarget);

    gfxSetRenderTargetDepthTexture(rs->shadowTarget, rs->shadowMap);

    // quad
    rs->quadVertexData = gfxCreateVertexData(12, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->quadTexCoordData = gfxCreateVertexData(12, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->quadNormalData = gfxCreateVertexData(12, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->quadVertexArray = gfxCreateVertexArray(GFX_FALSE);

    gfxIncRef((gfxObject*)rs->quadVertexData);
    gfxIncRef((gfxObject*)rs->quadNormalData);
    gfxIncRef((gfxObject*)rs->quadTexCoordData);
    gfxIncRef((gfxObject*)rs->quadVertexArray);

    gfxSetVertexArrayData(rs->quadVertexArray, GFX_VERTEX, rs->quadVertexData);
    gfxSetVertexArrayData(rs->quadVertexArray, GFX_NORMAL, rs->quadNormalData);
    gfxSetVertexArrayData(rs->quadVertexArray, GFX_TEX_COORD, rs->quadTexCoordData);

    // bounding box
    rs->bbVertexData = gfxCreateVertexData(24, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->bbIndexData = gfxCreateVertexData(36, GFX_UINT, GFX_VERTEX_DATA_STATIC);
    rs->bbVertexArray = gfxCreateVertexArray(GFX_FALSE);
    rs->bbVertexIndex = gfxCreateVertexIndex(GFX_FALSE, rs->bbIndexData);

    gfxIncRef((gfxObject*)rs->bbVertexData);
    gfxIncRef((gfxObject*)rs->bbIndexData);
    gfxIncRef((gfxObject*)rs->bbVertexArray);
    gfxIncRef((gfxObject*)rs->bbVertexIndex);

    gfxSetVertexArrayData(rs->bbVertexArray, GFX_VERTEX, rs->bbVertexData);

    indexBuffer = (unsigned int*)gfxGetVertexDataBuffer(rs->bbIndexData);

    indexBuffer[0] = 0;
    indexBuffer[1] = 1;
    indexBuffer[2] = 2;
    indexBuffer[3] = 2;
    indexBuffer[4] = 1;
    indexBuffer[5] = 3;
    indexBuffer[6] = 2;
    indexBuffer[7] = 3;
    indexBuffer[8] = 4;
    indexBuffer[9] = 4;
    indexBuffer[10] = 3;
    indexBuffer[11] = 5;
    indexBuffer[12] = 4;
    indexBuffer[13] = 5;
    indexBuffer[14] = 6;
    indexBuffer[15] = 6;
    indexBuffer[16] = 5;
    indexBuffer[17] = 7;
    indexBuffer[18] = 6;
    indexBuffer[19] = 7;
    indexBuffer[20] = 0;
    indexBuffer[21] = 0;
    indexBuffer[22] = 7;
    indexBuffer[23] = 1;
    indexBuffer[24] = 0;
    indexBuffer[25] = 2;
    indexBuffer[26] = 6;
    indexBuffer[27] = 6;
    indexBuffer[28] = 2;
    indexBuffer[29] = 4;
    indexBuffer[30] = 3;
    indexBuffer[31] = 1;
    indexBuffer[32] = 5;
    indexBuffer[33] = 5;
    indexBuffer[34] = 1;
    indexBuffer[35] = 7;

    // lines
    rs->linesVertexArray = gfxCreateVertexArray(GFX_FALSE);
    gfxIncRef((gfxObject*)rs->linesVertexArray);

    rs->lines = gfxCreateVertexData(512, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    gfxIncRef((gfxObject*)rs->lines);

    // circle
    rs->circleVertexData = gfxCreateVertexData((GFX_MAX_CIRCLE_VERTICES + 2) * 3, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->circleVertexArray = gfxCreateVertexArray(GFX_FALSE);

    gfxIncRef((gfxObject*)rs->circleVertexData);
    gfxIncRef((gfxObject*)rs->circleVertexArray);

    gfxSetVertexArrayData(rs->circleVertexArray, GFX_VERTEX, rs->circleVertexData);

    rs->prevCircleVerticeCount = 0;

    // sprite
    rs->spriteVertexArray = gfxCreateVertexArray(GFX_FALSE);
    gfxIncRef((gfxObject*)rs->spriteVertexArray);

    vertexData = gfxCreateVertexData(36, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(vertexData);

    vertexBuffer[0] = -0.5f;
    vertexBuffer[1] = 0.5f;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = -0.5f;
    vertexBuffer[4] = -0.5f;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = 0.5f;
    vertexBuffer[7] = -0.5f;
    vertexBuffer[8] = 0.0f;

    vertexBuffer[9] = -0.5f;
    vertexBuffer[10] = 0.5f;
    vertexBuffer[11] = 0.0f;
    vertexBuffer[12] = 0.5f;
    vertexBuffer[13] = -0.5f;
    vertexBuffer[14] = 0.0f;
    vertexBuffer[15] = 0.5f;
    vertexBuffer[16] = 0.5f;
    vertexBuffer[17] = 0.0f;

    vertexBuffer[18] = 0.5f;
    vertexBuffer[19] = 0.5f;
    vertexBuffer[20] = 0.0f;
    vertexBuffer[21] = 0.5f;
    vertexBuffer[22] = -0.5f;
    vertexBuffer[23] = 0.0f;
    vertexBuffer[24] = -0.5f;
    vertexBuffer[25] = -0.5f;
    vertexBuffer[26] = 0.0f;

    vertexBuffer[27] = 0.5f;
    vertexBuffer[28] = 0.5f;
    vertexBuffer[29] = 0.0f;
    vertexBuffer[30] = -0.5f;
    vertexBuffer[31] = -0.5f;
    vertexBuffer[32] = 0.0f;
    vertexBuffer[33] = -0.5f;
    vertexBuffer[34] = 0.5f;
    vertexBuffer[35] = 0.0f;

    gfxSetVertexArrayData(rs->spriteVertexArray, GFX_VERTEX, vertexData);

    vertexData = gfxCreateVertexData(24, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(vertexData);

    vertexBuffer[0] = 0.0f;
    vertexBuffer[1] = 1.0f;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = 0.0f;
    vertexBuffer[4] = 1.0f;
    vertexBuffer[5] = 0.0f;

    vertexBuffer[6] = 0.0f;
    vertexBuffer[7] = 1.0f;
    vertexBuffer[8] = 1.0f;
    vertexBuffer[9] = 0.0f;
    vertexBuffer[10] = 1.0f;
    vertexBuffer[11] = 1.0f;

    vertexBuffer[12] = 0.0f;
    vertexBuffer[13] = 1.0f;
    vertexBuffer[14] = 0.0f;
    vertexBuffer[15] = 0.0f;
    vertexBuffer[16] = 1.0f;
    vertexBuffer[17] = 0.0f;

    vertexBuffer[18] = 0.0f;
    vertexBuffer[19] = 1.0f;
    vertexBuffer[20] = 1.0f;
    vertexBuffer[21] = 0.0f;
    vertexBuffer[22] = 1.0f;
    vertexBuffer[23] = 1.0f;

    gfxSetVertexArrayData(rs->spriteVertexArray, GFX_TEX_COORD, vertexData);

    vertexData = gfxCreateVertexData(36, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);

    vertexBuffer = (float*)gfxGetVertexDataBuffer(vertexData);

    vertexBuffer[0] = 0.0f;
    vertexBuffer[1] = 0.0f;
    vertexBuffer[2] = 1.0f;
    vertexBuffer[3] = 0.0f;
    vertexBuffer[4] = 0.0f;
    vertexBuffer[5] = 1.0f;
    vertexBuffer[6] = 0.0f;
    vertexBuffer[7] = 0.0f;
    vertexBuffer[8] = 1.0f;

    vertexBuffer[9] = 0.0f;
    vertexBuffer[10] = 0.0f;
    vertexBuffer[11] = 1.0f;
    vertexBuffer[12] = 0.0f;
    vertexBuffer[13] = 0.0f;
    vertexBuffer[14] = 1.0f;
    vertexBuffer[15] = 0.0f;
    vertexBuffer[16] = 0.0f;
    vertexBuffer[17] = 1.0f;

    vertexBuffer[18] = 0.0f;
    vertexBuffer[19] = 0.0f;
    vertexBuffer[20] = -1.0f;
    vertexBuffer[21] = 0.0f;
    vertexBuffer[22] = 0.0f;
    vertexBuffer[23] = -1.0f;
    vertexBuffer[24] = 0.0f;
    vertexBuffer[25] = 0.0f;
    vertexBuffer[26] = -1.0f;

    vertexBuffer[27] = 0.0f;
    vertexBuffer[28] = 0.0f;
    vertexBuffer[29] = -1.0f;
    vertexBuffer[30] = 0.0f;
    vertexBuffer[31] = 0.0f;
    vertexBuffer[32] = -1.0f;
    vertexBuffer[33] = 0.0f;
    vertexBuffer[34] = 0.0f;
    vertexBuffer[35] = -1.0f;

    gfxSetVertexArrayData(rs->spriteVertexArray, GFX_NORMAL, vertexData);

    // gradient
    rs->gradientVertexData = gfxCreateVertexData(18, GFX_INT, GFX_VERTEX_DATA_DYNAMIC);
    rs->gradientColorData = gfxCreateVertexData(24, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    rs->gradientVertexArray = gfxCreateVertexArray(GFX_FALSE);

    gfxSetVertexArrayData(rs->gradientVertexArray, GFX_VERTEX, rs->gradientVertexData);
    gfxSetVertexArrayData(rs->gradientVertexArray, GFX_COLOR, rs->gradientColorData);

    gfxIncRef((gfxObject*)rs->gradientVertexData);
    gfxIncRef((gfxObject*)rs->gradientColorData);
    gfxIncRef((gfxObject*)rs->gradientVertexArray);

    elfIncObj(ELF_RENDER_STATION);

    return rs;
}

void elfDestroyRenderStation(void* data)
{
    elfRenderStation* rs = (elfRenderStation*)data;

    gfxDecRef((gfxObject*)rs->shadowMap);
    gfxDecRef((gfxObject*)rs->shadowTarget);

    gfxDecRef((gfxObject*)rs->quadVertexArray);
    gfxDecRef((gfxObject*)rs->quadVertexData);
    gfxDecRef((gfxObject*)rs->quadNormalData);
    gfxDecRef((gfxObject*)rs->quadTexCoordData);

    gfxDecRef((gfxObject*)rs->bbVertexData);
    gfxDecRef((gfxObject*)rs->bbIndexData);
    gfxDecRef((gfxObject*)rs->bbVertexArray);
    gfxDecRef((gfxObject*)rs->bbVertexIndex);

    gfxDecRef((gfxObject*)rs->linesVertexArray);

    gfxDecRef((gfxObject*)rs->circleVertexData);
    gfxDecRef((gfxObject*)rs->circleVertexArray);

    gfxDecRef((gfxObject*)rs->lines);
    gfxDecRef((gfxObject*)rs->spriteVertexArray);

    gfxDecRef((gfxObject*)rs->gradientVertexData);
    gfxDecRef((gfxObject*)rs->gradientColorData);
    gfxDecRef((gfxObject*)rs->gradientVertexArray);

    elfDecObj(ELF_RENDER_STATION);

    free(rs);
}

bool elfInitRenderStation()
{
    if (rnd)
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: can't initialize the render station twice!\n");
        return false;
    }

    rnd = elfCreateRenderStation();
    elfIncRef((elfObject*)rnd);

    return true;
}

void elfDeinitRenderStation()
{
    if (!rnd)
        return;

    elfDecRef((elfObject*)rnd);
    rnd = NULL;
}

void elfDraw2dQuad(float x, float y, float width, float height)
{
    float* vertexBuffer;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->quadVertexData);

    vertexBuffer[0] = x;
    vertexBuffer[1] = y + height;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = x;
    vertexBuffer[4] = y;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = x + width;
    vertexBuffer[7] = y + height;
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = x + width;
    vertexBuffer[10] = y;
    vertexBuffer[11] = 0.0f;

    gfxUpdateVertexData(rnd->quadVertexData);
    gfxDrawVertexArray(rnd->quadVertexArray, 4, GFX_TRIANGLE_STRIP);
}

void elfDrawTextured2dQuad(float x, float y, float width, float height)
{
    float* vertexBuffer;
    float* texCoordBuffer;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->quadVertexData);
    texCoordBuffer = (float*)gfxGetVertexDataBuffer(rnd->quadTexCoordData);

    vertexBuffer[0] = x;
    vertexBuffer[1] = y + height;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = x;
    vertexBuffer[4] = y;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = x + width;
    vertexBuffer[7] = y + height;
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = x + width;
    vertexBuffer[10] = y;
    vertexBuffer[11] = 0.0f;

    texCoordBuffer[0] = 0.0f;
    texCoordBuffer[1] = 1.0f;
    texCoordBuffer[2] = 0.0f;
    texCoordBuffer[3] = 0.0f;
    texCoordBuffer[4] = 1.0f;
    texCoordBuffer[5] = 1.0f;
    texCoordBuffer[6] = 1.0f;
    texCoordBuffer[7] = 0.0f;

    gfxUpdateVertexData(rnd->quadVertexData);
    gfxUpdateVertexData(rnd->quadTexCoordData);
    gfxDrawVertexArray(rnd->quadVertexArray, 4, GFX_TRIANGLE_STRIP);
}

void elfDrawTextured2dQuadRegion(float x, float y, float width, float height, float tx, float ty, float twidth,
                                 float theight)
{
    float* vertexBuffer;
    float* texCoordBuffer;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->quadVertexData);
    texCoordBuffer = (float*)gfxGetVertexDataBuffer(rnd->quadTexCoordData);

    vertexBuffer[0] = x;
    vertexBuffer[1] = y + height;
    vertexBuffer[2] = 0.0f;
    vertexBuffer[3] = x;
    vertexBuffer[4] = y;
    vertexBuffer[5] = 0.0f;
    vertexBuffer[6] = x + width;
    vertexBuffer[7] = y + height;
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = x + width;
    vertexBuffer[10] = y;
    vertexBuffer[11] = 0.0f;

    texCoordBuffer[0] = tx;
    texCoordBuffer[1] = ty + theight;
    texCoordBuffer[2] = tx;
    texCoordBuffer[3] = ty;
    texCoordBuffer[4] = tx + twidth;
    texCoordBuffer[5] = ty + theight;
    texCoordBuffer[6] = tx + twidth;
    texCoordBuffer[7] = ty;

    gfxUpdateVertexData(rnd->quadVertexData);
    gfxUpdateVertexData(rnd->quadTexCoordData);
    gfxDrawVertexArray(rnd->quadVertexArray, 4, GFX_TRIANGLE_STRIP);
}

void elfDrawCircle(float x, float y, int vertices, float size)
{
    float step;
    int i;
    float* vertexBuffer;

    if (vertices < 3)
        return;
    if (vertices > GFX_MAX_CIRCLE_VERTICES)
        vertices = GFX_MAX_CIRCLE_VERTICES;

    if (vertices != rnd->prevCircleVerticeCount || !elfAboutSame(size, rnd->prevCircleSize))
    {
        step = (360.0f / ((float)vertices)) * GFX_PI_DIV_180;

        vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->circleVertexData);

        vertexBuffer[0] = x;
        vertexBuffer[1] = y;
        vertexBuffer[2] = 0.0f;

        for (i = 1; i < vertices + 2; i++)
        {
            vertexBuffer[i * 3] = -((float)sin((float)(step * (i - 1)))) * size + x;
            vertexBuffer[i * 3 + 1] = ((float)cos((float)(step * (i - 1)))) * size + y;
            vertexBuffer[i * 3 + 2] = 0.0f;
        }
    }

    gfxUpdateVertexData(rnd->circleVertexData);
    gfxDrawVertexArray(rnd->circleVertexArray, vertices + 2, GFX_TRIANGLE_FAN);

    rnd->prevCircleVerticeCount = vertices;
}

void elfDrawBoundingBox(float min[3], float max[3])
{
    float* vertexBuffer;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->bbVertexData);

    vertexBuffer[0] = min[0];
    vertexBuffer[1] = max[1];
    vertexBuffer[2] = max[2];
    vertexBuffer[3] = min[0];
    vertexBuffer[4] = max[1];
    vertexBuffer[5] = min[2];
    vertexBuffer[6] = min[0];
    vertexBuffer[7] = min[1];
    vertexBuffer[8] = max[2];
    vertexBuffer[9] = min[0];
    vertexBuffer[10] = min[1];
    vertexBuffer[11] = min[2];
    vertexBuffer[12] = max[0];
    vertexBuffer[13] = min[1];
    vertexBuffer[14] = max[2];
    vertexBuffer[15] = max[0];
    vertexBuffer[16] = min[1];
    vertexBuffer[17] = min[2];
    vertexBuffer[18] = max[0];
    vertexBuffer[19] = max[1];
    vertexBuffer[20] = max[2];
    vertexBuffer[21] = max[0];
    vertexBuffer[22] = max[1];
    vertexBuffer[23] = min[2];

    gfxUpdateVertexData(rnd->bbVertexData);
    gfxSetVertexArray(rnd->bbVertexArray);
    gfxDrawVertexIndex(rnd->bbVertexIndex, GFX_TRIANGLES);
}

void elfDrawLines(int count, gfxVertexData* vertices)
{
    if (count < 2)
        return;
    if (count > gfxGetVertexDataCount(vertices) / 3)
        count -= count - (gfxGetVertexDataCount(vertices) / 3);

    gfxSetVertexArrayData(rnd->linesVertexArray, GFX_VERTEX, vertices);
    gfxDrawVertexArray(rnd->linesVertexArray, count, GFX_LINES);
}

void elfDrawLineLoop(int count, gfxVertexData* vertices)
{
    if (count < 2)
        return;
    if (count > gfxGetVertexDataCount(vertices) / 3)
        count -= count - (gfxGetVertexDataCount(vertices) / 3);

    gfxSetVertexArrayData(rnd->linesVertexArray, GFX_VERTEX, vertices);
    gfxDrawVertexArray(rnd->linesVertexArray, count, GFX_LINE_LOOP);
}

void elfDrawHorGradient(int x, int y, int width, int height, elfColor col1, elfColor col2)
{
    int* vertexBuffer;
    float* colorBuffer;

    vertexBuffer = (int*)gfxGetVertexDataBuffer(rnd->gradientVertexData);

    vertexBuffer[0] = x;
    vertexBuffer[1] = y + height;
    vertexBuffer[2] = 0;
    vertexBuffer[3] = x;
    vertexBuffer[4] = y;
    vertexBuffer[5] = 0;
    vertexBuffer[6] = x + width;
    vertexBuffer[7] = y + height;
    vertexBuffer[8] = 0;
    vertexBuffer[9] = x + width;
    vertexBuffer[10] = y;
    vertexBuffer[11] = 0;

    colorBuffer = (float*)gfxGetVertexDataBuffer(rnd->gradientColorData);

    colorBuffer[0] = col1.r;
    colorBuffer[1] = col1.g;
    colorBuffer[2] = col1.b;
    colorBuffer[3] = col1.a;
    colorBuffer[4] = col2.r;
    colorBuffer[5] = col2.g;
    colorBuffer[6] = col2.b;
    colorBuffer[7] = col2.a;
    colorBuffer[8] = col1.r;
    colorBuffer[9] = col1.g;
    colorBuffer[10] = col1.b;
    colorBuffer[11] = col1.a;
    colorBuffer[12] = col2.r;
    colorBuffer[13] = col2.g;
    colorBuffer[14] = col2.b;
    colorBuffer[15] = col2.a;

    gfxDrawVertexArray(rnd->gradientVertexArray, 4, GFX_TRIANGLE_STRIP);
}

void elfDrawHorGradientBorder(int x, int y, int width, int height, elfColor col1, elfColor col2)
{
    int* vertexBuffer;
    float* colorBuffer;

    vertexBuffer = (int*)gfxGetVertexDataBuffer(rnd->gradientVertexData);

    vertexBuffer[0] = x;
    vertexBuffer[1] = y + height;
    vertexBuffer[2] = 0;
    vertexBuffer[3] = x;
    vertexBuffer[4] = y;
    vertexBuffer[5] = 0;
    vertexBuffer[6] = x + width;
    vertexBuffer[7] = y + 1;
    vertexBuffer[8] = 0;
    vertexBuffer[9] = x + width;
    vertexBuffer[10] = y + height;
    vertexBuffer[11] = 0;

    colorBuffer = (float*)gfxGetVertexDataBuffer(rnd->gradientColorData);

    colorBuffer[0] = col1.r;
    colorBuffer[1] = col1.g;
    colorBuffer[2] = col1.b;
    colorBuffer[3] = col1.a;
    colorBuffer[4] = col2.r;
    colorBuffer[5] = col2.g;
    colorBuffer[6] = col2.b;
    colorBuffer[7] = col2.a;
    colorBuffer[8] = col2.r;
    colorBuffer[9] = col2.g;
    colorBuffer[10] = col2.b;
    colorBuffer[11] = col2.a;
    colorBuffer[12] = col1.r;
    colorBuffer[13] = col1.g;
    colorBuffer[14] = col1.b;
    colorBuffer[15] = col1.a;

    gfxDrawVertexArray(rnd->gradientVertexArray, 4, GFX_LINE_LOOP);
}
