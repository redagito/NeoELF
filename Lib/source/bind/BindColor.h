#pragma once

#include "bind/BindUtil.h"
#include "nelf/Color.h"

struct lua_elfColor
{
    LUA_ELF_USERDATA_HEADER;
    elfColor val;
};

int lua_elfColor__index(lua_State* L);
int lua_elfColor__newindex(lua_State* L);

void lua_create_elfColor(lua_State* L, elfColor col);