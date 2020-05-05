#include "nelf/Random.h"

#include <cmath>

float elfRandomFloat() { return (float)rand() / (float)RAND_MAX; }

float elfRandomFloatRange(float min, float max) { return min + (max - min) * elfRandomFloat(); }

int elfRandomInt() { return rand(); }

int elfRandomIntRange(int min, int max) { return (int)(((float)(min + (max - min))) * elfRandomFloat()); }