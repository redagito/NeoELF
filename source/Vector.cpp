#include "nelf/Vector.h"

#include <cstring>

elfVec2f elfCreateVec2f(float x, float y)
{
    elfVec2f result;

    result.x = x;
    result.y = y;

    return result;
}

elfVec3f elfCreateVec3f(float x, float y, float z)
{
    elfVec3f result;

    result.x = x;
    result.y = y;
    result.z = z;

    return result;
}

elfVec4f elfCreateQua(float x, float y, float z, float w)
{
    elfVec4f result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return result;
}

elfVec4f elfCreateQuaFromEuler(float x, float y, float z)
{
    elfVec4f result;
    memset(&result, 0x0, sizeof(elfVec4f));

    gfxQuaFromEuler(x, y, z, &result.x);

    return result;
}

elfVec4f elfCreateQuaFromAngleAxis(float angle, float x, float y, float z)
{
    elfVec4f qua;
    float axis[3];

    axis[0] = x;
    axis[1] = y;
    axis[2] = z;
    gfxQuaFromAngleAxis(angle, axis, &qua.x);

    return qua;
}

elfVec2f elfAddVec2fVec2f(elfVec2f vec1, elfVec2f vec2)
{
    elfVec2f result;
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    return result;
}

elfVec2f elfSubVec2fVec2f(elfVec2f vec1, elfVec2f vec2)
{
    elfVec2f result;
    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    return result;
}

elfVec2f elfMulVec2fVec2f(elfVec2f vec1, elfVec2f vec2)
{
    elfVec2f result;
    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    return result;
}

float elfGetVec2fLength(elfVec2f vec) { return (float)sqrt(vec.x * vec.x + vec.y * vec.y); }

elfVec3f elfAddVec3fVec3f(elfVec3f vec1, elfVec3f vec2)
{
    elfVec3f result;
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    result.z = vec1.z + vec2.z;
    return result;
}

elfVec3f elfSubVec3fVec3f(elfVec3f vec1, elfVec3f vec2)
{
    elfVec3f result;
    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    result.z = vec1.z - vec2.z;
    return result;
}

elfVec3f elfMulVec3fVec3f(elfVec3f vec1, elfVec3f vec2)
{
    elfVec3f result;
    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    result.z = vec1.z * vec2.z;
    return result;
}

float elfGetVec3fLength(elfVec3f vec) { return gfxVecLength(&vec.x); }

elfVec3f elfRotateVec3f(elfVec3f vec, float x, float y, float z)
{
    elfVec4f qua;
    elfVec3f result;

    gfxQuaFromEuler(x, y, z, &qua.x);

    gfxMulQuaVec(&qua.x, &vec.x, &result.x);

    return result;
}

elfVec3f elfMulQuaVec3f(elfVec4f qua, elfVec3f vec)
{
    elfVec3f result;

    gfxMulQuaVec(&qua.x, &vec.x, &result.x);

    return result;
}

elfVec4f elfMulQuaQua(elfVec4f qua1, elfVec4f qua2)
{
    elfVec4f result;

    gfxMulQuaQua(&qua1.x, &qua2.x, &result.x);

    return result;
}

elfVec4f elfGetQuaInverted(elfVec4f qua)
{
    elfVec4f result;

    gfxQuaGetInverse(&qua.x, &result.x);

    return result;
}