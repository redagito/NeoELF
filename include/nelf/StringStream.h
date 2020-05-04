#pragma once

// String parsing

char* elfReadSstNext(const char* text, int* pos, const char* sep);

char* elfReadSstText(const char* text, int* pos);

char* elfReadSstString(const char* text, int* pos);

float elfReadSstFloat(const char* text, int* pos);

void elfReadSstFloats(const char* text, int* pos, int n, float* params);

int elfReadSstInt(const char* text, int* pos);

void elfReadSstInts(const char* text, int* pos, int n, int* params);

bool elfReadSstBool(const char* text, int* pos);