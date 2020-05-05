#pragma once

struct elfVec2i
{
    int x = 0;
    int y = 0;
};

struct elfVec2f
{
    float x = 0.f;
    float y = 0.f;
};

elfVec2f elfCreateVec2f(float x, float y);

elfVec2f elfAddVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
elfVec2f elfSubVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
elfVec2f elfMulVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
float elfGetVec2fLength(elfVec2f vec);

struct elfVec3f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

elfVec3f elfCreateVec3f(float x, float y, float z);

elfVec3f elfAddVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
elfVec3f elfSubVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
elfVec3f elfMulVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
float elfGetVec3fLength(elfVec3f vec);

elfVec3f elfRotateVec3f(elfVec3f vec, float x, float y, float z);

// TODO Maybe rename to Quaternion?
struct elfVec4f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;
};

elfVec4f elfCreateQua(float x, float y, float z, float w);
elfVec4f elfCreateQuaFromEuler(float x, float y, float z);
elfVec4f elfCreateQuaFromAngleAxis(float angle, float x, float y, float z);

elfVec4f elfMulQuaQua(elfVec4f qua1, elfVec4f qua2);

elfVec4f elfGetQuaInverted(elfVec4f qua);

elfVec3f elfMulQuaVec3f(elfVec4f qua, elfVec3f vec);
