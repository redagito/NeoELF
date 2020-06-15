#pragma once

#include "bind/BindUtil.h"

int lua_CreateList(lua_State* L);
int lua_GetListLength(lua_State* L);

int lua_InsertListObject(lua_State* L);
int lua_AppendListObject(lua_State* L);

int lua_RemoveListObject(lua_State* L);
int lua_GetListObject(lua_State* L);

int lua_BeginList(lua_State* L);
int lua_GetListNext(lua_State* L);

int lua_RBeginList(lua_State* L);
int lua_GetListRNext(lua_State* L);

int lua_SeekList(lua_State* L);
int lua_RSeekList(lua_State* L);