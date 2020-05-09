#pragma once

#include <lua.hpp>

#include "nelf/objectHeader.h"

struct elfScript;

struct elfScripting
{
    ELF_OBJECT_HEADER;
    lua_State* L = nullptr;
};

// Global script engine
extern elfScripting* scr;

elfScripting* elfCreateScripting();
void elfDestroyScripting(void* data);

bool elfInitScripting();
void elfUpdateScripting();
void elfDeinitScripting();

bool elfRunString(const char* str);
bool elfRunScript(elfScript* script);