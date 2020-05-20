#pragma once

#include <lua.hpp>

// Lua bindings for directory API

int lua_ReadDirectory(lua_State* L);
int lua_GetDirectoryPath(lua_State* L);
int lua_GetDirectoryItemCount(lua_State* L);
int lua_GetDirectoryItem(lua_State* L);
int lua_GetDirectoryItemName(lua_State* L);
int lua_GetDirectoryItemType(lua_State* L);