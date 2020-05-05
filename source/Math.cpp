#include "nelf/Math.h"

constexpr float epsilon = 0.0001f;

bool elfAboutZero(float val) { return val < epsilon && val > -epsilon; }

bool elfAboutSame(float v1, float v2) { return elfFloatAbs(v1 - v2) < epsilon; }

float elfFloatAbs(float val)
{
    if (val < 0.0f)
        return -val;
    return val;
}

float elfFloatMin(float a, float b)
{
    if (a < b)
        return a;
    return b;
}

float elfFloatMax(float a, float b)
{
    if (a > b)
        return a;
    return b;
}