#pragma once

struct elfColor
{
    // Default to hot pink color
    float r = 1.f;
    float g = 0.41f;
    float b = 0.71f;
    float a = 1.f;
};

// Common colors
extern const elfColor colorWhite;
extern const elfColor colorGrey;
extern const elfColor colorBlack;

extern const elfColor colorRed;
extern const elfColor colorGreen;
extern const elfColor colorBlue;

// Less common colors
// Use this as default to represent missing settings
extern const elfColor colorHotPink;

elfColor elfCreateColor(float r, float g, float b, float a);