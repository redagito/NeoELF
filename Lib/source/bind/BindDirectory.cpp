#include "bind/BindDirectory.h"

#include "bind/BindUtil.h"
#include "nelf/Directory.h"
#include "nelf/Object.h"

int lua_ReadDirectory(lua_State* L)
{
    elfDirectory* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ReadDirectory", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "ReadDirectory", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfReadDirectory(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetDirectoryPath(lua_State* L)
{
    const char* result;
    elfDirectory* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetDirectoryPath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_DIRECTORY)
    {
        return lua_fail_arg(L, "GetDirectoryPath", 1, "elfDirectory");
    }
    arg0 = (elfDirectory*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetDirectoryPath(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_GetDirectoryItemCount(lua_State* L)
{
    int result;
    elfDirectory* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetDirectoryItemCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_DIRECTORY)
    {
        return lua_fail_arg(L, "GetDirectoryItemCount", 1, "elfDirectory");
    }
    arg0 = (elfDirectory*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetDirectoryItemCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_GetDirectoryItem(lua_State* L)
{
    elfDirectoryItem* result;
    elfDirectory* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetDirectoryItem", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_DIRECTORY)
    {
        return lua_fail_arg(L, "GetDirectoryItem", 1, "elfDirectory");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetDirectoryItem", 2, "number");
    }
    arg0 = (elfDirectory*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetDirectoryItem(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetDirectoryItemName(lua_State* L)
{
    const char* result;
    elfDirectoryItem* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetDirectoryItemName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_DIRECTORY_ITEM)
    {
        return lua_fail_arg(L, "GetDirectoryItemName", 1, "elfDirectoryItem");
    }
    arg0 = (elfDirectoryItem*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetDirectoryItemName(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_GetDirectoryItemType(lua_State* L)
{
    int result;
    elfDirectoryItem* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetDirectoryItemType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_DIRECTORY_ITEM)
    {
        return lua_fail_arg(L, "GetDirectoryItemType", 1, "elfDirectoryItem");
    }
    arg0 = (elfDirectoryItem*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetDirectoryItemType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}