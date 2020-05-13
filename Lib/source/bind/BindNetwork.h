#pragma once

#include <lua.hpp>

// Lua bindings for network module

int lua_CreateRequest(lua_State* L);
int lua_SendRequest(lua_State* L);

int lua_GetRequestUrl(lua_State* L);
int lua_SetRequestUrl(lua_State* L);

int lua_GetRequestMethod(lua_State* L);
int lua_SetRequestMethod(lua_State* L);
int lua_GetRequestState(lua_State* L);