#pragma once

struct elfColor
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};

// Common colors
extern const elfColor colorWhite;
extern const elfColor colorGrey;
extern const elfColor colorBlack;

extern const elfColor colorRed;
extern const elfColor colorGreen;
extern const elfColor colorBlue;

elfColor elfCreateColor(float r, float g, float b, float a);