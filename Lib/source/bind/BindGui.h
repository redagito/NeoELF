#pragma once

#include <lua.hpp>

// Lua bindings for the GUI module

// elfGui
int lua_CreateGui(lua_State* L);
int lua_AddGuiObject(lua_State* L);
int lua_GetGuiObjectByName(lua_State* L);
int lua_GetGuiObjectByIndex(lua_State* L);
int lua_RemoveGuiObjectByName(lua_State* L);
int lua_RemoveGuiObjectByIndex(lua_State* L);
int lua_RemoveGuiObjectByObject(lua_State* L);
int lua_GetGuiTrace(lua_State* L);
int lua_GetGuiFocus(lua_State* L);
int lua_GetGuiActiveTextField(lua_State* L);
int lua_GetGuiDragging(lua_State* L);
int lua_GetGuiDragObject(lua_State* L);
int lua_GetGuiDragContent(lua_State* L);
int lua_EmptyGui(lua_State* L);