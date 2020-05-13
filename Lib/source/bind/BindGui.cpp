#include "bind/BindGui.h"

#include "bind/BindUtil.h"
#include "nelf/Object.h"
#include "nelf/gui/Gui.h"
#include "nelf/gui/GuiObject.h"

int lua_CreateGui(lua_State* L)
{
    elfGui* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateGui", lua_gettop(L), 0);
    }
    result = elfCreateGui();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_AddGuiObject(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    elfGuiObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddGuiObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddGuiObject", 1, "elfGuiObject");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "AddGuiObject", 2, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfAddGuiObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetGuiObjectByName(lua_State* L)
{
    elfGuiObject* result;
    elfGuiObject* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetGuiObjectByName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectByName", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetGuiObjectByName", 2, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetGuiObjectByName(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetGuiObjectByIndex(lua_State* L)
{
    elfGuiObject* result;
    elfGuiObject* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetGuiObjectByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectByIndex", 1, "elfGuiObject");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetGuiObjectByIndex", 2, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetGuiObjectByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_RemoveGuiObjectByName(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveGuiObjectByName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByName", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByName", 2, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveGuiObjectByName(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}

int lua_RemoveGuiObjectByIndex(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveGuiObjectByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByIndex", 1, "elfGuiObject");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByIndex", 2, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveGuiObjectByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}

int lua_RemoveGuiObjectByObject(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    elfGuiObject* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveGuiObjectByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByObject", 1, "elfGuiObject");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "RemoveGuiObjectByObject", 2, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveGuiObjectByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetGuiTrace(lua_State* L)
{
    elfGuiObject* result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiTrace", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiTrace", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiTrace(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetGuiFocus(lua_State* L)
{
    elfGuiObject* result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiFocus", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiFocus", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiFocus(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetGuiActiveTextField(lua_State* L)
{
    elfGuiObject* result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiActiveTextField", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiActiveTextField", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiActiveTextField(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetGuiDragging(lua_State* L)
{
    unsigned char result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiDragging", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiDragging", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiDragging(arg0);
    lua_pushboolean(L, result);
    return 1;
}

int lua_GetGuiDragObject(lua_State* L)
{
    elfGuiObject* result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiDragObject", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiDragObject", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiDragObject(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetGuiDragContent(lua_State* L)
{
    const char* result;
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiDragContent", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "GetGuiDragContent", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiDragContent(arg0);
    lua_pushstring(L, result);
    return 1;
}

int lua_EmptyGui(lua_State* L)
{
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "EmptyGui", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "EmptyGui", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfEmptyGui(arg0);
    return 0;
}