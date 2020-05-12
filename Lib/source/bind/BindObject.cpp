#include "bind/BindObject.h"

#include "bind/BindUtil.h"
#include "nelf/Object.h"

int lua_IncRef(lua_State* L)
{
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IncRef", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "IncRef", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfIncRef(arg0);
    return 0;
}

int lua_DecRef(lua_State* L)
{
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "DecRef", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "DecRef", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfDecRef(arg0);
    return 0;
}

int lua_GetObjectType(lua_State* L)
{
    int result;
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetObjectType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "GetObjectType", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetObjectType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_GetObjectRefCount(lua_State* L)
{
    int result;
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetObjectRefCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "GetObjectRefCount", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetObjectRefCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_IsActor(lua_State* L)
{
    unsigned char result;
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsActor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "IsActor", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsActor(arg0);
    lua_pushboolean(L, result);
    return 1;
}

int lua_IsGuiObject(lua_State* L)
{
    unsigned char result;
    elfObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsGuiObject", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "IsGuiObject", 1, "elfObject");
    }
    arg0 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsGuiObject(arg0);
    lua_pushboolean(L, result);
    return 1;
}