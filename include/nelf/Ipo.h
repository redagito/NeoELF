#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

// Single point in bezier curve
struct elfBezierPoint
{
    ELF_OBJECT_HEADER;
    elfVec2f c1;
    elfVec2f p;
    elfVec2f c2;
};

elfBezierPoint* elfCreateBezierPoint();

void elfDestroyBezierPoint(void* data);

// Setter
void elfSetBezierPointPosition(elfBezierPoint* point, float x, float y);
void elfSetBezierPointControl1(elfBezierPoint* point, float x, float y);
void elfSetBezierPointControl2(elfBezierPoint* point, float x, float y);

// Getter
elfVec2f elfGetBezierPointPosition(elfBezierPoint* point);
elfVec2f elfGetBezierPointControl1(elfBezierPoint* point);
elfVec2f elfGetBezierPointControl2(elfBezierPoint* point);

// Bezier curve
struct elfBezierCurve
{
    ELF_OBJECT_HEADER;
    unsigned char curveType;
    unsigned char interpolation;
    elfList* points = nullptr;
};

elfBezierCurve* elfCreateBezierCurve();

void elfDestroyBezierCurve(void* data);

void elfSetBezierCurveType(elfBezierCurve* curve, int type);

int elfGetBezierCurveType(elfBezierCurve* curve);

void elfAddBezierCurvePoint(elfBezierCurve* curve, elfBezierPoint* point);

int elfGetCurvePointCount(elfBezierCurve* curve);

elfBezierPoint* elfGetPointFromBezierCurve(elfBezierCurve* curve, int idx);

float elfGetBezierCurveValue(elfBezierCurve* curve, float x);

// Honestly no idea what Ipo stands for
struct elfIpo
{
    ELF_OBJECT_HEADER;
    elfList* curves = nullptr;
    bool loc = false;
    bool rot = false;
    bool scale = false;
    bool qua = false;
};

elfIpo* elfCreateIpo();

void elfDestroyIpo(void* data);

bool elfAddIpoCurve(elfIpo* ipo, elfBezierCurve* curve);

int elfGetIpoCurveCount(elfIpo* ipo);

elfBezierCurve* elfGetCurveFromIpo(elfIpo* ipo, int idx);

elfVec3f elfGetIpoLoc(elfIpo* ipo, float x);

elfVec3f elfGetIpoRot(elfIpo* ipo, float x);

elfVec3f elfGetIpoScale(elfIpo* ipo, float x);

elfVec4f elfGetIpoQua(elfIpo* ipo, float x);