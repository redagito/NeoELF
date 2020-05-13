#include "bind/BindNetwork.h"

#include "bind/BindUtil.h"
#include "nelf/Object.h"
#include "nelf/resource/Network.h"

int lua_CreateRequest(lua_State* L)
{
    elfRequest* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateRequest", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateRequest", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateRequest(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_SendRequest(lua_State* L)
{
    elfRequest* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SendRequest", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_REQUEST)
    {
        return lua_fail_arg(L, "SendRequest", 1, "elfRequest");
    }
    arg0 = (elfRequest*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfSendRequest(arg0);
    return 0;
}

int lua_GetRequestUrl(lua_State* L)
{
    const char* result;
    elfRequest* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetRequestUrl", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_REQUEST)
    {
        return lua_fail_arg(L, "GetRequestUrl", 1, "elfRequest");
    }
    arg0 = (elfRequest*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetRequestUrl(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_SetRequestUrl(lua_State* L)
{
    elfRequest* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetRequestUrl", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_REQUEST)
    {
        return lua_fail_arg(L, "SetRequestUrl", 1, "elfRequest");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetRequestUrl", 2, "string");
    }
    arg0 = (elfRequest*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetRequestUrl(arg0, arg1);
    return 0;
}

int lua_GetRequestMethod(lua_State* L)
{
    const char* result;
    elfRequest* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetRequestMethod", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_REQUEST)
    {
        return lua_fail_arg(L, "GetRequestMethod", 1, "elfRequest");
    }
    arg0 = (elfRequest*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetRequestMethod(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_SetRequestMethod(lua_State* L)
{
    elfRequest* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetRequestMethod", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_REQUEST)
    {
        return lua_fail_arg(L, "SetRequestMethod", 1, "elfRequest");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetRequestMethod", 2, "string");
    }
    arg0 = (elfRequest*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetRequestMethod(arg0, arg1);
    return 0;
}

int lua_GetRequestState(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetRequestState", lua_gettop(L), 0);
    }
    result = elfGetRequestState();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}