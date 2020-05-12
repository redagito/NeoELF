#pragma once

struct lua_State;

// Lua script to C++ function binding
// Initializes all bindings
int luaopen_elf(lua_State* L);