#pragma once

#include <lua.hpp>

// Common stuff
// TODO Cleanup
#define LUA_ELF_USERDATA_HEADER unsigned char type

#define LUA_ELF_OBJECT 0x0001
#define LUA_ELF_VEC2I 0x0002
#define LUA_ELF_VEC2F 0x0002
#define LUA_ELF_VEC3F 0x0003
#define LUA_ELF_VEC4F 0x0004
#define LUA_ELF_COLOR 0x0005

struct lua_elf_userdata
{
    LUA_ELF_USERDATA_HEADER;
};

struct elfObject;

struct lua_elfObject
{
    LUA_ELF_USERDATA_HEADER;
    elfObject* object;
};

// Utility functions for Lua to C++ function binding
int lua_fail_with_backtrace(lua_State* L, const char* fmt, ...);
int lua_fail_arg_count(lua_State* L, const char* func_name, int a, int b);
int lua_fail_arg(lua_State* L, const char* func_name, int idx, const char* etype);

// TODO Implementation in Binds.cpp
void lua_create_elfObject(lua_State* L, elfObject* obj);