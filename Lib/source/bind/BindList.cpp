#include "bind/BindList.h"

#include "nelf/List.h"
#include "nelf/Object.h"

int lua_CreateList(lua_State* L)
{
    elfList* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateList", lua_gettop(L), 0);
    }
    result = elfCreateList();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetListLength(lua_State* L)
{
    int result;
    elfList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetListLength", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "GetListLength", 1, "elfList");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetListLength(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
int lua_InsertListObject(lua_State* L)
{
    elfList* arg0;
    int arg1;
    elfObject* arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "InsertListObject", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "InsertListObject", 1, "elfList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "InsertListObject", 2, "number");
    }
    if (!lua_isuserdata(L, 3) || ((lua_elf_userdata*)lua_touserdata(L, 3))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "InsertListObject", 3, "elfObject");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 3))->object;
    elfInsertListObject(arg0, arg1, arg2);
    return 0;
}

int lua_AppendListObject(lua_State* L)
{
    elfList* arg0;
    elfObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AppendListObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "AppendListObject", 1, "elfList");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "AppendListObject", 2, "elfObject");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAppendListObject(arg0, arg1);
    return 0;
}

int lua_RemoveListObject(lua_State* L)
{
    unsigned char result;
    elfList* arg0;
    elfObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveListObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "RemoveListObject", 1, "elfList");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "RemoveListObject", 2, "elfObject");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveListObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetListObject(lua_State* L)
{
    elfObject* result;
    elfList* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetListObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "GetListObject", 1, "elfList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetListObject", 2, "number");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetListObject(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_BeginList(lua_State* L)
{
    elfObject* result;
    elfList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "BeginList", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "BeginList", 1, "elfList");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfBeginList(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetListNext(lua_State* L)
{
    elfObject* result;
    elfList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetListNext", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "GetListNext", 1, "elfList");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetListNext(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_RBeginList(lua_State* L)
{
    elfObject* result;
    elfList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "RBeginList", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "RBeginList", 1, "elfList");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfRBeginList(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetListRNext(lua_State* L)
{
    elfObject* result;
    elfList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetListRNext", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "GetListRNext", 1, "elfList");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetListRNext(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_SeekList(lua_State* L)
{
    elfList* arg0;
    elfObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SeekList", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "SeekList", 1, "elfList");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "SeekList", 2, "elfObject");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSeekList(arg0, arg1);
    return 0;
}

int lua_RSeekList(lua_State* L)
{
    elfList* arg0;
    elfObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RSeekList", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIST)
    {
        return lua_fail_arg(L, "RSeekList", 1, "elfList");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT)
    {
        return lua_fail_arg(L, "RSeekList", 2, "elfObject");
    }
    arg0 = (elfList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfRSeekList(arg0, arg1);
    return 0;
}