#include "bind/BindColor.h"

#include <string>

int lua_elfColor__index(lua_State* L)
{
    lua_elfColor* ud = (lua_elfColor*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "r"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.r);
    }
    else if (!strcmp(lua_tostring(L, 2), "g"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.g);
    }
    else if (!strcmp(lua_tostring(L, 2), "b"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.b);
    }
    else if (!strcmp(lua_tostring(L, 2), "a"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.a);
    }
    return 1;
}

int lua_elfColor__newindex(lua_State* L)
{
    lua_elfColor* ud = (lua_elfColor*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "r"))
    {
        if (lua_isnumber(L, 3))
            ud->val.r = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec4f.r");
    }
    else if (!strcmp(lua_tostring(L, 2), "g"))
    {
        if (lua_isnumber(L, 3))
            ud->val.g = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec4f.g");
    }
    else if (!strcmp(lua_tostring(L, 2), "b"))
    {
        if (lua_isnumber(L, 3))
            ud->val.b = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec4f.b");
    }
    else if (!strcmp(lua_tostring(L, 2), "a"))
    {
        if (lua_isnumber(L, 3))
            ud->val.a = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec4f.a");
    }
    return 0;
}

void lua_create_elfColor(lua_State* L, elfColor col)
{
    lua_elfColor* ud;
    ud = (lua_elfColor*)lua_newuserdata(L, sizeof(lua_elfColor));
    ud->type = LUA_ELF_COLOR;
    ud->val = col;
    luaL_getmetatable(L, "lua_elfColor_mt");
    lua_setmetatable(L, -2);
}