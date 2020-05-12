#pragma once

#include "nelf/objectHeader.h"

struct elfScript;

// Global scripting engine
bool elfInitScripting();
void elfUpdateScripting();
void elfDeinitScripting();

bool elfRunString(const char* str);
bool elfRunScript(elfScript* script);