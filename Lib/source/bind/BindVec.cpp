#include "bind/BindVec.h"

#include <cstring>

int lua_elfVec2i__index(lua_State* L)
{
    lua_elfVec2i* ud = (lua_elfVec2i*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.x);
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.y);
    }
    return 1;
}

int lua_elfVec2i__newindex(lua_State* L)
{
    lua_elfVec2i* ud = (lua_elfVec2i*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        if (lua_isnumber(L, 3))
            ud->val.x = (int)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec2i.x");
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        if (lua_isnumber(L, 3))
            ud->val.y = (int)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec2i.y");
    }
    return 0;
}

int lua_elfVec2f__index(lua_State* L)
{
    lua_elfVec2f* ud = (lua_elfVec2f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.x);
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.y);
    }
    return 1;
}

int lua_elfVec2f__newindex(lua_State* L)
{
    lua_elfVec2f* ud = (lua_elfVec2f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        if (lua_isnumber(L, 3))
            ud->val.x = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec2f.x");
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        if (lua_isnumber(L, 3))
            ud->val.y = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec2f.y");
    }
    return 0;
}

int lua_elfVec3f__index(lua_State* L)
{
    lua_elfVec3f* ud = (lua_elfVec3f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.x);
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.y);
    }
    else if (!strcmp(lua_tostring(L, 2), "z"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.z);
    }
    return 1;
}

int lua_elfVec3f__newindex(lua_State* L)
{
    lua_elfVec3f* ud = (lua_elfVec3f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        if (lua_isnumber(L, 3))
            ud->val.x = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec3f.x");
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        if (lua_isnumber(L, 3))
            ud->val.y = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec3f.y");
    }
    else if (!strcmp(lua_tostring(L, 2), "z"))
    {
        if (lua_isnumber(L, 3))
            ud->val.z = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_vec3f.z");
    }
    return 0;
}

int lua_elfVec4f__index(lua_State* L)
{
    lua_elfVec4f* ud = (lua_elfVec4f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.x);
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.y);
    }
    else if (!strcmp(lua_tostring(L, 2), "z"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.z);
    }
    else if (!strcmp(lua_tostring(L, 2), "w"))
    {
        lua_pushnumber(L, (lua_Number)ud->val.w);
    }
    return 1;
}

int lua_elfVec4f__newindex(lua_State* L)
{
    lua_elfVec4f* ud = (lua_elfVec4f*)lua_touserdata(L, 1);
    if (!strcmp(lua_tostring(L, 2), "x"))
    {
        if (lua_isnumber(L, 3))
            ud->val.x = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_color.x");
    }
    else if (!strcmp(lua_tostring(L, 2), "y"))
    {
        if (lua_isnumber(L, 3))
            ud->val.y = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_color.y");
    }
    else if (!strcmp(lua_tostring(L, 2), "z"))
    {
        if (lua_isnumber(L, 3))
            ud->val.z = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_color.z");
    }
    else if (!strcmp(lua_tostring(L, 2), "w"))
    {
        if (lua_isnumber(L, 3))
            ud->val.w = (float)lua_tonumber(L, 3);
        else
            return lua_fail_with_backtrace(L, "Attemp to assign a non-number to elf_color.w");
    }
    return 0;
}

void lua_create_elfVec2i(lua_State* L, elfVec2i vec)
{
    lua_elfVec2i* ud;
    ud = (lua_elfVec2i*)lua_newuserdata(L, sizeof(lua_elfVec2i));
    ud->type = LUA_ELF_VEC2I;
    ud->val = vec;
    luaL_getmetatable(L, "lua_elfVec2i_mt");
    lua_setmetatable(L, -2);
}

void lua_create_elfVec2f(lua_State* L, elfVec2f vec)
{
    lua_elfVec2f* ud;
    ud = (lua_elfVec2f*)lua_newuserdata(L, sizeof(lua_elfVec2f));
    ud->type = LUA_ELF_VEC2F;
    ud->val = vec;
    luaL_getmetatable(L, "lua_elfVec2f_mt");
    lua_setmetatable(L, -2);
}

void lua_create_elfVec3f(lua_State* L, elfVec3f vec)
{
    lua_elfVec3f* ud;
    ud = (lua_elfVec3f*)lua_newuserdata(L, sizeof(lua_elfVec3f));
    ud->type = LUA_ELF_VEC3F;
    ud->val = vec;
    luaL_getmetatable(L, "lua_elfVec3f_mt");
    lua_setmetatable(L, -2);
}

void lua_create_elfVec4f(lua_State* L, elfVec4f vec)
{
    lua_elfVec4f* ud;
    ud = (lua_elfVec4f*)lua_newuserdata(L, sizeof(lua_elfVec4f));
    ud->type = LUA_ELF_VEC4F;
    ud->val = vec;
    luaL_getmetatable(L, "lua_elfVec4f_mt");
    lua_setmetatable(L, -2);
}