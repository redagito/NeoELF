#include "nelf/Gui.h"

void elfSetOrtho(int x, int y, int width, int height, gfxShaderParams* shaderParams)
{
    gfxSetViewport(x, y, width, height);
    gfxGetOrthographicProjectionMatrix((float)x, (float)x + width, (float)y, (float)y + height, -1.0f, 1.0f,
                                       shaderParams->projectionMatrix);
}
