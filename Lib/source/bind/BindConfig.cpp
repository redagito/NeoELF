#include "bind/BindConfig.h"

#include "bind/BindVec.h"
#include "nelf/Config.h"
#include "nelf/Object.h"

int lua_ReadConfig(lua_State* L)
{
    elfConfig* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ReadConfig", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "ReadConfig", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfReadConfig(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_SetConfigWindowSize(lua_State* L)
{
    elfConfig* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetConfigWindowSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigWindowSize", 1, "elfConfig");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetConfigWindowSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetConfigWindowSize", 3, "number");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetConfigWindowSize(arg0, arg1, arg2);
    return 0;
}

int lua_SetConfigMultisamples(lua_State* L)
{
    elfConfig* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigMultisamples", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigMultisamples", 1, "elfConfig");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetConfigMultisamples", 2, "number");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetConfigMultisamples(arg0, arg1);
    return 0;
}

int lua_SetConfigFullscreen(lua_State* L)
{
    elfConfig* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigFullscreen", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigFullscreen", 1, "elfConfig");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetConfigFullscreen", 2, "boolean");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetConfigFullscreen(arg0, arg1);
    return 0;
}

int lua_SetConfigTextureCompress(lua_State* L)
{
    elfConfig* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigTextureCompress", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigTextureCompress", 1, "elfConfig");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetConfigTextureCompress", 2, "boolean");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetConfigTextureCompress(arg0, arg1);
    return 0;
}

int lua_SetConfigTextureAnisotropy(lua_State* L)
{
    elfConfig* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigTextureAnisotropy", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigTextureAnisotropy", 1, "elfConfig");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetConfigTextureAnisotropy", 2, "number");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetConfigTextureAnisotropy(arg0, arg1);
    return 0;
}

int lua_SetConfigShadowMapSize(lua_State* L)
{
    elfConfig* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigShadowMapSize", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigShadowMapSize", 1, "elfConfig");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetConfigShadowMapSize", 2, "number");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetConfigShadowMapSize(arg0, arg1);
    return 0;
}

int lua_SetConfigStart(lua_State* L)
{
    elfConfig* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigStart", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigStart", 1, "elfConfig");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetConfigStart", 2, "string");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetConfigStart(arg0, arg1);
    return 0;
}

int lua_SetConfigLogPath(lua_State* L)
{
    elfConfig* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetConfigLogPath", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "SetConfigLogPath", 1, "elfConfig");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetConfigLogPath", 2, "string");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetConfigLogPath(arg0, arg1);
    return 0;
}

int lua_GetConfigWindowSize(lua_State* L)
{
    elfVec2i result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigWindowSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigWindowSize", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigWindowSize(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}

int lua_GetConfigMultisamples(lua_State* L)
{
    int result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigMultisamples", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigMultisamples", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigMultisamples(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_GetConfigFullscreen(lua_State* L)
{
    unsigned char result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigFullscreen", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigFullscreen", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigFullscreen(arg0);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetConfigTextureCompress(lua_State* L)
{
    unsigned char result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigTextureCompress", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigTextureCompress", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigTextureCompress(arg0);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetConfigTextureAnisotropy(lua_State* L)
{
    float result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigTextureAnisotropy", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigTextureAnisotropy", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigTextureAnisotropy(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_GetConfigShadowMapSize(lua_State* L)
{
    int result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigShadowMapSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigShadowMapSize", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigShadowMapSize(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_GetConfigStart(lua_State* L)
{
    const char* result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigStart", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigStart", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigStart(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_GetConfigLogPath(lua_State* L)
{
    const char* result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetConfigLogPath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "GetConfigLogPath", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetConfigLogPath(arg0);
    lua_pushstring(L, result);
    return 1;
}