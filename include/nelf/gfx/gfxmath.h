#pragma once

// TODO Use glm instead

#define GFX_PI 3.14159265
#define GFX_PI_DIV_180 GFX_PI / 180.0
#define GFX_180_DIV_PI 180.0 / GFX_PI

// Vector

void gfxVecToEuler(float* vec, float* euler);

void gfxVecNormalize(float* vec);

void gfxVecDotVec(float* vec1, float* vec2, float* dot);

void gfxVecCrossProduct(float* vec1, float* vec2, float* vec3);

float gfxVecLength(float* vec);
float gfxVecSquaredLength(float* vec);

// Quaternion

void gfxQuaSetIdentity(float* qua);

void gfxQuaNormalize(float* qua, float* result);

void gfxQuaGetInverse(float* qua, float* invqua);

void gfxQuaFromAngleAxis(float angle, float* axis, float* qua);
void gfxQuaFromEuler(float x, float y, float z, float* qua);

void gfxQuaToMatrix4(float* qua, float* mat);
void gfxQuaToEuler(float* qua, float* euler);

void gfxRotateQua(float x, float y, float z, float* qua);
void gfxRotateQuaLocal(float x, float y, float z, float* qua);

void gfxMulQuaVec(float* qua, float* vec1, float* vec2);
void gfxMulQuaQua(float* qua1, float* qua2, float* qua3);

void gfxQuaSlerp(float* qa, float* qc, double t, float* result);

// Matrix

void gfxMatrix4SetIdentity(float* mat);
void gfxMatrix3SetIdentity(float* mat);

bool gfxMatrix4GetInverse(float* mat1, float* mat2);
bool gfxMatrix3GetInverse(float* mat1, float* mat2);

void gfxMatrix4GetTranspose(float* mat1, float* mat2);
void gfxMatrix3GetTranspose(float* mat1, float* mat2);

void gfxMatrix3ToQua(float* mat, float* qua);
void gfxMatrix4ToEuler(float* mat, float* eul);

void gfxMulMatrix4Vec3(float* m1, float* vec1, float* vec2);
void gfxMulMatrix4Vec4(float* m1, float* vec1, float* vec2);
void gfxMulMatrix4Matrix4(float* m1, float* m2, float* m3);
void gfxMulMatrix3Matrix4(float* m1, float* m2, float* m3);

// Intersection

bool gfxBoxSphereIntersect(float* bmin, float* bmax, float* spos, float srad);
bool gfxAabbInsideFrustum(float frustum[6][4], float* min, float* max);
bool gfxSphereInsideFrustum(float frustum[6][4], float* pos, float radius);