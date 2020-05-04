#include "nelf/Ipo.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Object.h"
#include "nelf/bezierCurveType.h"
#include "nelf/objectType.h"

elfBezierPoint* elfCreateBezierPoint()
{
    elfBezierPoint* point;

    point = (elfBezierPoint*)malloc(sizeof(elfBezierPoint));
    memset(point, 0x0, sizeof(elfBezierPoint));
    point->objType = ELF_BEZIER_POINT;
    point->objDestr = elfDestroyBezierPoint;

    elfIncObj(ELF_BEZIER_POINT);

    return point;
}

void elfDestroyBezierPoint(void* data)
{
    elfBezierPoint* point = (elfBezierPoint*)data;

    free(point);

    elfDecObj(ELF_BEZIER_POINT);
}

void elfSetBezierPointPosition(elfBezierPoint* point, float x, float y)
{
    point->p.x = x;
    point->p.y = y;
}

void elfSetBezierPointControl1(elfBezierPoint* point, float x, float y)
{
    point->c1.x = x;
    point->c1.y = y;
}

void elfSetBezierPointControl2(elfBezierPoint* point, float x, float y)
{
    point->c2.x = x;
    point->c2.y = y;
}

elfVec2f elfGetBezierPointPosition(elfBezierPoint* point) { return point->p; }

elfVec2f elfGetBezierPointControl1(elfBezierPoint* point) { return point->c1; }

elfVec2f elfGetBezierPointControl2(elfBezierPoint* point) { return point->c2; }

elfBezierCurve* elfCreateBezierCurve()
{
    elfBezierCurve* curve;

    curve = (elfBezierCurve*)malloc(sizeof(elfBezierCurve));
    memset(curve, 0x0, sizeof(elfBezierCurve));
    curve->objType = ELF_BEZIER_CURVE;
    curve->objDestr = elfDestroyBezierCurve;

    curve->points = elfCreateList();
    elfIncRef((elfObject*)curve->points);

    elfIncObj(ELF_BEZIER_CURVE);

    return curve;
}

void elfDestroyBezierCurve(void* data)
{
    elfBezierCurve* curve = (elfBezierCurve*)data;

    elfDecRef((elfObject*)curve->points);

    free(curve);

    elfDecObj(ELF_BEZIER_CURVE);
}

void elfSetBezierCurveType(elfBezierCurve* curve, int type)
{
    if (type >= ELF_LOC_X && type <= ELF_QUA_W)
    {
        curve->curveType = type;
    }
}

int elfGetBezierCurveType(elfBezierCurve* curve) { return curve->curveType; }

void elfAddBezierCurvePoint(elfBezierCurve* curve, elfBezierPoint* point)
{
    int i;
    elfBezierPoint* pnt;

    for (i = 0, pnt = (elfBezierPoint*)elfBeginList(curve->points); pnt;
         pnt = (elfBezierPoint*)elfGetListNext(curve->points), i++)
    {
        if (pnt->p.x > point->p.x)
        {
            elfInsertListObject(curve->points, i, (elfObject*)point);
            return;
        }
    }

    elfAppendListObject(curve->points, (elfObject*)point);
}

int elfGetCurvePointCount(elfBezierCurve* curve) { return elfGetListLength(curve->points); }

elfBezierPoint* elfGetPointFromBezierCurve(elfBezierCurve* curve, int idx)
{
    return (elfBezierPoint*)elfGetListObject(curve->points, idx);
}

float elfGetBezierCurveValue(elfBezierCurve* curve, float x)
{
    elfBezierPoint* pnt;
    elfBezierPoint* point1 = NULL;
    elfBezierPoint* point2 = NULL;
    float t;

    for (pnt = (elfBezierPoint*)elfBeginList(curve->points); pnt; pnt = (elfBezierPoint*)elfGetListNext(curve->points))
    {
        if (pnt->p.x > x)
        {
            point2 = pnt;
            break;
        }
        point1 = pnt;
    }

    if (!point1 && !point2)
        return 0.0f;
    if (!point2)
        return point1->p.y;

    t = (x - point1->p.x) / (point2->p.x - point1->p.x);
    return point1->p.y + (point2->p.y - point1->p.y) * t;
}

elfIpo* elfCreateIpo()
{
    elfIpo* ipo;

    ipo = (elfIpo*)malloc(sizeof(elfIpo));
    memset(ipo, 0x0, sizeof(elfIpo));
    ipo->objType = ELF_IPO;
    ipo->objDestr = elfDestroyIpo;

    ipo->curves = elfCreateList();
    elfIncRef((elfObject*)ipo->curves);

    elfIncObj(ELF_IPO);

    return ipo;
}

void elfDestroyIpo(void* data)
{
    elfIpo* ipo = (elfIpo*)data;

    elfDecRef((elfObject*)ipo->curves);

    free(ipo);

    elfDecObj(ELF_IPO);
}

bool elfAddIpoCurve(elfIpo* ipo, elfBezierCurve* curve)
{
    elfBezierCurve* cur;

    for (cur = (elfBezierCurve*)elfBeginList(ipo->curves); cur; cur = (elfBezierCurve*)elfGetListNext(ipo->curves))
    {
        if (cur->curveType == curve->curveType)
            return false;
    }

    elfAppendListObject(ipo->curves, (elfObject*)curve);

    if (curve->curveType <= ELF_LOC_Z)
        ipo->loc = true;
    if (curve->curveType <= ELF_ROT_Z)
        ipo->rot = true;
    if (curve->curveType <= ELF_SCALE_Z)
        ipo->scale = true;
    if (curve->curveType <= ELF_QUA_W)
        ipo->qua = true;

    return true;
}

int elfGetIpoCurveCount(elfIpo* ipo) { return elfGetListLength(ipo->curves); }

elfBezierCurve* elfGetCurveFromIpo(elfIpo* ipo, int idx) { return (elfBezierCurve*)elfGetListObject(ipo->curves, idx); }

elfVec3f elfGetIpoLoc(elfIpo* ipo, float x)
{
    elfBezierCurve* curve;
    elfVec3f result;

    memset(&result, 0x0, sizeof(elfVec3f));

    for (curve = (elfBezierCurve*)elfBeginList(ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(ipo->curves))
    {
        if (curve->curveType == ELF_LOC_X)
            result.x = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_LOC_Y)
            result.y = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_LOC_Z)
            result.z = elfGetBezierCurveValue(curve, x);
    }

    return result;
}

elfVec3f elfGetIpoRot(elfIpo* ipo, float x)
{
    elfBezierCurve* curve;
    elfVec3f result;

    memset(&result, 0x0, sizeof(elfVec3f));

    for (curve = (elfBezierCurve*)elfBeginList(ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(ipo->curves))
    {
        if (curve->curveType == ELF_ROT_X)
            result.x = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_ROT_Y)
            result.y = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_ROT_Z)
            result.z = elfGetBezierCurveValue(curve, x);
    }

    return result;
}

elfVec3f elfGetIpoScale(elfIpo* ipo, float x)
{
    elfBezierCurve* curve;
    elfVec3f result;

    memset(&result, 0x0, sizeof(elfVec3f));

    for (curve = (elfBezierCurve*)elfBeginList(ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(ipo->curves))
    {
        if (curve->curveType == ELF_SCALE_X)
            result.x = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_SCALE_Y)
            result.y = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_SCALE_Z)
            result.z = elfGetBezierCurveValue(curve, x);
    }

    return result;
}

elfVec4f elfGetIpoQua(elfIpo* ipo, float x)
{
    elfBezierCurve* curve;
    elfVec4f result;

    memset(&result, 0x0, sizeof(elfVec4f));

    for (curve = (elfBezierCurve*)elfBeginList(ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(ipo->curves))
    {
        if (curve->curveType == ELF_QUA_X)
            result.x = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_QUA_Y)
            result.y = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_QUA_Z)
            result.z = elfGetBezierCurveValue(curve, x);
        else if (curve->curveType == ELF_QUA_W)
            result.w = elfGetBezierCurveValue(curve, x);
    }

    return result;
}