#include "nelf/Color.h"

const elfColor colorWhite = {1.f, 1.f, 1.f, 1.f};
const elfColor colorGrey = {0.5f, 0.5f, 0.5f, 1.f};
const elfColor colorBlack = {0.f, 0.f, 0.f, 1.f};

const elfColor colorRed = {1.f, 0.f, 0.f, 1.f};
const elfColor colorGreen = {0.f, 1.f, 0.f, 1.f};
const elfColor colorBlue = {0.f, 0.f, 1.f, 1.f};

elfColor elfCreateColor(float r, float g, float b, float a)
{
    elfColor col;

    col.r = r;
    col.g = g;
    col.b = b;
    col.a = a;

    return col;
}