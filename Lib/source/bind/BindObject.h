#pragma once

#include <lua.hpp>

struct elfObject;

int lua_IncRef(lua_State* L);
int lua_DecRef(lua_State* L);

int lua_GetObjectType(lua_State* L);
int lua_GetObjectRefCount(lua_State* L);

int lua_IsActor(lua_State* L);
int lua_IsGuiObject(lua_State* L);