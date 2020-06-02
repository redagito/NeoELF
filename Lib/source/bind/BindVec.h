#pragma once

#include "bind/BindUtil.h"
#include "nelf/Vector.h"

struct lua_elfVec2i
{
    LUA_ELF_USERDATA_HEADER;
    elfVec2i val;
};

struct lua_elfVec2f
{
    LUA_ELF_USERDATA_HEADER;
    elfVec2f val;
};

struct lua_elfVec3f
{
    LUA_ELF_USERDATA_HEADER;
    elfVec3f val;
};

struct lua_elfVec4f
{
    LUA_ELF_USERDATA_HEADER;
    elfVec4f val;
};

int lua_elfVec2i__index(lua_State* L);
int lua_elfVec2i__newindex(lua_State* L);

int lua_elfVec2f__index(lua_State* L);
int lua_elfVec2f__newindex(lua_State* L);

int lua_elfVec3f__index(lua_State* L);
int lua_elfVec3f__newindex(lua_State* L);

int lua_elfVec4f__index(lua_State* L);
int lua_elfVec4f__newindex(lua_State* L);

void lua_create_elfVec2i(lua_State* L, elfVec2i vec);
void lua_create_elfVec2f(lua_State* L, elfVec2f vec);
void lua_create_elfVec3f(lua_State* L, elfVec3f vec);
void lua_create_elfVec4f(lua_State* L, elfVec4f vec);