#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <lua.hpp>

#include "bind/BindAudio.h"
#include "bind/BindObject.h"
#include "bind/BindUtil.h"
#include "nelf/Color.h"
#include "nelf/Config.h"
#include "nelf/Context.h"
#include "nelf/Directory.h"
#include "nelf/Engine.h"
#include "nelf/Face.h"
#include "nelf/Font.h"
#include "nelf/FramePlayer.h"
#include "nelf/General.h"
#include "nelf/Image.h"
#include "nelf/Ipo.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Math.h"
#include "nelf/MeshData.h"
#include "nelf/Object.h"
#include "nelf/PostProcess.h"
#include "nelf/Property.h"
#include "nelf/Random.h"
#include "nelf/Scripting.h"
#include "nelf/String.h"
#include "nelf/Timer.h"
#include "nelf/Vector.h"
#include "nelf/Version.h"
#include "nelf/Vertex.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/actor/Entity.h"
#include "nelf/actor/Light.h"
#include "nelf/actor/Particle.h"
#include "nelf/actor/Particles.h"
#include "nelf/actor/Sprite.h"
#include "nelf/audio/AudioDevice.h"
#include "nelf/audio/AudioSource.h"
#include "nelf/audio/Sound.h"
#include "nelf/bind/Bind.h"
#include "nelf/gui/Button.h"
#include "nelf/gui/CheckBox.h"
#include "nelf/gui/Gui.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/gui/Label.h"
#include "nelf/gui/Picture.h"
#include "nelf/gui/Screen.h"
#include "nelf/gui/Slider.h"
#include "nelf/gui/TextField.h"
#include "nelf/gui/TextList.h"
#include "nelf/objectType.h"
#include "nelf/physics/Collision.h"
#include "nelf/physics/Joint.h"
#include "nelf/resource/Armature.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Network.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Scene.h"
#include "nelf/resource/Script.h"
#include "nelf/resource/Texture.h"

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
struct lua_elfColor
{
    LUA_ELF_USERDATA_HEADER;
    elfColor val;
};

void lua_create_elfObject(lua_State* L, elfObject* obj)
{
    lua_elfObject* ud;
    ud = (lua_elfObject*)lua_newuserdata(L, sizeof(lua_elfObject));
    ud->type = LUA_ELF_OBJECT;
    ud->object = obj;
    elfIncRef(ud->object);
    luaL_getmetatable(L, "lua_elfObject_mt");
    lua_setmetatable(L, -2);
}

int lua_elfObject__gc(lua_State* L)
{
    lua_elfObject* obj = (lua_elfObject*)lua_touserdata(L, 1);
    elfDecRef(obj->object);
    return 0;
}

static int lua_elfVec2i__index(lua_State* L)
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
static int lua_elfVec2i__newindex(lua_State* L)
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
static int lua_elfVec2f__index(lua_State* L)
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
static int lua_elfVec2f__newindex(lua_State* L)
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
static int lua_elfVec3f__index(lua_State* L)
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
static int lua_elfVec3f__newindex(lua_State* L)
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

static int lua_elfVec4f__index(lua_State* L)
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

static int lua_elfVec4f__newindex(lua_State* L)
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

static int lua_elfColor__index(lua_State* L)
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
static int lua_elfColor__newindex(lua_State* L)
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

static const luaL_Reg lua_elfVec2i_mt[] = {
    {"__index", lua_elfVec2i__index}, {"__newindex", lua_elfVec2i__newindex}, {NULL, NULL}};

static const luaL_Reg lua_elfVec2f_mt[] = {
    {"__index", lua_elfVec2f__index}, {"__newindex", lua_elfVec2f__newindex}, {NULL, NULL}};

static const luaL_Reg lua_elfVec3f_mt[] = {
    {"__index", lua_elfVec3f__index}, {"__newindex", lua_elfVec3f__newindex}, {NULL, NULL}};

static const luaL_Reg lua_elfVec4f_mt[] = {
    {"__index", lua_elfVec4f__index}, {"__newindex", lua_elfVec4f__newindex}, {NULL, NULL}};

static const luaL_Reg lua_elfColor_mt[] = {
    {"__index", lua_elfColor__index}, {"__newindex", lua_elfColor__newindex}, {NULL, NULL}};

static const luaL_Reg lua_elfObject_mt[] = {{"__gc", lua_elfObject__gc}, {NULL, NULL}};

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
void lua_create_elfColor(lua_State* L, elfColor col)
{
    lua_elfColor* ud;
    ud = (lua_elfColor*)lua_newuserdata(L, sizeof(lua_elfColor));
    ud->type = LUA_ELF_COLOR;
    ud->val = col;
    luaL_getmetatable(L, "lua_elfColor_mt");
    lua_setmetatable(L, -2);
}

static int lua_GetGlobalRefCount(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetGlobalRefCount", lua_gettop(L), 0);
    }
    result = elfGetGlobalRefCount();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

static int lua_GetGlobalObjCount(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetGlobalObjCount", lua_gettop(L), 0);
    }
    result = elfGetGlobalObjCount();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

static int lua_CreateList(lua_State* L)
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
static int lua_GetListLength(lua_State* L)
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
static int lua_InsertListObject(lua_State* L)
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
static int lua_AppendListObject(lua_State* L)
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
static int lua_RemoveListObject(lua_State* L)
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
static int lua_GetListObject(lua_State* L)
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
static int lua_BeginList(lua_State* L)
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
static int lua_GetListNext(lua_State* L)
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
static int lua_RBeginList(lua_State* L)
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
static int lua_GetListRNext(lua_State* L)
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
static int lua_SeekList(lua_State* L)
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
static int lua_RSeekList(lua_State* L)
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
static int lua_ReadConfig(lua_State* L)
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
static int lua_SetConfigWindowSize(lua_State* L)
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
static int lua_SetConfigMultisamples(lua_State* L)
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
static int lua_SetConfigFullscreen(lua_State* L)
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
static int lua_SetConfigTextureCompress(lua_State* L)
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
static int lua_SetConfigTextureAnisotropy(lua_State* L)
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
static int lua_SetConfigShadowMapSize(lua_State* L)
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
static int lua_SetConfigStart(lua_State* L)
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
static int lua_SetConfigLogPath(lua_State* L)
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
static int lua_GetConfigWindowSize(lua_State* L)
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
static int lua_GetConfigMultisamples(lua_State* L)
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
static int lua_GetConfigFullscreen(lua_State* L)
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
static int lua_GetConfigTextureCompress(lua_State* L)
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
static int lua_GetConfigTextureAnisotropy(lua_State* L)
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
static int lua_GetConfigShadowMapSize(lua_State* L)
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
static int lua_GetConfigStart(lua_State* L)
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
static int lua_GetConfigLogPath(lua_State* L)
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
static int lua_WriteLogLine(lua_State* L)
{
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "WriteLogLine", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "WriteLogLine", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    elfWriteLogLine(arg0);
    return 0;
}
static int lua_SetTitle(lua_State* L)
{
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetTitle", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "SetTitle", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    elfSetTitle(arg0);
    return 0;
}
static int lua_GetWindowWidth(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetWindowWidth", lua_gettop(L), 0);
    }
    result = elfGetWindowWidth();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetWindowHeight(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetWindowHeight", lua_gettop(L), 0);
    }
    result = elfGetWindowHeight();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetVideoModeCount(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetVideoModeCount", lua_gettop(L), 0);
    }
    result = elfGetVideoModeCount();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetVideoMode(lua_State* L)
{
    elfVec2i result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVideoMode", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetVideoMode", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetVideoMode(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetMultisamples(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetMultisamples", lua_gettop(L), 0);
    }
    result = elfGetMultisamples();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_IsFullscreen(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsFullscreen", lua_gettop(L), 0);
    }
    result = elfIsFullscreen();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetTitle(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetTitle", lua_gettop(L), 0);
    }
    result = elfGetTitle();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetTime(lua_State* L)
{
    double result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetTime", lua_gettop(L), 0);
    }
    result = elfGetTime();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_Sleep(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "Sleep", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "Sleep", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSleep(arg0);
    return 0;
}
static int lua_IsWindowOpened(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsWindowOpened", lua_gettop(L), 0);
    }
    result = elfIsWindowOpened();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetMousePosition(lua_State* L)
{
    elfVec2i result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetMousePosition", lua_gettop(L), 0);
    }
    result = elfGetMousePosition();
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetMouseForce(lua_State* L)
{
    elfVec2i result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetMouseForce", lua_gettop(L), 0);
    }
    result = elfGetMouseForce();
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_SetMousePosition(lua_State* L)
{
    int arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMousePosition", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetMousePosition", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMousePosition", 2, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    elfSetMousePosition(arg0, arg1);
    return 0;
}
static int lua_HideMouse(lua_State* L)
{
    unsigned char arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "HideMouse", lua_gettop(L), 1);
    }
    if (!lua_isboolean(L, 1))
    {
        return lua_fail_arg(L, "HideMouse", 1, "boolean");
    }
    arg0 = (unsigned char)lua_toboolean(L, 1);
    elfHideMouse(arg0);
    return 0;
}
static int lua_IsMouseHidden(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsMouseHidden", lua_gettop(L), 0);
    }
    result = elfIsMouseHidden();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetMouseWheel(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetMouseWheel", lua_gettop(L), 0);
    }
    result = elfGetMouseWheel();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetMouseButtonState(lua_State* L)
{
    int result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMouseButtonState", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetMouseButtonState", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetMouseButtonState(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetKeyState(lua_State* L)
{
    int result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetKeyState", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetKeyState", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetKeyState(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetJoystickPresent(lua_State* L)
{
    unsigned char result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJoystickPresent", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetJoystickPresent", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetJoystickPresent(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetJoystickAxis(lua_State* L)
{
    elfVec2f result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJoystickAxis", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetJoystickAxis", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetJoystickAxis(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetJoystickButtonState(lua_State* L)
{
    int result;
    int arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetJoystickButtonState", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetJoystickButtonState", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetJoystickButtonState", 2, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetJoystickButtonState(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetEventCount(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetEventCount", lua_gettop(L), 0);
    }
    result = elfGetEventCount();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetEvent(lua_State* L)
{
    elfObject* result;
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEvent", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetEvent", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    result = elfGetEvent(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetKeyEventKey(lua_State* L)
{
    int result;
    elfKeyEvent* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetKeyEventKey", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_KEY_EVENT)
    {
        return lua_fail_arg(L, "GetKeyEventKey", 1, "elfKeyEvent");
    }
    arg0 = (elfKeyEvent*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetKeyEventKey(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetKeyEventState(lua_State* L)
{
    int result;
    elfKeyEvent* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetKeyEventState", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_KEY_EVENT)
    {
        return lua_fail_arg(L, "GetKeyEventState", 1, "elfKeyEvent");
    }
    arg0 = (elfKeyEvent*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetKeyEventState(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_Init(lua_State* L)
{
    unsigned char result;
    elfConfig* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "Init", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CONFIG)
    {
        return lua_fail_arg(L, "Init", 1, "elfConfig");
    }
    arg0 = (elfConfig*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfInit(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_Deinit(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "Deinit", lua_gettop(L), 0);
    }
    elfDeinit();
    return 0;
}
static int lua_ResizeWindow(lua_State* L)
{
    int arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "ResizeWindow", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "ResizeWindow", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "ResizeWindow", 2, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    elfResizeWindow(arg0, arg1);
    return 0;
}
static int lua_GetPlatform(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetPlatform", lua_gettop(L), 0);
    }
    result = elfGetPlatform();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetVersionMajor(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetVersionMajor", lua_gettop(L), 0);
    }
    result = elfGetVersionMajor();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetVersionMinor(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetVersionMinor", lua_gettop(L), 0);
    }
    result = elfGetVersionMinor();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetVersionRelease(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetVersionRelease", lua_gettop(L), 0);
    }
    result = elfGetVersionRelease();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetVersion(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetVersion", lua_gettop(L), 0);
    }
    result = elfGetVersion();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetCurrentDirectory(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetCurrentDirectory", lua_gettop(L), 0);
    }
    result = elfGetCurrentDirectory();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetErrorString(lua_State* L)
{
    const char* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetErrorString", lua_gettop(L), 0);
    }
    result = elfGetErrorString();
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetError(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetError", lua_gettop(L), 0);
    }
    result = elfGetError();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_Run(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "Run", lua_gettop(L), 0);
    }
    result = elfRun();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_Quit(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "Quit", lua_gettop(L), 0);
    }
    elfQuit();
    return 0;
}
static int lua_SetF10Exit(lua_State* L)
{
    unsigned char arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetF10Exit", lua_gettop(L), 1);
    }
    if (!lua_isboolean(L, 1))
    {
        return lua_fail_arg(L, "SetF10Exit", 1, "boolean");
    }
    arg0 = (unsigned char)lua_toboolean(L, 1);
    elfSetF10Exit(arg0);
    return 0;
}
static int lua_GetF10Exit(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetF10Exit", lua_gettop(L), 0);
    }
    result = elfGetF10Exit();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_LoadScene(lua_State* L)
{
    elfScene* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "LoadScene", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "LoadScene", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfLoadScene(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetScene(lua_State* L)
{
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetScene", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetScene", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfSetScene(arg0);
    return 0;
}
static int lua_GetScene(lua_State* L)
{
    elfScene* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetScene", lua_gettop(L), 0);
    }
    result = elfGetScene();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetGui(lua_State* L)
{
    elfGui* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetGui", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_GUI)
    {
        return lua_fail_arg(L, "SetGui", 1, "elfGui");
    }
    arg0 = (elfGui*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfSetGui(arg0);
    return 0;
}
static int lua_GetGui(lua_State* L)
{
    elfGui* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetGui", lua_gettop(L), 0);
    }
    result = elfGetGui();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSync(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetSync", lua_gettop(L), 0);
    }
    result = elfGetSync();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFps(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetFps", lua_gettop(L), 0);
    }
    result = elfGetFps();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SaveScreenShot(lua_State* L)
{
    unsigned char result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SaveScreenShot", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "SaveScreenShot", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfSaveScreenShot(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetFpsLimit(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetFpsLimit", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetFpsLimit", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetFpsLimit(arg0);
    return 0;
}
static int lua_GetFpsLimit(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetFpsLimit", lua_gettop(L), 0);
    }
    result = elfGetFpsLimit();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetTickRate(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetTickRate", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetTickRate", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetTickRate(arg0);
    return 0;
}
static int lua_GetTickRate(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetTickRate", lua_gettop(L), 0);
    }
    result = elfGetTickRate();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetSpeed(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetSpeed", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetSpeed", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetSpeed(arg0);
    return 0;
}
static int lua_GetSpeed(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetSpeed", lua_gettop(L), 0);
    }
    result = elfGetSpeed();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetTextureCompress(lua_State* L)
{
    unsigned char arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetTextureCompress", lua_gettop(L), 1);
    }
    if (!lua_isboolean(L, 1))
    {
        return lua_fail_arg(L, "SetTextureCompress", 1, "boolean");
    }
    arg0 = (unsigned char)lua_toboolean(L, 1);
    elfSetTextureCompress(arg0);
    return 0;
}
static int lua_GetTextureCompress(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetTextureCompress", lua_gettop(L), 0);
    }
    result = elfGetTextureCompress();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetTextureAnisotropy(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetTextureAnisotropy", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetTextureAnisotropy", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetTextureAnisotropy(arg0);
    return 0;
}
static int lua_GetTextureAnisotropy(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetTextureAnisotropy", lua_gettop(L), 0);
    }
    result = elfGetTextureAnisotropy();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetShadowMapSize(lua_State* L)
{
    int arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetShadowMapSize", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetShadowMapSize", 1, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    elfSetShadowMapSize(arg0);
    return 0;
}
static int lua_GetShadowMapSize(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetShadowMapSize", lua_gettop(L), 0);
    }
    result = elfGetShadowMapSize();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetPolygonsRendered(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetPolygonsRendered", lua_gettop(L), 0);
    }
    result = elfGetPolygonsRendered();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetBloom(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetBloom", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetBloom", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetBloom(arg0);
    return 0;
}
static int lua_DisableBloom(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "DisableBloom", lua_gettop(L), 0);
    }
    elfDisableBloom();
    return 0;
}
static int lua_GetBloomThreshold(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetBloomThreshold", lua_gettop(L), 0);
    }
    result = elfGetBloomThreshold();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetDof(lua_State* L)
{
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetDof", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetDof", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetDof", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    elfSetDof(arg0, arg1);
    return 0;
}
static int lua_DisableDof(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "DisableDof", lua_gettop(L), 0);
    }
    elfDisableDof();
    return 0;
}
static int lua_GetDofFocalRange(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetDofFocalRange", lua_gettop(L), 0);
    }
    result = elfGetDofFocalRange();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetDofFocalDistance(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetDofFocalDistance", lua_gettop(L), 0);
    }
    result = elfGetDofFocalDistance();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetSsao(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetSsao", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetSsao", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetSsao(arg0);
    return 0;
}
static int lua_DisableSsao(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "DisableSsao", lua_gettop(L), 0);
    }
    elfDisableSsao();
    return 0;
}
static int lua_GetSsaoAmount(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetSsaoAmount", lua_gettop(L), 0);
    }
    result = elfGetSsaoAmount();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetLightShafts(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetLightShafts", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetLightShafts", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetLightShafts(arg0);
    return 0;
}
static int lua_DisableLightShafts(lua_State* L)
{
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "DisableLightShafts", lua_gettop(L), 0);
    }
    elfDisableLightShafts();
    return 0;
}
static int lua_GetLightShaftsIntensity(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetLightShaftsIntensity", lua_gettop(L), 0);
    }
    result = elfGetLightShaftsIntensity();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_IsBloom(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsBloom", lua_gettop(L), 0);
    }
    result = elfIsBloom();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsSsao(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsSsao", lua_gettop(L), 0);
    }
    result = elfIsSsao();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsDof(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsDof", lua_gettop(L), 0);
    }
    result = elfIsDof();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsLightShafts(lua_State* L)
{
    unsigned char result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "IsLightShafts", lua_gettop(L), 0);
    }
    result = elfIsLightShafts();
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetActor(lua_State* L)
{
    elfObject* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetActor", lua_gettop(L), 0);
    }
    result = elfGetActor();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_ReadDirectory(lua_State* L)
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
static int lua_GetDirectoryPath(lua_State* L)
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
static int lua_GetDirectoryItemCount(lua_State* L)
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
static int lua_GetDirectoryItem(lua_State* L)
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
static int lua_GetDirectoryItemName(lua_State* L)
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
static int lua_GetDirectoryItemType(lua_State* L)
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
static int lua_CreateColor(lua_State* L)
{
    elfColor result;
    float arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "CreateColor", lua_gettop(L), 4);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateColor", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateColor", 4, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfCreateColor(arg0, arg1, arg2, arg3);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_CreateVec2f(lua_State* L)
{
    elfVec2f result;
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateVec2f", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateVec2f", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateVec2f", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    result = elfCreateVec2f(arg0, arg1);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_CreateVec3f(lua_State* L)
{
    elfVec3f result;
    float arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "CreateVec3f", lua_gettop(L), 3);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateVec3f", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateVec3f", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateVec3f", 3, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    result = elfCreateVec3f(arg0, arg1, arg2);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_CreateQua(lua_State* L)
{
    elfVec4f result;
    float arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "CreateQua", lua_gettop(L), 4);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateQua", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateQua", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateQua", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateQua", 4, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfCreateQua(arg0, arg1, arg2, arg3);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_CreateQuaFromEuler(lua_State* L)
{
    elfVec4f result;
    float arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "CreateQuaFromEuler", lua_gettop(L), 3);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateQuaFromEuler", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateQuaFromEuler", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateQuaFromEuler", 3, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    result = elfCreateQuaFromEuler(arg0, arg1, arg2);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_CreateQuaFromAngleAxis(lua_State* L)
{
    elfVec4f result;
    float arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "CreateQuaFromAngleAxis", lua_gettop(L), 4);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateQuaFromAngleAxis", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateQuaFromAngleAxis", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateQuaFromAngleAxis", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateQuaFromAngleAxis", 4, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfCreateQuaFromAngleAxis(arg0, arg1, arg2, arg3);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_AddVec2fVec2f(lua_State* L)
{
    elfVec2f result;
    elfVec2f arg0;
    elfVec2f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddVec2fVec2f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "AddVec2fVec2f", 1, "elfVec2f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "AddVec2fVec2f", 2, "elfVec2f");
    }
    arg0 = ((lua_elfVec2f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec2f*)lua_touserdata(L, 2))->val;
    result = elfAddVec2fVec2f(arg0, arg1);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_SubVec2fVec2f(lua_State* L)
{
    elfVec2f result;
    elfVec2f arg0;
    elfVec2f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SubVec2fVec2f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "SubVec2fVec2f", 1, "elfVec2f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "SubVec2fVec2f", 2, "elfVec2f");
    }
    arg0 = ((lua_elfVec2f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec2f*)lua_touserdata(L, 2))->val;
    result = elfSubVec2fVec2f(arg0, arg1);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_MulVec2fVec2f(lua_State* L)
{
    elfVec2f result;
    elfVec2f arg0;
    elfVec2f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "MulVec2fVec2f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "MulVec2fVec2f", 1, "elfVec2f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "MulVec2fVec2f", 2, "elfVec2f");
    }
    arg0 = ((lua_elfVec2f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec2f*)lua_touserdata(L, 2))->val;
    result = elfMulVec2fVec2f(arg0, arg1);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetVec2fLength(lua_State* L)
{
    float result;
    elfVec2f arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVec2fLength", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "GetVec2fLength", 1, "elfVec2f");
    }
    arg0 = ((lua_elfVec2f*)lua_touserdata(L, 1))->val;
    result = elfGetVec2fLength(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddVec3fVec3f(lua_State* L)
{
    elfVec3f result;
    elfVec3f arg0;
    elfVec3f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddVec3fVec3f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "AddVec3fVec3f", 1, "elfVec3f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "AddVec3fVec3f", 2, "elfVec3f");
    }
    arg0 = ((lua_elfVec3f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec3f*)lua_touserdata(L, 2))->val;
    result = elfAddVec3fVec3f(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_SubVec3fVec3f(lua_State* L)
{
    elfVec3f result;
    elfVec3f arg0;
    elfVec3f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SubVec3fVec3f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "SubVec3fVec3f", 1, "elfVec3f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "SubVec3fVec3f", 2, "elfVec3f");
    }
    arg0 = ((lua_elfVec3f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec3f*)lua_touserdata(L, 2))->val;
    result = elfSubVec3fVec3f(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_MulVec3fVec3f(lua_State* L)
{
    elfVec3f result;
    elfVec3f arg0;
    elfVec3f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "MulVec3fVec3f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "MulVec3fVec3f", 1, "elfVec3f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "MulVec3fVec3f", 2, "elfVec3f");
    }
    arg0 = ((lua_elfVec3f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec3f*)lua_touserdata(L, 2))->val;
    result = elfMulVec3fVec3f(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetVec3fLength(lua_State* L)
{
    float result;
    elfVec3f arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVec3fLength", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "GetVec3fLength", 1, "elfVec3f");
    }
    arg0 = ((lua_elfVec3f*)lua_touserdata(L, 1))->val;
    result = elfGetVec3fLength(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_RotateVec3f(lua_State* L)
{
    elfVec3f result;
    elfVec3f arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "RotateVec3f", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "RotateVec3f", 1, "elfVec3f");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RotateVec3f", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "RotateVec3f", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "RotateVec3f", 4, "number");
    }
    arg0 = ((lua_elfVec3f*)lua_touserdata(L, 1))->val;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfRotateVec3f(arg0, arg1, arg2, arg3);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_MulQuaVec3f(lua_State* L)
{
    elfVec3f result;
    elfVec4f arg0;
    elfVec3f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "MulQuaVec3f", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "MulQuaVec3f", 1, "elfVec4f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "MulQuaVec3f", 2, "elfVec3f");
    }
    arg0 = ((lua_elfVec4f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec3f*)lua_touserdata(L, 2))->val;
    result = elfMulQuaVec3f(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_MulQuaQua(lua_State* L)
{
    elfVec4f result;
    elfVec4f arg0;
    elfVec4f arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "MulQuaQua", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "MulQuaQua", 1, "elfVec4f");
    }
    if (!lua_isuserdata(L, 2))
    {
        return lua_fail_arg(L, "MulQuaQua", 2, "elfVec4f");
    }
    arg0 = ((lua_elfVec4f*)lua_touserdata(L, 1))->val;
    arg1 = ((lua_elfVec4f*)lua_touserdata(L, 2))->val;
    result = elfMulQuaQua(arg0, arg1);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_GetQuaInverted(lua_State* L)
{
    elfVec4f result;
    elfVec4f arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetQuaInverted", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1))
    {
        return lua_fail_arg(L, "GetQuaInverted", 1, "elfVec4f");
    }
    arg0 = ((lua_elfVec4f*)lua_touserdata(L, 1))->val;
    result = elfGetQuaInverted(arg0);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_AboutSame(lua_State* L)
{
    unsigned char result;
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AboutSame", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "AboutSame", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "AboutSame", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    result = elfAboutSame(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_AboutZero(lua_State* L)
{
    unsigned char result;
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "AboutZero", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "AboutZero", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    result = elfAboutZero(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_FloatAbs(lua_State* L)
{
    float result;
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "FloatAbs", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "FloatAbs", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    result = elfFloatAbs(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_FloatMin(lua_State* L)
{
    float result;
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "FloatMin", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "FloatMin", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "FloatMin", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    result = elfFloatMin(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_FloatMax(lua_State* L)
{
    float result;
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "FloatMax", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "FloatMax", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "FloatMax", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    result = elfFloatMax(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_RandomFloat(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "RandomFloat", lua_gettop(L), 0);
    }
    result = elfRandomFloat();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_RandomFloatRange(lua_State* L)
{
    float result;
    float arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RandomFloatRange", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "RandomFloatRange", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RandomFloatRange", 2, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    arg1 = (float)lua_tonumber(L, 2);
    result = elfRandomFloatRange(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_RandomInt(lua_State* L)
{
    int result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "RandomInt", lua_gettop(L), 0);
    }
    result = elfRandomInt();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_RandomIntRange(lua_State* L)
{
    int result;
    int arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RandomIntRange", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "RandomIntRange", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RandomIntRange", 2, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRandomIntRange(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetResourcesTexture(lua_State* L)
{
    elfTexture* result;
    const char* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetResourcesTexture", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "GetResourcesTexture", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetResourcesTexture", 2, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    result = elfGetResourcesTexture(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetOrLoadResourcesTexture(lua_State* L)
{
    elfTexture* result;
    const char* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetOrLoadResourcesTexture", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "GetOrLoadResourcesTexture", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetOrLoadResourcesTexture", 2, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    result = elfGetOrLoadResourcesTexture(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateFramePlayer(lua_State* L)
{
    elfFramePlayer* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateFramePlayer", lua_gettop(L), 0);
    }
    result = elfCreateFramePlayer();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_UpdateFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "UpdateFramePlayer", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "UpdateFramePlayer", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfUpdateFramePlayer(arg0);
    return 0;
}
static int lua_SetFramePlayerFrame(lua_State* L)
{
    elfFramePlayer* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetFramePlayerFrame", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "SetFramePlayerFrame", 1, "elfFramePlayer");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetFramePlayerFrame", 2, "number");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetFramePlayerFrame(arg0, arg1);
    return 0;
}
static int lua_PlayFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "PlayFramePlayer", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "PlayFramePlayer", 1, "elfFramePlayer");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "PlayFramePlayer", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "PlayFramePlayer", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "PlayFramePlayer", 4, "number");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfPlayFramePlayer(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_LoopFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "LoopFramePlayer", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "LoopFramePlayer", 1, "elfFramePlayer");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "LoopFramePlayer", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "LoopFramePlayer", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "LoopFramePlayer", 4, "number");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfLoopFramePlayer(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_StopFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "StopFramePlayer", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "StopFramePlayer", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfStopFramePlayer(arg0);
    return 0;
}
static int lua_PauseFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "PauseFramePlayer", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "PauseFramePlayer", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfPauseFramePlayer(arg0);
    return 0;
}
static int lua_ResumeFramePlayer(lua_State* L)
{
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ResumeFramePlayer", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "ResumeFramePlayer", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfResumeFramePlayer(arg0);
    return 0;
}
static int lua_GetFramePlayerStart(lua_State* L)
{
    float result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFramePlayerStart", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "GetFramePlayerStart", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFramePlayerStart(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFramePlayerEnd(lua_State* L)
{
    float result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFramePlayerEnd", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "GetFramePlayerEnd", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFramePlayerEnd(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFramePlayerSpeed(lua_State* L)
{
    float result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFramePlayerSpeed", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "GetFramePlayerSpeed", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFramePlayerSpeed(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFramePlayerFrame(lua_State* L)
{
    float result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFramePlayerFrame", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "GetFramePlayerFrame", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFramePlayerFrame(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_IsFramePlayerPlaying(lua_State* L)
{
    unsigned char result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsFramePlayerPlaying", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "IsFramePlayerPlaying", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsFramePlayerPlaying(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsFramePlayerPaused(lua_State* L)
{
    unsigned char result;
    elfFramePlayer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsFramePlayerPaused", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FRAME_PLAYER)
    {
        return lua_fail_arg(L, "IsFramePlayerPaused", 1, "elfFramePlayer");
    }
    arg0 = (elfFramePlayer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsFramePlayerPaused(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_CreateTimer(lua_State* L)
{
    elfTimer* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateTimer", lua_gettop(L), 0);
    }
    result = elfCreateTimer();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_StartTimer(lua_State* L)
{
    elfTimer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "StartTimer", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TIMER)
    {
        return lua_fail_arg(L, "StartTimer", 1, "elfTimer");
    }
    arg0 = (elfTimer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfStartTimer(arg0);
    return 0;
}
static int lua_GetElapsedTime(lua_State* L)
{
    double result;
    elfTimer* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetElapsedTime", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TIMER)
    {
        return lua_fail_arg(L, "GetElapsedTime", 1, "elfTimer");
    }
    arg0 = (elfTimer*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetElapsedTime(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_CreateEmptyImage(lua_State* L)
{
    elfImage* result;
    int arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "CreateEmptyImage", lua_gettop(L), 3);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "CreateEmptyImage", 1, "number");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateEmptyImage", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateEmptyImage", 3, "number");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    result = elfCreateEmptyImage(arg0, arg1, arg2);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_CreateImageFromFile(lua_State* L)
{
    elfImage* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateImageFromFile", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateImageFromFile", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateImageFromFile(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_SetImagePixel(lua_State* L)
{
    elfImage* arg0;
    int arg1;
    int arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "SetImagePixel", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "SetImagePixel", 1, "elfImage");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetImagePixel", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetImagePixel", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetImagePixel", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetImagePixel", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "SetImagePixel", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "SetImagePixel", 7, "number");
    }
    arg0 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    elfSetImagePixel(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    return 0;
}
static int lua_GetImageWidth(lua_State* L)
{
    int result;
    elfImage* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetImageWidth", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "GetImageWidth", 1, "elfImage");
    }
    arg0 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetImageWidth(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetImageHeight(lua_State* L)
{
    int result;
    elfImage* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetImageHeight", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "GetImageHeight", 1, "elfImage");
    }
    arg0 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetImageHeight(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetImageBitsPerPixel(lua_State* L)
{
    int result;
    elfImage* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetImageBitsPerPixel", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "GetImageBitsPerPixel", 1, "elfImage");
    }
    arg0 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetImageBitsPerPixel(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetImagePixel(lua_State* L)
{
    elfColor result;
    elfImage* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "GetImagePixel", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "GetImagePixel", 1, "elfImage");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetImagePixel", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "GetImagePixel", 3, "number");
    }
    arg0 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    result = elfGetImagePixel(arg0, arg1, arg2);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_CreateTextureFromFile(lua_State* L)
{
    elfTexture* result;
    const char* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateTextureFromFile", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateTextureFromFile", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateTextureFromFile", 2, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    result = elfCreateTextureFromFile(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateTextureFromImage(lua_State* L)
{
    elfTexture* result;
    const char* arg0;
    elfImage* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateTextureFromImage", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateTextureFromImage", 1, "string");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_IMAGE)
    {
        return lua_fail_arg(L, "CreateTextureFromImage", 2, "elfImage");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = (elfImage*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfCreateTextureFromImage(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateCubeMapFromFiles(lua_State* L)
{
    elfTexture* result;
    const char* arg0;
    const char* arg1;
    const char* arg2;
    const char* arg3;
    const char* arg4;
    const char* arg5;
    const char* arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "CreateCubeMapFromFiles", lua_gettop(L), 7);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 2, "string");
    }
    if (!lua_isstring(L, 3))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 3, "string");
    }
    if (!lua_isstring(L, 4))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 4, "string");
    }
    if (!lua_isstring(L, 5))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 5, "string");
    }
    if (!lua_isstring(L, 6))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 6, "string");
    }
    if (!lua_isstring(L, 7))
    {
        return lua_fail_arg(L, "CreateCubeMapFromFiles", 7, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    arg2 = lua_tostring(L, 3);
    arg3 = lua_tostring(L, 4);
    arg4 = lua_tostring(L, 5);
    arg5 = lua_tostring(L, 6);
    arg6 = lua_tostring(L, 7);
    result = elfCreateCubeMapFromFiles(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetTextureName(lua_State* L)
{
    elfTexture* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextureName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetTextureName", 1, "elfTexture");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetTextureName", 2, "string");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetTextureName(arg0, arg1);
    return 0;
}
static int lua_GetTextureName(lua_State* L)
{
    const char* result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureName", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetTextureFilePath(lua_State* L)
{
    const char* result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureFilePath", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetTextureWidth(lua_State* L)
{
    int result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureWidth", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureWidth", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureWidth(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextureHeight(lua_State* L)
{
    int result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureHeight", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureHeight", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureHeight(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextureFormat(lua_State* L)
{
    int result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureFormat", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureFormat", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureFormat(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextureDataFormat(lua_State* L)
{
    int result;
    elfTexture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextureDataFormat", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "GetTextureDataFormat", 1, "elfTexture");
    }
    arg0 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextureDataFormat(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_CreateMaterial(lua_State* L)
{
    elfMaterial* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateMaterial", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateMaterial", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateMaterial(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetMaterialDiffuseColor(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetMaterialDiffuseColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialDiffuseColor", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialDiffuseColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetMaterialDiffuseColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetMaterialDiffuseColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetMaterialDiffuseColor", 5, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetMaterialDiffuseColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetMaterialSpecularColor(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetMaterialSpecularColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialSpecularColor", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialSpecularColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetMaterialSpecularColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetMaterialSpecularColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetMaterialSpecularColor", 5, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetMaterialSpecularColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetMaterialAmbientColor(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetMaterialAmbientColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialAmbientColor", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialAmbientColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetMaterialAmbientColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetMaterialAmbientColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetMaterialAmbientColor", 5, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetMaterialAmbientColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetMaterialSpecularPower(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialSpecularPower", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialSpecularPower", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialSpecularPower", 2, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetMaterialSpecularPower(arg0, arg1);
    return 0;
}
static int lua_SetMaterialLighting(lua_State* L)
{
    elfMaterial* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialLighting", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialLighting", 1, "elfMaterial");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialLighting", 2, "boolean");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetMaterialLighting(arg0, arg1);
    return 0;
}
static int lua_SetMaterialName(lua_State* L)
{
    elfMaterial* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialName", 1, "elfMaterial");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialName", 2, "string");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetMaterialName(arg0, arg1);
    return 0;
}
static int lua_GetMaterialName(lua_State* L)
{
    const char* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialName", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetMaterialFilePath(lua_State* L)
{
    const char* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialFilePath", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetMaterialDiffuseColor(lua_State* L)
{
    elfColor result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialDiffuseColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialDiffuseColor", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialDiffuseColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetMaterialSpecularColor(lua_State* L)
{
    elfColor result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialSpecularColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialSpecularColor", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialSpecularColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetMaterialAmbientColor(lua_State* L)
{
    elfColor result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialAmbientColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialAmbientColor", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialAmbientColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetMaterialSpecularPower(lua_State* L)
{
    float result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialSpecularPower", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialSpecularPower", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialSpecularPower(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetMaterialLighting(lua_State* L)
{
    unsigned char result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialLighting", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialLighting", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialLighting(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetMaterialDiffuseMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialDiffuseMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialDiffuseMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialDiffuseMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialDiffuseMap(arg0, arg1);
    return 0;
}
static int lua_SetMaterialNormalMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialNormalMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialNormalMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialNormalMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialNormalMap(arg0, arg1);
    return 0;
}
static int lua_SetMaterialHeightMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialHeightMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialHeightMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialHeightMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialHeightMap(arg0, arg1);
    return 0;
}
static int lua_SetMaterialSpecularMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialSpecularMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialSpecularMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialSpecularMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialSpecularMap(arg0, arg1);
    return 0;
}
static int lua_SetMaterialLightMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialLightMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialLightMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialLightMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialLightMap(arg0, arg1);
    return 0;
}
static int lua_SetMaterialCubeMap(lua_State* L)
{
    elfMaterial* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialCubeMap", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialCubeMap", 1, "elfMaterial");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetMaterialCubeMap", 2, "elfTexture");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetMaterialCubeMap(arg0, arg1);
    return 0;
}
static int lua_ClearMaterialDiffuseMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialDiffuseMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialDiffuseMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialDiffuseMap(arg0);
    return 0;
}
static int lua_ClearMaterialNormalMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialNormalMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialNormalMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialNormalMap(arg0);
    return 0;
}
static int lua_ClearMaterialHeightMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialHeightMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialHeightMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialHeightMap(arg0);
    return 0;
}
static int lua_ClearMaterialSpecularMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialSpecularMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialSpecularMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialSpecularMap(arg0);
    return 0;
}
static int lua_ClearMaterialLightMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialLightMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialLightMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialLightMap(arg0);
    return 0;
}
static int lua_ClearMaterialCubeMap(lua_State* L)
{
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearMaterialCubeMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "ClearMaterialCubeMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearMaterialCubeMap(arg0);
    return 0;
}
static int lua_GetMaterialDiffuseMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialDiffuseMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialDiffuseMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialDiffuseMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMaterialNormalMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialNormalMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialNormalMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialNormalMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMaterialHeightMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialHeightMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialHeightMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialHeightMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMaterialSpecularMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialSpecularMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialSpecularMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialSpecularMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMaterialLightMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialLightMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialLightMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialLightMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMaterialCubeMap(lua_State* L)
{
    elfTexture* result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialCubeMap", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialCubeMap", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialCubeMap(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetMaterialParallaxScale(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialParallaxScale", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialParallaxScale", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialParallaxScale", 2, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetMaterialParallaxScale(arg0, arg1);
    return 0;
}
static int lua_SetMaterialAlphaTest(lua_State* L)
{
    elfMaterial* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialAlphaTest", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialAlphaTest", 1, "elfMaterial");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialAlphaTest", 2, "boolean");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetMaterialAlphaTest(arg0, arg1);
    return 0;
}
static int lua_SetMaterialAlphaThreshold(lua_State* L)
{
    elfMaterial* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetMaterialAlphaThreshold", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetMaterialAlphaThreshold", 1, "elfMaterial");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetMaterialAlphaThreshold", 2, "number");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetMaterialAlphaThreshold(arg0, arg1);
    return 0;
}
static int lua_GetMaterialParallaxScale(lua_State* L)
{
    float result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialParallaxScale", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialParallaxScale", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialParallaxScale(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetMaterialAlphaTest(lua_State* L)
{
    unsigned char result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialAlphaTest", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialAlphaTest", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialAlphaTest(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetMaterialAlphaThreshold(lua_State* L)
{
    float result;
    elfMaterial* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMaterialAlphaThreshold", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "GetMaterialAlphaThreshold", 1, "elfMaterial");
    }
    arg0 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMaterialAlphaThreshold(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_CreateBezierPoint(lua_State* L)
{
    elfBezierPoint* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateBezierPoint", lua_gettop(L), 0);
    }
    result = elfCreateBezierPoint();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetBezierPointPosition(lua_State* L)
{
    elfBezierPoint* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetBezierPointPosition", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "SetBezierPointPosition", 1, "elfBezierPoint");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetBezierPointPosition", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetBezierPointPosition", 3, "number");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetBezierPointPosition(arg0, arg1, arg2);
    return 0;
}
static int lua_SetBezierPointControl1(lua_State* L)
{
    elfBezierPoint* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetBezierPointControl1", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "SetBezierPointControl1", 1, "elfBezierPoint");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetBezierPointControl1", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetBezierPointControl1", 3, "number");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetBezierPointControl1(arg0, arg1, arg2);
    return 0;
}
static int lua_SetBezierPointControl2(lua_State* L)
{
    elfBezierPoint* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetBezierPointControl2", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "SetBezierPointControl2", 1, "elfBezierPoint");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetBezierPointControl2", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetBezierPointControl2", 3, "number");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetBezierPointControl2(arg0, arg1, arg2);
    return 0;
}
static int lua_GetBezierPointPosition(lua_State* L)
{
    elfVec2f result;
    elfBezierPoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBezierPointPosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "GetBezierPointPosition", 1, "elfBezierPoint");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBezierPointPosition(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetBezierPointControl1(lua_State* L)
{
    elfVec2f result;
    elfBezierPoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBezierPointControl1", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "GetBezierPointControl1", 1, "elfBezierPoint");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBezierPointControl1(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetBezierPointControl2(lua_State* L)
{
    elfVec2f result;
    elfBezierPoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBezierPointControl2", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "GetBezierPointControl2", 1, "elfBezierPoint");
    }
    arg0 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBezierPointControl2(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_CreateBezierCurve(lua_State* L)
{
    elfBezierCurve* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateBezierCurve", lua_gettop(L), 0);
    }
    result = elfCreateBezierCurve();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetBezierCurveType(lua_State* L)
{
    elfBezierCurve* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetBezierCurveType", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "SetBezierCurveType", 1, "elfBezierCurve");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetBezierCurveType", 2, "number");
    }
    arg0 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetBezierCurveType(arg0, arg1);
    return 0;
}
static int lua_GetBezierCurveType(lua_State* L)
{
    int result;
    elfBezierCurve* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBezierCurveType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "GetBezierCurveType", 1, "elfBezierCurve");
    }
    arg0 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBezierCurveType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddBezierCurvePoint(lua_State* L)
{
    elfBezierCurve* arg0;
    elfBezierPoint* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddBezierCurvePoint", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "AddBezierCurvePoint", 1, "elfBezierCurve");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_BEZIER_POINT)
    {
        return lua_fail_arg(L, "AddBezierCurvePoint", 2, "elfBezierPoint");
    }
    arg0 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfBezierPoint*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddBezierCurvePoint(arg0, arg1);
    return 0;
}
static int lua_GetPointFromBezierCurve(lua_State* L)
{
    elfBezierPoint* result;
    elfBezierCurve* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetPointFromBezierCurve", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "GetPointFromBezierCurve", 1, "elfBezierCurve");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetPointFromBezierCurve", 2, "number");
    }
    arg0 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetPointFromBezierCurve(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBezierCurveValue(lua_State* L)
{
    float result;
    elfBezierCurve* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBezierCurveValue", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "GetBezierCurveValue", 1, "elfBezierCurve");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetBezierCurveValue", 2, "number");
    }
    arg0 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfGetBezierCurveValue(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_CreateIpo(lua_State* L)
{
    elfIpo* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateIpo", lua_gettop(L), 0);
    }
    result = elfCreateIpo();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_AddIpoCurve(lua_State* L)
{
    unsigned char result;
    elfIpo* arg0;
    elfBezierCurve* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddIpoCurve", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "AddIpoCurve", 1, "elfIpo");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_BEZIER_CURVE)
    {
        return lua_fail_arg(L, "AddIpoCurve", 2, "elfBezierCurve");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfBezierCurve*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfAddIpoCurve(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetIpoCurveCount(lua_State* L)
{
    int result;
    elfIpo* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetIpoCurveCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetIpoCurveCount", 1, "elfIpo");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetIpoCurveCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetCurveFromIpo(lua_State* L)
{
    elfBezierCurve* result;
    elfIpo* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetCurveFromIpo", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetCurveFromIpo", 1, "elfIpo");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetCurveFromIpo", 2, "number");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetCurveFromIpo(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetIpoLoc(lua_State* L)
{
    elfVec3f result;
    elfIpo* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetIpoLoc", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetIpoLoc", 1, "elfIpo");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetIpoLoc", 2, "number");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfGetIpoLoc(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetIpoRot(lua_State* L)
{
    elfVec3f result;
    elfIpo* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetIpoRot", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetIpoRot", 1, "elfIpo");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetIpoRot", 2, "number");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfGetIpoRot(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetIpoScale(lua_State* L)
{
    elfVec3f result;
    elfIpo* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetIpoScale", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetIpoScale", 1, "elfIpo");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetIpoScale", 2, "number");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfGetIpoScale(arg0, arg1);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetIpoQua(lua_State* L)
{
    elfVec4f result;
    elfIpo* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetIpoQua", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "GetIpoQua", 1, "elfIpo");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetIpoQua", 2, "number");
    }
    arg0 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfGetIpoQua(arg0, arg1);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_CreateProperty(lua_State* L)
{
    elfProperty* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateProperty", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateProperty", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateProperty(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetPropertyType(lua_State* L)
{
    int result;
    elfProperty* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPropertyType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "GetPropertyType", 1, "elfProperty");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPropertyType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetPropertyInt(lua_State* L)
{
    int result;
    elfProperty* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPropertyInt", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "GetPropertyInt", 1, "elfProperty");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPropertyInt(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetPropertyFloat(lua_State* L)
{
    float result;
    elfProperty* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPropertyFloat", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "GetPropertyFloat", 1, "elfProperty");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPropertyFloat(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetPropertyString(lua_State* L)
{
    const char* result;
    elfProperty* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPropertyString", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "GetPropertyString", 1, "elfProperty");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPropertyString(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetPropertyBool(lua_State* L)
{
    unsigned char result;
    elfProperty* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPropertyBool", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "GetPropertyBool", 1, "elfProperty");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPropertyBool(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetPropertyInt(lua_State* L)
{
    elfProperty* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetPropertyInt", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "SetPropertyInt", 1, "elfProperty");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetPropertyInt", 2, "number");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetPropertyInt(arg0, arg1);
    return 0;
}
static int lua_SetPropertyFloat(lua_State* L)
{
    elfProperty* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetPropertyFloat", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "SetPropertyFloat", 1, "elfProperty");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetPropertyFloat", 2, "number");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetPropertyFloat(arg0, arg1);
    return 0;
}
static int lua_SetPropertyString(lua_State* L)
{
    elfProperty* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetPropertyString", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "SetPropertyString", 1, "elfProperty");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetPropertyString", 2, "string");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetPropertyString(arg0, arg1);
    return 0;
}
static int lua_SetPropertyBool(lua_State* L)
{
    elfProperty* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetPropertyBool", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "SetPropertyBool", 1, "elfProperty");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetPropertyBool", 2, "boolean");
    }
    arg0 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetPropertyBool(arg0, arg1);
    return 0;
}
static int lua_GetActorName(lua_State* L)
{
    const char* result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorName", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetActorFilePath(lua_State* L)
{
    const char* result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorFilePath", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetActorScript(lua_State* L)
{
    elfScript* result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorScript", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorScript", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorScript(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetActorName(lua_State* L)
{
    elfActor* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorName", 1, "elfActor");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetActorName", 2, "string");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetActorName(arg0, arg1);
    return 0;
}
static int lua_SetActorScript(lua_State* L)
{
    elfActor* arg0;
    elfScript* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorScript", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorScript", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "SetActorScript", 2, "elfScript");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetActorScript(arg0, arg1);
    return 0;
}
static int lua_ClearActorScript(lua_State* L)
{
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearActorScript", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "ClearActorScript", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearActorScript(arg0);
    return 0;
}
static int lua_SetActorPosition(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorPosition", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorPosition", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorPosition", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorPosition", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorPosition", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorPosition(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorRotation(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorRotation", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorRotation", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorRotation", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorRotation", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorRotation", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorRotation(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorOrientation(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetActorOrientation", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorOrientation", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorOrientation", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorOrientation", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorOrientation", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetActorOrientation", 5, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetActorOrientation(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_RotateActor(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "RotateActor", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RotateActor", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RotateActor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "RotateActor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "RotateActor", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfRotateActor(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_RotateActorLocal(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "RotateActorLocal", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RotateActorLocal", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RotateActorLocal", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "RotateActorLocal", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "RotateActorLocal", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfRotateActorLocal(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_MoveActor(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "MoveActor", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "MoveActor", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "MoveActor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "MoveActor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "MoveActor", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfMoveActor(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_MoveActorLocal(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "MoveActorLocal", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "MoveActorLocal", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "MoveActorLocal", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "MoveActorLocal", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "MoveActorLocal", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfMoveActorLocal(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorPositionRelativeTo(lua_State* L)
{
    elfActor* arg0;
    elfActor* arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetActorPositionRelativeTo", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorPositionRelativeTo", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "SetActorPositionRelativeTo", 2, "elfActor");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorPositionRelativeTo", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorPositionRelativeTo", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetActorPositionRelativeTo", 5, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetActorPositionRelativeTo(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetActorRotationRelativeTo(lua_State* L)
{
    elfActor* arg0;
    elfActor* arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetActorRotationRelativeTo", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorRotationRelativeTo", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "SetActorRotationRelativeTo", 2, "elfActor");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorRotationRelativeTo", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorRotationRelativeTo", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetActorRotationRelativeTo", 5, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetActorRotationRelativeTo(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetActorOrientationRelativeTo(lua_State* L)
{
    elfActor* arg0;
    elfActor* arg1;
    float arg2;
    float arg3;
    float arg4;
    float arg5;
    if (lua_gettop(L) != 6)
    {
        return lua_fail_arg_count(L, "SetActorOrientationRelativeTo", lua_gettop(L), 6);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 2, "elfActor");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "SetActorOrientationRelativeTo", 6, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    elfSetActorOrientationRelativeTo(arg0, arg1, arg2, arg3, arg4, arg5);
    return 0;
}
static int lua_GetActorPosition(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorPosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorPosition", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorPosition(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorRotation(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorRotation", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorRotation", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorRotation(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorOrientation(lua_State* L)
{
    elfVec4f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorOrientation", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorOrientation", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorOrientation(arg0);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_SetActorPhysics(lua_State* L)
{
    elfActor* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorPhysics", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorPhysics", 1, "elfActor");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetActorPhysics", 2, "boolean");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetActorPhysics(arg0, arg1);
    return 0;
}
static int lua_SetActorShape(lua_State* L)
{
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorShape", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorShape", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorShape", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetActorShape(arg0, arg1);
    return 0;
}
static int lua_SetActorBoundingLengths(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorBoundingLengths", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorBoundingLengths", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorBoundingLengths", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorBoundingLengths", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorBoundingLengths", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorBoundingLengths(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorBoundingOffset(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorBoundingOffset", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorBoundingOffset", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorBoundingOffset", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorBoundingOffset", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorBoundingOffset", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorBoundingOffset(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorMass(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorMass", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorMass", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorMass", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetActorMass(arg0, arg1);
    return 0;
}
static int lua_SetActorDamping(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetActorDamping", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorDamping", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorDamping", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorDamping", 3, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetActorDamping(arg0, arg1, arg2);
    return 0;
}
static int lua_SetActorSleep(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetActorSleep", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorSleep", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorSleep", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorSleep", 3, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetActorSleep(arg0, arg1, arg2);
    return 0;
}
static int lua_SetActorRestitution(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorRestitution", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorRestitution", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorRestitution", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetActorRestitution(arg0, arg1);
    return 0;
}
static int lua_SetActorAnisotropicFriction(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorAnisotropicFriction", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorAnisotropicFriction", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorAnisotropicFriction", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorAnisotropicFriction", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorAnisotropicFriction", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorAnisotropicFriction(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorLinearFactor(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorLinearFactor", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorLinearFactor", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorLinearFactor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorLinearFactor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorLinearFactor", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorLinearFactor(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorAngularFactor(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorAngularFactor", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorAngularFactor", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorAngularFactor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorAngularFactor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorAngularFactor", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorAngularFactor(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_AddActorForce(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "AddActorForce", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorForce", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "AddActorForce", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "AddActorForce", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorForce", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfAddActorForce(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_AddActorForceLocal(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "AddActorForceLocal", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorForceLocal", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "AddActorForceLocal", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "AddActorForceLocal", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorForceLocal", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfAddActorForceLocal(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_AddActorTorque(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "AddActorTorque", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorTorque", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "AddActorTorque", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "AddActorTorque", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorTorque", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfAddActorTorque(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorLinearVelocity(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorLinearVelocity", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorLinearVelocity", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorLinearVelocity", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorLinearVelocity", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorLinearVelocity", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorLinearVelocity(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorLinearVelocityLocal(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorLinearVelocityLocal", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorLinearVelocityLocal", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorLinearVelocityLocal", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorLinearVelocityLocal", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorLinearVelocityLocal", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorLinearVelocityLocal(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetActorAngularVelocity(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetActorAngularVelocity", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorAngularVelocity", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorAngularVelocity", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetActorAngularVelocity", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetActorAngularVelocity", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetActorAngularVelocity(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_GetActorPhysics(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorPhysics", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorPhysics", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorPhysics(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetActorBoundingLengths(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorBoundingLengths", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorBoundingLengths", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorBoundingLengths(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorBoundingOffset(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorBoundingOffset", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorBoundingOffset", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorBoundingOffset(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorShape(lua_State* L)
{
    int result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorShape", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorShape", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorShape(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorMass(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorMass", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorMass", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorMass(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorLinearDamping(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorLinearDamping", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorLinearDamping", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorLinearDamping(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorAngularDamping(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorAngularDamping", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorAngularDamping", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorAngularDamping(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorLinearSleep(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorLinearSleep", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorLinearSleep", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorLinearSleep(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorAngularSleep(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorAngularSleep", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorAngularSleep", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorAngularSleep(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorRestitution(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorRestitution", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorRestitution", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorRestitution(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorAnisotropicFriction(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorAnisotropicFriction", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorAnisotropicFriction", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorAnisotropicFriction(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorLinearFactor(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorLinearFactor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorLinearFactor", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorLinearFactor(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorAngularFactor(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorAngularFactor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorAngularFactor", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorAngularFactor(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorLinearVelocity(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorLinearVelocity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorLinearVelocity", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorLinearVelocity(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetActorAngularVelocity(lua_State* L)
{
    elfVec3f result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorAngularVelocity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorAngularVelocity", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorAngularVelocity(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_AddActorHingeJoint(lua_State* L)
{
    elfJoint* result;
    elfActor* arg0;
    elfActor* arg1;
    const char* arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    float arg7;
    float arg8;
    if (lua_gettop(L) != 9)
    {
        return lua_fail_arg_count(L, "AddActorHingeJoint", lua_gettop(L), 9);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 2, "elfActor");
    }
    if (!lua_isstring(L, 3))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 3, "string");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 7, "number");
    }
    if (!lua_isnumber(L, 8))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 8, "number");
    }
    if (!lua_isnumber(L, 9))
    {
        return lua_fail_arg(L, "AddActorHingeJoint", 9, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = lua_tostring(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    arg7 = (float)lua_tonumber(L, 8);
    arg8 = (float)lua_tonumber(L, 9);
    result = elfAddActorHingeJoint(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_AddActorBallJoint(lua_State* L)
{
    elfJoint* result;
    elfActor* arg0;
    elfActor* arg1;
    const char* arg2;
    float arg3;
    float arg4;
    float arg5;
    if (lua_gettop(L) != 6)
    {
        return lua_fail_arg_count(L, "AddActorBallJoint", lua_gettop(L), 6);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 2, "elfActor");
    }
    if (!lua_isstring(L, 3))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 3, "string");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "AddActorBallJoint", 6, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = lua_tostring(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    result = elfAddActorBallJoint(arg0, arg1, arg2, arg3, arg4, arg5);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_AddActorConeTwistJoint(lua_State* L)
{
    elfJoint* result;
    elfActor* arg0;
    elfActor* arg1;
    const char* arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    float arg7;
    float arg8;
    if (lua_gettop(L) != 9)
    {
        return lua_fail_arg_count(L, "AddActorConeTwistJoint", lua_gettop(L), 9);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 2, "elfActor");
    }
    if (!lua_isstring(L, 3))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 3, "string");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 7, "number");
    }
    if (!lua_isnumber(L, 8))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 8, "number");
    }
    if (!lua_isnumber(L, 9))
    {
        return lua_fail_arg(L, "AddActorConeTwistJoint", 9, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = lua_tostring(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    arg7 = (float)lua_tonumber(L, 8);
    arg8 = (float)lua_tonumber(L, 9);
    result = elfAddActorConeTwistJoint(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetActorJoint(lua_State* L)
{
    elfJoint* result;
    elfActor* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetActorJoint", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorJoint", 1, "elfActor");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetActorJoint", 2, "string");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetActorJoint(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetActorJointByIndex(lua_State* L)
{
    elfJoint* result;
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetActorJointByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorJointByIndex", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetActorJointByIndex", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetActorJointByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_RemoveActorJoint(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorJoint", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorJoint", 1, "elfActor");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveActorJoint", 2, "string");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveActorJoint(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveActorJointByIndex(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorJointByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorJointByIndex", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveActorJointByIndex", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveActorJointByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveActorJointByObject(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    elfJoint* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorJointByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorJointByObject", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "RemoveActorJointByObject", 2, "elfJoint");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveActorJointByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetActorIpo(lua_State* L)
{
    elfActor* arg0;
    elfIpo* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorIpo", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorIpo", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_IPO)
    {
        return lua_fail_arg(L, "SetActorIpo", 2, "elfIpo");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfIpo*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetActorIpo(arg0, arg1);
    return 0;
}
static int lua_GetActorIpo(lua_State* L)
{
    elfIpo* result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorIpo", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorIpo", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorIpo(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetActorIpoFrame(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorIpoFrame", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorIpoFrame", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetActorIpoFrame", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetActorIpoFrame(arg0, arg1);
    return 0;
}
static int lua_PlayActorIpo(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "PlayActorIpo", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "PlayActorIpo", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "PlayActorIpo", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "PlayActorIpo", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "PlayActorIpo", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfPlayActorIpo(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_LoopActorIpo(lua_State* L)
{
    elfActor* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "LoopActorIpo", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "LoopActorIpo", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "LoopActorIpo", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "LoopActorIpo", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "LoopActorIpo", 4, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfLoopActorIpo(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_StopActorIpo(lua_State* L)
{
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "StopActorIpo", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "StopActorIpo", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfStopActorIpo(arg0);
    return 0;
}
static int lua_PauseActorIpo(lua_State* L)
{
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "PauseActorIpo", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "PauseActorIpo", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfPauseActorIpo(arg0);
    return 0;
}
static int lua_ResumeActorIpo(lua_State* L)
{
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ResumeActorIpo", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "ResumeActorIpo", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfResumeActorIpo(arg0);
    return 0;
}
static int lua_GetActorIpoStart(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorIpoStart", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorIpoStart", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorIpoStart(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorIpoEnd(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorIpoEnd", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorIpoEnd", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorIpoEnd(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorIpoSpeed(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorIpoSpeed", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorIpoSpeed", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorIpoSpeed(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorIpoFrame(lua_State* L)
{
    float result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorIpoFrame", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorIpoFrame", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorIpoFrame(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_IsActorIpoPlaying(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsActorIpoPlaying", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "IsActorIpoPlaying", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsActorIpoPlaying(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsActorIpoPaused(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsActorIpoPaused", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "IsActorIpoPaused", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsActorIpoPaused(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetActorCollisionCount(lua_State* L)
{
    int result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorCollisionCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorCollisionCount", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorCollisionCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetActorCollision(lua_State* L)
{
    elfCollision* result;
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetActorCollision", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorCollision", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetActorCollision", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetActorCollision(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetActorPropertyCount(lua_State* L)
{
    int result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorPropertyCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorPropertyCount", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorPropertyCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddPropertyToActor(lua_State* L)
{
    elfActor* arg0;
    elfProperty* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddPropertyToActor", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "AddPropertyToActor", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "AddPropertyToActor", 2, "elfProperty");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddPropertyToActor(arg0, arg1);
    return 0;
}
static int lua_GetActorPropertyByName(lua_State* L)
{
    elfProperty* result;
    elfActor* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetActorPropertyByName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorPropertyByName", 1, "elfActor");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetActorPropertyByName", 2, "string");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetActorPropertyByName(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetActorPropertyByIndex(lua_State* L)
{
    elfProperty* result;
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetActorPropertyByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorPropertyByIndex", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetActorPropertyByIndex", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetActorPropertyByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_RemoveActorPropertyByName(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorPropertyByName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorPropertyByName", 1, "elfActor");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveActorPropertyByName", 2, "string");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveActorPropertyByName(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveActorPropertyByIndex(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorPropertyByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorPropertyByIndex", 1, "elfActor");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveActorPropertyByIndex", 2, "number");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveActorPropertyByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveActorPropertyByObject(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    elfProperty* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveActorPropertyByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorPropertyByObject", 1, "elfActor");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_PROPERTY)
    {
        return lua_fail_arg(L, "RemoveActorPropertyByObject", 2, "elfProperty");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfProperty*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveActorPropertyByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveActorProperties(lua_State* L)
{
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "RemoveActorProperties", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "RemoveActorProperties", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfRemoveActorProperties(arg0);
    return 0;
}
static int lua_SetActorSelected(lua_State* L)
{
    elfActor* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetActorSelected", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetActorSelected", 1, "elfActor");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetActorSelected", 2, "boolean");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetActorSelected(arg0, arg1);
    return 0;
}
static int lua_GetActorSelected(lua_State* L)
{
    unsigned char result;
    elfActor* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetActorSelected", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetActorSelected", 1, "elfActor");
    }
    arg0 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetActorSelected(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_CreateCamera(lua_State* L)
{
    elfCamera* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateCamera", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateCamera", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateCamera(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetCameraViewport(lua_State* L)
{
    elfCamera* arg0;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetCameraViewport", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraViewport", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraViewport", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetCameraViewport", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetCameraViewport", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetCameraViewport", 5, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    elfSetCameraViewport(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetCameraOrthoViewport(lua_State* L)
{
    elfCamera* arg0;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetCameraOrthoViewport", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraOrthoViewport", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraOrthoViewport", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetCameraOrthoViewport", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetCameraOrthoViewport", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetCameraOrthoViewport", 5, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    elfSetCameraOrthoViewport(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetCameraMode(lua_State* L)
{
    elfCamera* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCameraMode", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraMode", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraMode", 2, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetCameraMode(arg0, arg1);
    return 0;
}
static int lua_SetCameraFov(lua_State* L)
{
    elfCamera* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCameraFov", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraFov", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraFov", 2, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetCameraFov(arg0, arg1);
    return 0;
}
static int lua_SetCameraAspect(lua_State* L)
{
    elfCamera* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCameraAspect", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraAspect", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraAspect", 2, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetCameraAspect(arg0, arg1);
    return 0;
}
static int lua_SetCameraClip(lua_State* L)
{
    elfCamera* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetCameraClip", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetCameraClip", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetCameraClip", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetCameraClip", 3, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetCameraClip(arg0, arg1, arg2);
    return 0;
}
static int lua_GetCameraViewportSize(lua_State* L)
{
    elfVec2i result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraViewportSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraViewportSize", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraViewportSize(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetCameraViewportOffset(lua_State* L)
{
    elfVec2i result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraViewportOffset", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraViewportOffset", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraViewportOffset(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetCameraOrthoViewportSize(lua_State* L)
{
    elfVec2i result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraOrthoViewportSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraOrthoViewportSize", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraOrthoViewportSize(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetCameraOrthoViewportOffset(lua_State* L)
{
    elfVec2i result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraOrthoViewportOffset", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraOrthoViewportOffset", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraOrthoViewportOffset(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetCameraMode(lua_State* L)
{
    int result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraMode", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraMode", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraMode(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetCameraFov(lua_State* L)
{
    float result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraFov", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraFov", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraFov(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetCameraAspect(lua_State* L)
{
    float result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraAspect", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraAspect", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraAspect(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetCameraClip(lua_State* L)
{
    elfVec2f result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraClip", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraClip", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraClip(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetCameraFarPlaneSize(lua_State* L)
{
    elfVec2f result;
    elfCamera* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCameraFarPlaneSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "GetCameraFarPlaneSize", 1, "elfCamera");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCameraFarPlaneSize(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_ProjectCameraPoint(lua_State* L)
{
    elfVec3f result;
    elfCamera* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "ProjectCameraPoint", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "ProjectCameraPoint", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "ProjectCameraPoint", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "ProjectCameraPoint", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "ProjectCameraPoint", 4, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfProjectCameraPoint(arg0, arg1, arg2, arg3);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_UnProjectCameraPoint(lua_State* L)
{
    elfVec3f result;
    elfCamera* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "UnProjectCameraPoint", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "UnProjectCameraPoint", 1, "elfCamera");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "UnProjectCameraPoint", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "UnProjectCameraPoint", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "UnProjectCameraPoint", 4, "number");
    }
    arg0 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    result = elfUnProjectCameraPoint(arg0, arg1, arg2, arg3);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_CreateVertex(lua_State* L)
{
    elfVertex* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateVertex", lua_gettop(L), 0);
    }
    result = elfCreateVertex();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetVertexPosition(lua_State* L)
{
    elfVertex* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetVertexPosition", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "SetVertexPosition", 1, "elfVertex");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetVertexPosition", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetVertexPosition", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetVertexPosition", 4, "number");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetVertexPosition(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetVertexNormal(lua_State* L)
{
    elfVertex* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetVertexNormal", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "SetVertexNormal", 1, "elfVertex");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetVertexNormal", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetVertexNormal", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetVertexNormal", 4, "number");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetVertexNormal(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetVertexTexCoord(lua_State* L)
{
    elfVertex* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetVertexTexCoord", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "SetVertexTexCoord", 1, "elfVertex");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetVertexTexCoord", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetVertexTexCoord", 3, "number");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetVertexTexCoord(arg0, arg1, arg2);
    return 0;
}
static int lua_GetVertexPosition(lua_State* L)
{
    elfVec3f result;
    elfVertex* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVertexPosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "GetVertexPosition", 1, "elfVertex");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetVertexPosition(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetVertexNormal(lua_State* L)
{
    elfVec3f result;
    elfVertex* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVertexNormal", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "GetVertexNormal", 1, "elfVertex");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetVertexNormal(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetVertexTexCoord(lua_State* L)
{
    elfVec2f result;
    elfVertex* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetVertexTexCoord", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "GetVertexTexCoord", 1, "elfVertex");
    }
    arg0 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetVertexTexCoord(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetFaceV1(lua_State* L)
{
    int result;
    elfFace* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFaceV1", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FACE)
    {
        return lua_fail_arg(L, "GetFaceV1", 1, "elfFace");
    }
    arg0 = (elfFace*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFaceV1(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFaceV2(lua_State* L)
{
    int result;
    elfFace* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFaceV2", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FACE)
    {
        return lua_fail_arg(L, "GetFaceV2", 1, "elfFace");
    }
    arg0 = (elfFace*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFaceV2(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFaceV3(lua_State* L)
{
    int result;
    elfFace* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFaceV3", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FACE)
    {
        return lua_fail_arg(L, "GetFaceV3", 1, "elfFace");
    }
    arg0 = (elfFace*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFaceV3(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_CreateMeshData(lua_State* L)
{
    elfMeshData* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "CreateMeshData", lua_gettop(L), 0);
    }
    result = elfCreateMeshData();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetMeshDataVertexCount(lua_State* L)
{
    int result;
    elfMeshData* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMeshDataVertexCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "GetMeshDataVertexCount", 1, "elfMeshData");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMeshDataVertexCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetMeshDataFaceCount(lua_State* L)
{
    int result;
    elfMeshData* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetMeshDataFaceCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "GetMeshDataFaceCount", 1, "elfMeshData");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetMeshDataFaceCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddMeshDataVertex(lua_State* L)
{
    elfMeshData* arg0;
    elfVertex* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddMeshDataVertex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "AddMeshDataVertex", 1, "elfMeshData");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_VERTEX)
    {
        return lua_fail_arg(L, "AddMeshDataVertex", 2, "elfVertex");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfVertex*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddMeshDataVertex(arg0, arg1);
    return 0;
}
static int lua_AddMeshDataFace(lua_State* L)
{
    elfMeshData* arg0;
    int arg1;
    int arg2;
    int arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "AddMeshDataFace", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "AddMeshDataFace", 1, "elfMeshData");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "AddMeshDataFace", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "AddMeshDataFace", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "AddMeshDataFace", 4, "number");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    elfAddMeshDataFace(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_GetVertexFromMeshData(lua_State* L)
{
    elfVertex* result;
    elfMeshData* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetVertexFromMeshData", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "GetVertexFromMeshData", 1, "elfMeshData");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetVertexFromMeshData", 2, "number");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetVertexFromMeshData(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetFaceFromMeshData(lua_State* L)
{
    elfFace* result;
    elfMeshData* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetFaceFromMeshData", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "GetFaceFromMeshData", 1, "elfMeshData");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetFaceFromMeshData", 2, "number");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetFaceFromMeshData(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateModelFromMeshData(lua_State* L)
{
    elfModel* result;
    elfMeshData* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateModelFromMeshData", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MESH_DATA)
    {
        return lua_fail_arg(L, "CreateModelFromMeshData", 1, "elfMeshData");
    }
    arg0 = (elfMeshData*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfCreateModelFromMeshData(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetModelName(lua_State* L)
{
    elfModel* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetModelName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "SetModelName", 1, "elfModel");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetModelName", 2, "string");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetModelName(arg0, arg1);
    return 0;
}
static int lua_GetModelName(lua_State* L)
{
    const char* result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelName", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetModelFilePath(lua_State* L)
{
    const char* result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelFilePath", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetModelVertexCount(lua_State* L)
{
    int result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelVertexCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelVertexCount", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelVertexCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetModelIndiceCount(lua_State* L)
{
    int result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelIndiceCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelIndiceCount", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelIndiceCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetModelBoundingBoxMin(lua_State* L)
{
    elfVec3f result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelBoundingBoxMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelBoundingBoxMin", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelBoundingBoxMin(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetModelBoundingBoxMax(lua_State* L)
{
    elfVec3f result;
    elfModel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetModelBoundingBoxMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "GetModelBoundingBoxMax", 1, "elfModel");
    }
    arg0 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetModelBoundingBoxMax(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_CreateEntity(lua_State* L)
{
    elfEntity* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateEntity", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateEntity", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateEntity(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GenerateEntityTangents(lua_State* L)
{
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GenerateEntityTangents", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GenerateEntityTangents", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfGenerateEntityTangents(arg0);
    return 0;
}
static int lua_SetEntityScale(lua_State* L)
{
    elfEntity* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetEntityScale", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityScale", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetEntityScale", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetEntityScale", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetEntityScale", 4, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetEntityScale(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_GetEntityScale(lua_State* L)
{
    elfVec3f result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityScale", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityScale", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityScale(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_SetEntityModel(lua_State* L)
{
    elfEntity* arg0;
    elfModel* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetEntityModel", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityModel", 1, "elfEntity");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "SetEntityModel", 2, "elfModel");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetEntityModel(arg0, arg1);
    return 0;
}
static int lua_ClearEntityModel(lua_State* L)
{
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearEntityModel", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "ClearEntityModel", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearEntityModel(arg0);
    return 0;
}
static int lua_GetEntityModel(lua_State* L)
{
    elfModel* result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityModel", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityModel", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityModel(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetEntityMaterialCount(lua_State* L)
{
    int result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityMaterialCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityMaterialCount", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityMaterialCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddEntityMaterial(lua_State* L)
{
    elfEntity* arg0;
    elfMaterial* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddEntityMaterial", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "AddEntityMaterial", 1, "elfEntity");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "AddEntityMaterial", 2, "elfMaterial");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddEntityMaterial(arg0, arg1);
    return 0;
}
static int lua_SetEntityMaterial(lua_State* L)
{
    elfEntity* arg0;
    int arg1;
    elfMaterial* arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetEntityMaterial", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityMaterial", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetEntityMaterial", 2, "number");
    }
    if (!lua_isuserdata(L, 3) || ((lua_elf_userdata*)lua_touserdata(L, 3))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 3))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetEntityMaterial", 3, "elfMaterial");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 3))->object;
    elfSetEntityMaterial(arg0, arg1, arg2);
    return 0;
}
static int lua_RemoveEntityMaterial(lua_State* L)
{
    elfEntity* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveEntityMaterial", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "RemoveEntityMaterial", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveEntityMaterial", 2, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfRemoveEntityMaterial(arg0, arg1);
    return 0;
}
static int lua_GetEntityMaterial(lua_State* L)
{
    elfMaterial* result;
    elfEntity* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetEntityMaterial", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityMaterial", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetEntityMaterial", 2, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetEntityMaterial(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetEntityVisible(lua_State* L)
{
    elfEntity* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetEntityVisible", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityVisible", 1, "elfEntity");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetEntityVisible", 2, "boolean");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetEntityVisible(arg0, arg1);
    return 0;
}
static int lua_GetEntityVisible(lua_State* L)
{
    unsigned char result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityVisible", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityVisible", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityVisible(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetEntityOccluder(lua_State* L)
{
    elfEntity* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetEntityOccluder", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityOccluder", 1, "elfEntity");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetEntityOccluder", 2, "boolean");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetEntityOccluder(arg0, arg1);
    return 0;
}
static int lua_GetEntityOccluder(lua_State* L)
{
    unsigned char result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityOccluder", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityOccluder", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityOccluder(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    elfArmature* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetEntityArmature", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityArmature", 1, "elfEntity");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ARMATURE)
    {
        return lua_fail_arg(L, "SetEntityArmature", 2, "elfArmature");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfArmature*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetEntityArmature(arg0, arg1);
    return 0;
}
static int lua_SetEntityArmatureFrame(lua_State* L)
{
    elfEntity* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetEntityArmatureFrame", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetEntityArmatureFrame", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetEntityArmatureFrame", 2, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetEntityArmatureFrame(arg0, arg1);
    return 0;
}
static int lua_PlayEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "PlayEntityArmature", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "PlayEntityArmature", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "PlayEntityArmature", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "PlayEntityArmature", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "PlayEntityArmature", 4, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfPlayEntityArmature(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_LoopEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "LoopEntityArmature", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "LoopEntityArmature", 1, "elfEntity");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "LoopEntityArmature", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "LoopEntityArmature", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "LoopEntityArmature", 4, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfLoopEntityArmature(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_StopEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "StopEntityArmature", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "StopEntityArmature", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfStopEntityArmature(arg0);
    return 0;
}
static int lua_PauseEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "PauseEntityArmature", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "PauseEntityArmature", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfPauseEntityArmature(arg0);
    return 0;
}
static int lua_ResumeEntityArmature(lua_State* L)
{
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ResumeEntityArmature", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "ResumeEntityArmature", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfResumeEntityArmature(arg0);
    return 0;
}
static int lua_GetEntityArmatureStart(lua_State* L)
{
    float result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityArmatureStart", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityArmatureStart", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityArmatureStart(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetEntityArmatureEnd(lua_State* L)
{
    float result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityArmatureEnd", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityArmatureEnd", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityArmatureEnd(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetEntityArmatureSpeed(lua_State* L)
{
    float result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityArmatureSpeed", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityArmatureSpeed", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityArmatureSpeed(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetEntityArmatureFrame(lua_State* L)
{
    float result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityArmatureFrame", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityArmatureFrame", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityArmatureFrame(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_IsEntityArmaturePlaying(lua_State* L)
{
    unsigned char result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsEntityArmaturePlaying", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "IsEntityArmaturePlaying", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsEntityArmaturePlaying(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_IsEntityArmaturePaused(lua_State* L)
{
    unsigned char result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsEntityArmaturePaused", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "IsEntityArmaturePaused", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsEntityArmaturePaused(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetEntityArmature(lua_State* L)
{
    elfArmature* result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityArmature", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityArmature", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityArmature(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetEntityChanged(lua_State* L)
{
    unsigned char result;
    elfEntity* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetEntityChanged", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "GetEntityChanged", 1, "elfEntity");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetEntityChanged(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_CreateLight(lua_State* L)
{
    elfLight* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateLight", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateLight", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateLight(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetLightType(lua_State* L)
{
    elfLight* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightType", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightType", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightType", 2, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetLightType(arg0, arg1);
    return 0;
}
static int lua_SetLightColor(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetLightColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightColor", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetLightColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetLightColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetLightColor", 5, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetLightColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetLightRange(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetLightRange", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightRange", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightRange", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetLightRange", 3, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetLightRange(arg0, arg1, arg2);
    return 0;
}
static int lua_SetLightShadows(lua_State* L)
{
    elfLight* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightShadows", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightShadows", 1, "elfLight");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetLightShadows", 2, "boolean");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetLightShadows(arg0, arg1);
    return 0;
}
static int lua_SetLightVisible(lua_State* L)
{
    elfLight* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightVisible", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightVisible", 1, "elfLight");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetLightVisible", 2, "boolean");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetLightVisible(arg0, arg1);
    return 0;
}
static int lua_SetLightCone(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetLightCone", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightCone", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightCone", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetLightCone", 3, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetLightCone(arg0, arg1, arg2);
    return 0;
}
static int lua_SetLightShaft(lua_State* L)
{
    elfLight* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightShaft", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightShaft", 1, "elfLight");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetLightShaft", 2, "boolean");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetLightShaft(arg0, arg1);
    return 0;
}
static int lua_SetLightShaftSize(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightShaftSize", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightShaftSize", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightShaftSize", 2, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetLightShaftSize(arg0, arg1);
    return 0;
}
static int lua_SetLightShaftIntensity(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightShaftIntensity", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightShaftIntensity", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightShaftIntensity", 2, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetLightShaftIntensity(arg0, arg1);
    return 0;
}
static int lua_SetLightShaftFadeOff(lua_State* L)
{
    elfLight* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLightShaftFadeOff", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "SetLightShaftFadeOff", 1, "elfLight");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetLightShaftFadeOff", 2, "number");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetLightShaftFadeOff(arg0, arg1);
    return 0;
}
static int lua_GetLightType(lua_State* L)
{
    int result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightType", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightColor(lua_State* L)
{
    elfColor result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightColor", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetLightRange(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightRange", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightRange", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightRange(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightFadeRange(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightFadeRange", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightFadeRange", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightFadeRange(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightShadows(lua_State* L)
{
    unsigned char result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightShadows", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightShadows", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightShadows(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetLightVisible(lua_State* L)
{
    unsigned char result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightVisible", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightVisible", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightVisible(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetLightInnerCone(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightInnerCone", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightInnerCone", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightInnerCone(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightOuterCone(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightOuterCone", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightOuterCone", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightOuterCone(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightShaft(lua_State* L)
{
    unsigned char result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightShaft", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightShaft", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightShaft(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetLightShaftSize(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightShaftSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightShaftSize", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightShaftSize(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightShaftIntensity(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightShaftIntensity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightShaftIntensity", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightShaftIntensity(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetLightShaftFadeOff(lua_State* L)
{
    float result;
    elfLight* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLightShaftFadeOff", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "GetLightShaftFadeOff", 1, "elfLight");
    }
    arg0 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLightShaftFadeOff(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetBoneArmature(lua_State* L)
{
    elfArmature* result;
    elfBone* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBoneArmature", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneArmature", 1, "elfBone");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBoneArmature(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneParent(lua_State* L)
{
    elfBone* result;
    elfBone* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBoneParent", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneParent", 1, "elfBone");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBoneParent(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneChildByName(lua_State* L)
{
    elfBone* result;
    elfBone* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBoneChildByName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneChildByName", 1, "elfBone");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetBoneChildByName", 2, "string");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetBoneChildByName(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneChildById(lua_State* L)
{
    elfBone* result;
    elfBone* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBoneChildById", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneChildById", 1, "elfBone");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetBoneChildById", 2, "number");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetBoneChildById(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneChildByIndex(lua_State* L)
{
    elfBone* result;
    elfBone* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBoneChildByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneChildByIndex", 1, "elfBone");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetBoneChildByIndex", 2, "number");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetBoneChildByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBonePosition(lua_State* L)
{
    elfVec3f result;
    elfBone* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBonePosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBonePosition", 1, "elfBone");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBonePosition(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetBoneRotation(lua_State* L)
{
    elfVec3f result;
    elfBone* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBoneRotation", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneRotation", 1, "elfBone");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBoneRotation(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetBoneOrientation(lua_State* L)
{
    elfVec4f result;
    elfBone* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetBoneOrientation", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BONE)
    {
        return lua_fail_arg(L, "GetBoneOrientation", 1, "elfBone");
    }
    arg0 = (elfBone*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetBoneOrientation(arg0);
    lua_create_elfVec4f(L, result);
    return 1;
}
static int lua_CreateArmature(lua_State* L)
{
    elfArmature* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateArmature", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateArmature", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateArmature(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneFromArmatureByName(lua_State* L)
{
    elfBone* result;
    const char* arg0;
    elfArmature* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBoneFromArmatureByName", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "GetBoneFromArmatureByName", 1, "string");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ARMATURE)
    {
        return lua_fail_arg(L, "GetBoneFromArmatureByName", 2, "elfArmature");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = (elfArmature*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfGetBoneFromArmatureByName(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetBoneFromArmatureById(lua_State* L)
{
    elfBone* result;
    int arg0;
    elfArmature* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetBoneFromArmatureById", lua_gettop(L), 2);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "GetBoneFromArmatureById", 1, "number");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ARMATURE)
    {
        return lua_fail_arg(L, "GetBoneFromArmatureById", 2, "elfArmature");
    }
    arg0 = (int)lua_tonumber(L, 1);
    arg1 = (elfArmature*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfGetBoneFromArmatureById(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateParticles(lua_State* L)
{
    elfParticles* result;
    const char* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateParticles", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateParticles", 1, "string");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateParticles", 2, "number");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    result = elfCreateParticles(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetParticlesName(lua_State* L)
{
    const char* result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesName", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetParticlesFilePath(lua_State* L)
{
    const char* result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesFilePath", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_SetParticlesMaxCount(lua_State* L)
{
    elfParticles* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesMaxCount", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesMaxCount", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesMaxCount", 2, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetParticlesMaxCount(arg0, arg1);
    return 0;
}
static int lua_SetParticlesDrawMode(lua_State* L)
{
    elfParticles* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesDrawMode", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesDrawMode", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesDrawMode", 2, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetParticlesDrawMode(arg0, arg1);
    return 0;
}
static int lua_SetParticlesTexture(lua_State* L)
{
    elfParticles* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesTexture", 1, "elfParticles");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetParticlesTexture", 2, "elfTexture");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetParticlesTexture(arg0, arg1);
    return 0;
}
static int lua_ClearParticlesTexture(lua_State* L)
{
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearParticlesTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "ClearParticlesTexture", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearParticlesTexture(arg0);
    return 0;
}
static int lua_SetParticlesModel(lua_State* L)
{
    elfParticles* arg0;
    elfModel* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesModel", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesModel", 1, "elfParticles");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_MODEL)
    {
        return lua_fail_arg(L, "SetParticlesModel", 2, "elfModel");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfModel*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetParticlesModel(arg0, arg1);
    return 0;
}
static int lua_ClearParticlesModel(lua_State* L)
{
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearParticlesModel", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "ClearParticlesModel", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearParticlesModel(arg0);
    return 0;
}
static int lua_SetParticlesEntity(lua_State* L)
{
    elfParticles* arg0;
    elfEntity* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesEntity", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesEntity", 1, "elfParticles");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "SetParticlesEntity", 2, "elfEntity");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetParticlesEntity(arg0, arg1);
    return 0;
}
static int lua_ClearParticlesEntity(lua_State* L)
{
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ClearParticlesEntity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "ClearParticlesEntity", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfClearParticlesEntity(arg0);
    return 0;
}
static int lua_SetParticlesGravity(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetParticlesGravity", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesGravity", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesGravity", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesGravity", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesGravity", 4, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetParticlesGravity(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetParticlesSpawnCount(lua_State* L)
{
    elfParticles* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesSpawnCount", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesSpawnCount", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesSpawnCount", 2, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetParticlesSpawnCount(arg0, arg1);
    return 0;
}
static int lua_SetParticlesSpawn(lua_State* L)
{
    elfParticles* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetParticlesSpawn", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesSpawn", 1, "elfParticles");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesSpawn", 2, "boolean");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetParticlesSpawn(arg0, arg1);
    return 0;
}
static int lua_SetParticlesSize(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesSize", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesSize", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesSize(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesSizeGrowth(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesSizeGrowth", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesSizeGrowth", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesSizeGrowth", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesSizeGrowth", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesSizeGrowth(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesRotation(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesRotation", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesRotation", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesRotation", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesRotation", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesRotation(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesRotationGrowth(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesRotationGrowth", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesRotationGrowth", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesRotationGrowth", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesRotationGrowth", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesRotationGrowth(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesLifeSpan(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesLifeSpan", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesLifeSpan", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesLifeSpan", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesLifeSpan", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesLifeSpan(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesFadeSpeed(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetParticlesFadeSpeed", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesFadeSpeed", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesFadeSpeed", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesFadeSpeed", 3, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetParticlesFadeSpeed(arg0, arg1, arg2);
    return 0;
}
static int lua_SetParticlesPositionMin(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetParticlesPositionMin", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesPositionMin", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesPositionMin", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesPositionMin", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesPositionMin", 4, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetParticlesPositionMin(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetParticlesPositionMax(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetParticlesPositionMax", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesPositionMax", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesPositionMax", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesPositionMax", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesPositionMax", 4, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetParticlesPositionMax(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetParticlesVelocityMin(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetParticlesVelocityMin", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesVelocityMin", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMin", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMin", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMin", 4, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetParticlesVelocityMin(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetParticlesVelocityMax(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetParticlesVelocityMax", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesVelocityMax", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMax", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMax", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesVelocityMax", 4, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetParticlesVelocityMax(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_SetParticlesColorMin(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetParticlesColorMin", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesColorMin", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesColorMin", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesColorMin", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesColorMin", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetParticlesColorMin", 5, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetParticlesColorMin(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetParticlesColorMax(lua_State* L)
{
    elfParticles* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetParticlesColorMax", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "SetParticlesColorMax", 1, "elfParticles");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetParticlesColorMax", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetParticlesColorMax", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetParticlesColorMax", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetParticlesColorMax", 5, "number");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetParticlesColorMax(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_GetParticlesMaxCount(lua_State* L)
{
    int result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesMaxCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesMaxCount", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesMaxCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesCount(lua_State* L)
{
    int result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesCount", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesDrawMode(lua_State* L)
{
    int result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesDrawMode", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesDrawMode", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesDrawMode(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesTexture(lua_State* L)
{
    elfTexture* result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesTexture", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetParticlesModel(lua_State* L)
{
    elfModel* result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesModel", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesModel", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesModel(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetParticlesEntity(lua_State* L)
{
    elfEntity* result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesEntity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesEntity", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesEntity(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetParticlesGravity(lua_State* L)
{
    elfVec3f result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesGravity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesGravity", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesGravity(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetParticlesSpawnCount(lua_State* L)
{
    int result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSpawnCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSpawnCount", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSpawnCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesSpawn(lua_State* L)
{
    unsigned char result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSpawn", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSpawn", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSpawn(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetParticlesSizeMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSizeMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSizeMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSizeMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesSizeMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSizeMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSizeMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSizeMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesSizeGrowthMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSizeGrowthMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSizeGrowthMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSizeGrowthMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesSizeGrowthMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesSizeGrowthMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesSizeGrowthMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesSizeGrowthMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesRotationMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesRotationMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesRotationMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesRotationMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesRotationMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesRotationMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesRotationMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesRotationMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesRotationGrowthMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesRotationGrowthMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesRotationGrowthMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesRotationGrowthMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesRotationGrowthMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesRotationGrowthMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesRotationGrowthMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesRotationGrowthMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesLifeSpanMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesLifeSpanMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesLifeSpanMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesLifeSpanMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesLifeSpanMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesLifeSpanMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesLifeSpanMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesLifeSpanMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesFadeSpeedMin(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesFadeSpeedMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesFadeSpeedMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesFadeSpeedMin(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesFadeSpeedMax(lua_State* L)
{
    float result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesFadeSpeedMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesFadeSpeedMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesFadeSpeedMax(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetParticlesPositionMin(lua_State* L)
{
    elfVec3f result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesPositionMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesPositionMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesPositionMin(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetParticlesPositionMax(lua_State* L)
{
    elfVec3f result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesPositionMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesPositionMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesPositionMax(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetParticlesVelocityMin(lua_State* L)
{
    elfVec3f result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesVelocityMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesVelocityMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesVelocityMin(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetParticlesVelocityMax(lua_State* L)
{
    elfVec3f result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesVelocityMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesVelocityMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesVelocityMax(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetParticlesColorMin(lua_State* L)
{
    elfColor result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesColorMin", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesColorMin", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesColorMin(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetParticlesColorMax(lua_State* L)
{
    elfColor result;
    elfParticles* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetParticlesColorMax", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "GetParticlesColorMax", 1, "elfParticles");
    }
    arg0 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetParticlesColorMax(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_CreateSprite(lua_State* L)
{
    elfSprite* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateSprite", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateSprite", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateSprite(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetSpriteMaterial(lua_State* L)
{
    elfSprite* arg0;
    elfMaterial* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSpriteMaterial", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "SetSpriteMaterial", 1, "elfSprite");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_MATERIAL)
    {
        return lua_fail_arg(L, "SetSpriteMaterial", 2, "elfMaterial");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfMaterial*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetSpriteMaterial(arg0, arg1);
    return 0;
}
static int lua_SetSpriteScale(lua_State* L)
{
    elfSprite* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetSpriteScale", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "SetSpriteScale", 1, "elfSprite");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSpriteScale", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetSpriteScale", 3, "number");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetSpriteScale(arg0, arg1, arg2);
    return 0;
}
static int lua_SetSpriteFaceCamera(lua_State* L)
{
    elfSprite* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSpriteFaceCamera", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "SetSpriteFaceCamera", 1, "elfSprite");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSpriteFaceCamera", 2, "boolean");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSpriteFaceCamera(arg0, arg1);
    return 0;
}
static int lua_GetSpriteMaterial(lua_State* L)
{
    elfMaterial* result;
    elfSprite* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSpriteMaterial", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "GetSpriteMaterial", 1, "elfSprite");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSpriteMaterial(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSpriteScale(lua_State* L)
{
    elfVec2f result;
    elfSprite* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSpriteScale", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "GetSpriteScale", 1, "elfSprite");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSpriteScale(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_GetSpriteFaceCamera(lua_State* L)
{
    unsigned char result;
    elfSprite* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSpriteFaceCamera", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "GetSpriteFaceCamera", 1, "elfSprite");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSpriteFaceCamera(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSpriteVisible(lua_State* L)
{
    elfSprite* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSpriteVisible", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "SetSpriteVisible", 1, "elfSprite");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSpriteVisible", 2, "boolean");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSpriteVisible(arg0, arg1);
    return 0;
}
static int lua_GetSpriteVisible(lua_State* L)
{
    unsigned char result;
    elfSprite* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSpriteVisible", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "GetSpriteVisible", 1, "elfSprite");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSpriteVisible(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSpriteOccluder(lua_State* L)
{
    elfSprite* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSpriteOccluder", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "SetSpriteOccluder", 1, "elfSprite");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSpriteOccluder", 2, "boolean");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSpriteOccluder(arg0, arg1);
    return 0;
}
static int lua_GetSpriteOccluder(lua_State* L)
{
    unsigned char result;
    elfSprite* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSpriteOccluder", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "GetSpriteOccluder", 1, "elfSprite");
    }
    arg0 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSpriteOccluder(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetSceneName(lua_State* L)
{
    const char* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneName", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetSceneFilePath(lua_State* L)
{
    const char* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneFilePath", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_CreateScene(lua_State* L)
{
    elfScene* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateScene", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateScene", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateScene(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateSceneFromFile(lua_State* L)
{
    elfScene* result;
    const char* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateSceneFromFile", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateSceneFromFile", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateSceneFromFile", 2, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    result = elfCreateSceneFromFile(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SaveScene(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SaveScene", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SaveScene", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SaveScene", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfSaveScene(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetScenePhysics(lua_State* L)
{
    elfScene* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetScenePhysics", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetScenePhysics", 1, "elfScene");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetScenePhysics", 2, "boolean");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetScenePhysics(arg0, arg1);
    return 0;
}
static int lua_GetScenePhysics(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetScenePhysics", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetScenePhysics", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetScenePhysics(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSceneRunScripts(lua_State* L)
{
    elfScene* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneRunScripts", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneRunScripts", 1, "elfScene");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSceneRunScripts", 2, "boolean");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSceneRunScripts(arg0, arg1);
    return 0;
}
static int lua_GetSceneRunScripts(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneRunScripts", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneRunScripts", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneRunScripts(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSceneDebugDraw(lua_State* L)
{
    elfScene* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneDebugDraw", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneDebugDraw", 1, "elfScene");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSceneDebugDraw", 2, "boolean");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSceneDebugDraw(arg0, arg1);
    return 0;
}
static int lua_GetSceneDebugDraw(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneDebugDraw", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneDebugDraw", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneDebugDraw(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSceneOcclusionCulling(lua_State* L)
{
    elfScene* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneOcclusionCulling", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneOcclusionCulling", 1, "elfScene");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSceneOcclusionCulling", 2, "boolean");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSceneOcclusionCulling(arg0, arg1);
    return 0;
}
static int lua_GetSceneOcclusionCulling(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneOcclusionCulling", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneOcclusionCulling", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneOcclusionCulling(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetSceneGravity(lua_State* L)
{
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    if (lua_gettop(L) != 4)
    {
        return lua_fail_arg_count(L, "SetSceneGravity", lua_gettop(L), 4);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneGravity", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSceneGravity", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetSceneGravity", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetSceneGravity", 4, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    elfSetSceneGravity(arg0, arg1, arg2, arg3);
    return 0;
}
static int lua_GetSceneGravity(lua_State* L)
{
    elfVec3f result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneGravity", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneGravity", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneGravity(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_SetSceneAmbientColor(lua_State* L)
{
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetSceneAmbientColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneAmbientColor", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSceneAmbientColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetSceneAmbientColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetSceneAmbientColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetSceneAmbientColor", 5, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetSceneAmbientColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_GetSceneAmbientColor(lua_State* L)
{
    elfColor result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneAmbientColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneAmbientColor", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneAmbientColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_SetSceneFog(lua_State* L)
{
    elfScene* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneFog", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneFog", 1, "elfScene");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetSceneFog", 2, "boolean");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetSceneFog(arg0, arg1);
    return 0;
}
static int lua_SetSceneFogStart(lua_State* L)
{
    elfScene* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneFogStart", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneFogStart", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSceneFogStart", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetSceneFogStart(arg0, arg1);
    return 0;
}
static int lua_SetSceneFogEnd(lua_State* L)
{
    elfScene* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneFogEnd", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneFogEnd", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSceneFogEnd", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetSceneFogEnd(arg0, arg1);
    return 0;
}
static int lua_SetSceneFogColor(lua_State* L)
{
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetSceneFogColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneFogColor", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSceneFogColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetSceneFogColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetSceneFogColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetSceneFogColor", 5, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetSceneFogColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_GetSceneFog(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneFog", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneFog", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneFog(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetSceneFogStart(lua_State* L)
{
    float result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneFogStart", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneFogStart", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneFogStart(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFogEnd(lua_State* L)
{
    float result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFogEnd", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetFogEnd", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFogEnd(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetFogColor(lua_State* L)
{
    elfColor result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFogColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetFogColor", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFogColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetSceneCameraCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneCameraCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneCameraCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneCameraCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetSceneEntityCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneEntityCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneEntityCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneEntityCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetSceneLightCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneLightCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneLightCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneLightCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetSceneArmatureCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneArmatureCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneArmatureCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneArmatureCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetSceneParticlesCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneParticlesCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneParticlesCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneParticlesCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetSceneSpriteCount(lua_State* L)
{
    int result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneSpriteCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneSpriteCount", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneSpriteCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_AddSceneCamera(lua_State* L)
{
    elfScene* arg0;
    elfCamera* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddSceneCamera", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "AddSceneCamera", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "AddSceneCamera", 2, "elfCamera");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddSceneCamera(arg0, arg1);
    return 0;
}
static int lua_AddSceneEntity(lua_State* L)
{
    elfScene* arg0;
    elfEntity* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddSceneEntity", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "AddSceneEntity", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "AddSceneEntity", 2, "elfEntity");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddSceneEntity(arg0, arg1);
    return 0;
}
static int lua_AddSceneLight(lua_State* L)
{
    elfScene* arg0;
    elfLight* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddSceneLight", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "AddSceneLight", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "AddSceneLight", 2, "elfLight");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddSceneLight(arg0, arg1);
    return 0;
}
static int lua_AddSceneParticles(lua_State* L)
{
    elfScene* arg0;
    elfParticles* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddSceneParticles", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "AddSceneParticles", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "AddSceneParticles", 2, "elfParticles");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddSceneParticles(arg0, arg1);
    return 0;
}
static int lua_AddSceneSprite(lua_State* L)
{
    elfScene* arg0;
    elfSprite* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddSceneSprite", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "AddSceneSprite", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "AddSceneSprite", 2, "elfSprite");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfAddSceneSprite(arg0, arg1);
    return 0;
}
static int lua_SetSceneActiveCamera(lua_State* L)
{
    elfScene* arg0;
    elfCamera* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSceneActiveCamera", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "SetSceneActiveCamera", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "SetSceneActiveCamera", 2, "elfCamera");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetSceneActiveCamera(arg0, arg1);
    return 0;
}
static int lua_GetSceneActiveCamera(lua_State* L)
{
    elfCamera* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneActiveCamera", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneActiveCamera", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneActiveCamera(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneRayCastResult(lua_State* L)
{
    elfCollision* result;
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "GetSceneRayCastResult", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "GetSceneRayCastResult", 7, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    result = elfGetSceneRayCastResult(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneRayCastResults(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "GetSceneRayCastResults", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "GetSceneRayCastResults", 7, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    result = elfGetSceneRayCastResults(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetDebugSceneRayCastResult(lua_State* L)
{
    elfCollision* result;
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "GetDebugSceneRayCastResult", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResult", 7, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    result = elfGetDebugSceneRayCastResult(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetDebugSceneRayCastResults(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    float arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "GetDebugSceneRayCastResults", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "GetDebugSceneRayCastResults", 7, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    arg5 = (float)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    result = elfGetDebugSceneRayCastResults(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneCameraByIndex(lua_State* L)
{
    elfCamera* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneCameraByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneCameraByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneCameraByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneCameraByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneEntityByIndex(lua_State* L)
{
    elfEntity* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneEntityByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneEntityByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneEntityByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneEntityByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneLightByIndex(lua_State* L)
{
    elfLight* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneLightByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneLightByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneLightByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneLightByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneArmatureByIndex(lua_State* L)
{
    elfArmature* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneArmatureByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneArmatureByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneArmatureByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneArmatureByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneParticlesByIndex(lua_State* L)
{
    elfParticles* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneParticlesByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneParticlesByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneParticlesByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneParticlesByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneSpriteByIndex(lua_State* L)
{
    elfSprite* result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneSpriteByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneSpriteByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetSceneSpriteByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetSceneSpriteByIndex(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneTexture(lua_State* L)
{
    elfTexture* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneTexture", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneTexture", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneTexture(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneMaterial(lua_State* L)
{
    elfMaterial* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneMaterial", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneMaterial", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneMaterial", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneMaterial(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneModel(lua_State* L)
{
    elfModel* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneModel", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneModel", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneModel", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneModel(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneScript(lua_State* L)
{
    elfScript* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneScript", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneScript", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneScript", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneScript(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneCamera(lua_State* L)
{
    elfCamera* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneCamera", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneCamera", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneCamera", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneCamera(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneEntity(lua_State* L)
{
    elfEntity* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneEntity", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneEntity", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneEntity", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneEntity(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneLight(lua_State* L)
{
    elfLight* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneLight", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneLight", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneLight", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneLight(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneArmature(lua_State* L)
{
    elfArmature* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneArmature", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneArmature", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneArmature", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneArmature(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneParticles(lua_State* L)
{
    elfParticles* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneParticles", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneParticles", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneParticles", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneParticles(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneSprite(lua_State* L)
{
    elfSprite* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneSprite", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneSprite", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneSprite", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneSprite(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneActor(lua_State* L)
{
    elfActor* result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetSceneActor", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneActor", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetSceneActor", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetSceneActor(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_RemoveSceneCamera(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneCamera", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneCamera", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneCamera", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveSceneCamera(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneEntity(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneEntity", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneEntity", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneEntity", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveSceneEntity(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneLight(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneLight", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneLight", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneLight", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveSceneLight(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneParticles(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneParticles", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneParticles", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneParticles", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveSceneParticles(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneSprite(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneSprite", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneSprite", 1, "elfScene");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneSprite", 2, "string");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfRemoveSceneSprite(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneCameraByIndex(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneCameraByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneCameraByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneCameraByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveSceneCameraByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneEntityByIndex(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneEntityByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneEntityByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneEntityByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveSceneEntityByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneLightByIndex(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneLightByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneLightByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneLightByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveSceneLightByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneParticlesByIndex(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneParticlesByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneParticlesByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneParticlesByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveSceneParticlesByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneSpriteByIndex(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneSpriteByIndex", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneSpriteByIndex", 1, "elfScene");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveSceneSpriteByIndex", 2, "number");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveSceneSpriteByIndex(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneCameraByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfCamera* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneCameraByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneCameraByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_CAMERA)
    {
        return lua_fail_arg(L, "RemoveSceneCameraByObject", 2, "elfCamera");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfCamera*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneCameraByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneEntityByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfEntity* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneEntityByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneEntityByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "RemoveSceneEntityByObject", 2, "elfEntity");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneEntityByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneLightByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfLight* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneLightByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneLightByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_LIGHT)
    {
        return lua_fail_arg(L, "RemoveSceneLightByObject", 2, "elfLight");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfLight*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneLightByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneParticlesByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfParticles* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneParticlesByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneParticlesByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_PARTICLES)
    {
        return lua_fail_arg(L, "RemoveSceneParticlesByObject", 2, "elfParticles");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfParticles*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneParticlesByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneSpriteByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfSprite* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneSpriteByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneSpriteByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SPRITE)
    {
        return lua_fail_arg(L, "RemoveSceneSpriteByObject", 2, "elfSprite");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfSprite*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneSpriteByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveSceneActorByObject(lua_State* L)
{
    unsigned char result;
    elfScene* arg0;
    elfActor* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveSceneActorByObject", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "RemoveSceneActorByObject", 1, "elfScene");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        !elfIsActor(((lua_elfObject*)lua_touserdata(L, 2))->object))
    {
        return lua_fail_arg(L, "RemoveSceneActorByObject", 2, "elfActor");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfActor*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    result = elfRemoveSceneActorByObject(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetSceneScripts(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneScripts", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneScripts", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneScripts(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneTextures(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneTextures", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneTextures", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneTextures(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneMaterials(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneMaterials", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneMaterials", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneMaterials(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSceneModels(lua_State* L)
{
    elfList* result;
    elfScene* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSceneModels", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCENE)
    {
        return lua_fail_arg(L, "GetSceneModels", 1, "elfScene");
    }
    arg0 = (elfScene*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSceneModels(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateScript(lua_State* L)
{
    elfScript* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateScript", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateScript", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateScript(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_CreateScriptFromFile(lua_State* L)
{
    elfScript* result;
    const char* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateScriptFromFile", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateScriptFromFile", 1, "string");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateScriptFromFile", 2, "string");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = lua_tostring(L, 2);
    result = elfCreateScriptFromFile(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetScriptName(lua_State* L)
{
    elfScript* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetScriptName", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "SetScriptName", 1, "elfScript");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetScriptName", 2, "string");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetScriptName(arg0, arg1);
    return 0;
}
static int lua_GetScriptName(lua_State* L)
{
    const char* result;
    elfScript* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetScriptName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "GetScriptName", 1, "elfScript");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetScriptName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetScriptFilePath(lua_State* L)
{
    const char* result;
    elfScript* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetScriptFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "GetScriptFilePath", 1, "elfScript");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetScriptFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_SetScriptText(lua_State* L)
{
    elfScript* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetScriptText", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "SetScriptText", 1, "elfScript");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetScriptText", 2, "string");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetScriptText(arg0, arg1);
    return 0;
}
static int lua_IsScriptError(lua_State* L)
{
    unsigned char result;
    elfScript* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsScriptError", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "IsScriptError", 1, "elfScript");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsScriptError(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RunString(lua_State* L)
{
    unsigned char result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "RunString", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "RunString", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfRunString(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RunScript(lua_State* L)
{
    unsigned char result;
    elfScript* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "RunScript", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "RunScript", 1, "elfScript");
    }
    arg0 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfRunScript(arg0);
    lua_pushboolean(L, result);
    return 1;
}

static int lua_GetCollisionActor(lua_State* L)
{
    elfActor* result;
    elfCollision* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCollisionActor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_COLLISION)
    {
        return lua_fail_arg(L, "GetCollisionActor", 1, "elfCollision");
    }
    arg0 = (elfCollision*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCollisionActor(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetCollisionPosition(lua_State* L)
{
    elfVec3f result;
    elfCollision* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCollisionPosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_COLLISION)
    {
        return lua_fail_arg(L, "GetCollisionPosition", 1, "elfCollision");
    }
    arg0 = (elfCollision*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCollisionPosition(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetCollisionNormal(lua_State* L)
{
    elfVec3f result;
    elfCollision* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCollisionNormal", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_COLLISION)
    {
        return lua_fail_arg(L, "GetCollisionNormal", 1, "elfCollision");
    }
    arg0 = (elfCollision*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCollisionNormal(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetCollisionDepth(lua_State* L)
{
    float result;
    elfCollision* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCollisionDepth", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_COLLISION)
    {
        return lua_fail_arg(L, "GetCollisionDepth", 1, "elfCollision");
    }
    arg0 = (elfCollision*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCollisionDepth(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetJointName(lua_State* L)
{
    const char* result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointName", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetJointType(lua_State* L)
{
    int result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointType", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetJointActorA(lua_State* L)
{
    elfActor* result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointActorA", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointActorA", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointActorA(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetJointActorB(lua_State* L)
{
    elfActor* result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointActorB", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointActorB", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointActorB(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetJointPivot(lua_State* L)
{
    elfVec3f result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointPivot", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointPivot", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointPivot(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_GetJointAxis(lua_State* L)
{
    elfVec3f result;
    elfJoint* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetJointAxis", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_JOINT)
    {
        return lua_fail_arg(L, "GetJointAxis", 1, "elfJoint");
    }
    arg0 = (elfJoint*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetJointAxis(arg0);
    lua_create_elfVec3f(L, result);
    return 1;
}
static int lua_CreateFontFromFile(lua_State* L)
{
    elfFont* result;
    const char* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "CreateFontFromFile", lua_gettop(L), 2);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateFontFromFile", 1, "string");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "CreateFontFromFile", 2, "number");
    }
    arg0 = lua_tostring(L, 1);
    arg1 = (int)lua_tonumber(L, 2);
    result = elfCreateFontFromFile(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetDefaultFont(lua_State* L)
{
    elfFont* result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetDefaultFont", lua_gettop(L), 0);
    }
    result = elfGetDefaultFont();
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetFontName(lua_State* L)
{
    const char* result;
    elfFont* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFontName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "GetFontName", 1, "elfFont");
    }
    arg0 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFontName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetFontFilePath(lua_State* L)
{
    const char* result;
    elfFont* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFontFilePath", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "GetFontFilePath", 1, "elfFont");
    }
    arg0 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFontFilePath(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetFontSize(lua_State* L)
{
    int result;
    elfFont* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetFontSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "GetFontSize", 1, "elfFont");
    }
    arg0 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetFontSize(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetStringWidth(lua_State* L)
{
    int result;
    elfFont* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetStringWidth", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "GetStringWidth", 1, "elfFont");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetStringWidth", 2, "string");
    }
    arg0 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetStringWidth(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetStringHeight(lua_State* L)
{
    int result;
    elfFont* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetStringHeight", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "GetStringHeight", 1, "elfFont");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "GetStringHeight", 2, "string");
    }
    arg0 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    result = elfGetStringHeight(arg0, arg1);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetGuiObjectName(lua_State* L)
{
    const char* result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectName", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectName", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectName(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetGuiObjectPosition(lua_State* L)
{
    elfVec2i result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectPosition", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectPosition", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectPosition(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetGuiObjectSize(lua_State* L)
{
    elfVec2i result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectSize", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectSize", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectSize(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetGuiObjectColor(lua_State* L)
{
    elfColor result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectColor", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetGuiObjectVisible(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectVisible", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectVisible", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectVisible(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetGuiObjectActive(lua_State* L)
{
    unsigned char result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectActive", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectActive", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectActive(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetGuiObjectScript(lua_State* L)
{
    elfScript* result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectScript", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectScript", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectScript(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetGuiObjectEvent(lua_State* L)
{
    int result;
    elfGuiObject* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetGuiObjectEvent", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "GetGuiObjectEvent", 1, "elfGuiObject");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetGuiObjectEvent(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetGuiObjectPosition(lua_State* L)
{
    elfGuiObject* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetGuiObjectPosition", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetGuiObjectPosition", 1, "elfGuiObject");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetGuiObjectPosition", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetGuiObjectPosition", 3, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetGuiObjectPosition(arg0, arg1, arg2);
    return 0;
}
static int lua_SetGuiObjectColor(lua_State* L)
{
    elfGuiObject* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetGuiObjectColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetGuiObjectColor", 1, "elfGuiObject");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetGuiObjectColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetGuiObjectColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetGuiObjectColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetGuiObjectColor", 5, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetGuiObjectColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetGuiObjectVisible(lua_State* L)
{
    elfGuiObject* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetGuiObjectVisible", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetGuiObjectVisible", 1, "elfGuiObject");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetGuiObjectVisible", 2, "boolean");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetGuiObjectVisible(arg0, arg1);
    return 0;
}
static int lua_SetGuiObjectActive(lua_State* L)
{
    elfGuiObject* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetGuiObjectActive", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetGuiObjectActive", 1, "elfGuiObject");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetGuiObjectActive", 2, "boolean");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetGuiObjectActive(arg0, arg1);
    return 0;
}
static int lua_SetGuiObjectScript(lua_State* L)
{
    elfGuiObject* arg0;
    elfScript* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetGuiObjectScript", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "SetGuiObjectScript", 1, "elfGuiObject");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SCRIPT)
    {
        return lua_fail_arg(L, "SetGuiObjectScript", 2, "elfScript");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfScript*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetGuiObjectScript(arg0, arg1);
    return 0;
}

/*
static int lua_CreateLabel(lua_State* L)
{
    elfLabel* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateLabel", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateLabel", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateLabel(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}*/
static int lua_CreateLabel(lua_State* L)
{
    elfLabel* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    const char* arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "CreateLabel", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateLabel", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateLabel", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateLabel", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateLabel", 4, "number");
    }
    if (!lua_isstring(L, 5))
    {
        return lua_fail_arg(L, "CreateLabel", 5, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = lua_tostring(L, 5);
    result = elfCreateLabel(arg0, arg1, arg2, arg3, arg4);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetLabelFont(lua_State* L)
{
    elfFont* result;
    elfLabel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLabelFont", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LABEL)
    {
        return lua_fail_arg(L, "GetLabelFont", 1, "elfLabel");
    }
    arg0 = (elfLabel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLabelFont(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetLabelText(lua_State* L)
{
    const char* result;
    elfLabel* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetLabelText", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LABEL)
    {
        return lua_fail_arg(L, "GetLabelText", 1, "elfLabel");
    }
    arg0 = (elfLabel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetLabelText(arg0);
    lua_pushstring(L, result);
    return 1;
}

static int lua_SetLabelFont(lua_State* L)
{
    elfLabel* arg0;
    elfFont* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLabelFont", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LABEL)
    {
        return lua_fail_arg(L, "SetLabelFont", 1, "elfLabel");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "SetLabelFont", 2, "elfFont");
    }
    arg0 = (elfLabel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetLabelFont(arg0, arg1);
    return 0;
}

static int lua_SetLabelText(lua_State* L)
{
    elfLabel* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetLabelText", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_LABEL)
    {
        return lua_fail_arg(L, "SetLabelText", 1, "elfLabel");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetLabelText", 2, "string");
    }
    arg0 = (elfLabel*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetLabelText(arg0, arg1);
    return 0;
}

/*
static int lua_CreateButton(lua_State* L)
{
    elfButton* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateButton", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateButton", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateButton(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}*/

static int lua_CreateButton(lua_State* L)
{
    elfButton* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    const char* arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "CreateButton", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateButton", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateButton", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateButton", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateButton", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "CreateButton", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "CreateButton", 6, "number");
    }
    if (!lua_isstring(L, 7))
    {
        return lua_fail_arg(L, "CreateButton", 7, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    arg5 = (int)lua_tonumber(L, 6);
    arg6 = lua_tostring(L, 7);
    result = elfCreateButton(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetButtonState(lua_State* L)
{
    unsigned char result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonState", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonState", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonState(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetButtonText(lua_State* L)
{
    const char* result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonText", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonText", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonText(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetButtonFont(lua_State* L)
{
    elfFont* result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonFont", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonFont", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonFont(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetButtonOffTexture(lua_State* L)
{
    elfTexture* result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonOffTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonOffTexture", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonOffTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetButtonOverTexture(lua_State* L)
{
    elfTexture* result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonOverTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonOverTexture", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonOverTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetButtonOnTexture(lua_State* L)
{
    elfTexture* result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonOnTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "GetButtonOnTexture", 1, "elfButton");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonOnTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetButtonTextColor(lua_State* L)
{
    elfColor result;
    elfButton* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetButtonTextColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetButtonTextColor", 1, "elfTextField");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetButtonTextColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_SetButtonTextColor(lua_State* L)
{
    elfButton* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetButtonTextColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonTextColor", 1, "elfButton");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetButtonTextColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetButtonTextColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetButtonTextColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetButtonTextColor", 5, "number");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetButtonTextColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetButtonText(lua_State* L)
{
    elfButton* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetButtonText", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonText", 1, "elfButton");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetButtonText", 2, "string");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetButtonText(arg0, arg1);
    return 0;
}
static int lua_SetButtonFont(lua_State* L)
{
    elfButton* arg0;
    elfFont* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetButtonFont", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonFont", 1, "elfButton");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "SetButtonFont", 2, "elfFont");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetButtonFont(arg0, arg1);
    return 0;
}
static int lua_SetButtonSize(lua_State* L)
{
    elfButton* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetButtonSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonSize", 1, "elfButton");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetButtonSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetButtonSize", 3, "number");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetButtonSize(arg0, arg1, arg2);
    return 0;
}
static int lua_SetButtonOffTexture(lua_State* L)
{
    elfButton* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetButtonOffTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonOffTexture", 1, "elfButton");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetButtonOffTexture", 2, "elfTexture");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetButtonOffTexture(arg0, arg1);
    return 0;
}
static int lua_SetButtonOverTexture(lua_State* L)
{
    elfButton* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetButtonOverTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonOverTexture", 1, "elfButton");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetButtonOverTexture", 2, "elfTexture");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetButtonOverTexture(arg0, arg1);
    return 0;
}
static int lua_SetButtonOnTexture(lua_State* L)
{
    elfButton* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetButtonOnTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_BUTTON)
    {
        return lua_fail_arg(L, "SetButtonOnTexture", 1, "elfButton");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetButtonOnTexture", 2, "elfTexture");
    }
    arg0 = (elfButton*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetButtonOnTexture(arg0, arg1);
    return 0;
}

/*
static int lua_CreatePicture(lua_State* L)
{
    elfPicture* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreatePicture", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreatePicture", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreatePicture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreatePicture(lua_State* L)
{
    elfPicture* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    const char* arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "CreatePicture", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreatePicture", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreatePicture", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreatePicture", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreatePicture", 4, "number");
    }
    if (!lua_isstring(L, 5))
    {
        return lua_fail_arg(L, "CreatePicture", 5, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = lua_tostring(L, 5);
    result = elfCreatePicture(arg0, arg1, arg2, arg3, arg4);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetPictureTexture(lua_State* L)
{
    elfTexture* result;
    elfPicture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPictureTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PICTURE)
    {
        return lua_fail_arg(L, "GetPictureTexture", 1, "elfPicture");
    }
    arg0 = (elfPicture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPictureTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetPictureScale(lua_State* L)
{
    elfVec2f result;
    elfPicture* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetPictureScale", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PICTURE)
    {
        return lua_fail_arg(L, "GetPictureScale", 1, "elfPicture");
    }
    arg0 = (elfPicture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetPictureScale(arg0);
    lua_create_elfVec2f(L, result);
    return 1;
}
static int lua_SetPictureTexture(lua_State* L)
{
    elfPicture* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetPictureTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PICTURE)
    {
        return lua_fail_arg(L, "SetPictureTexture", 1, "elfPicture");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetPictureTexture", 2, "elfTexture");
    }
    arg0 = (elfPicture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetPictureTexture(arg0, arg1);
    return 0;
}
static int lua_SetPictureScale(lua_State* L)
{
    elfPicture* arg0;
    float arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetPictureScale", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_PICTURE)
    {
        return lua_fail_arg(L, "SetPictureScale", 1, "elfPicture");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetPictureScale", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetPictureScale", 3, "number");
    }
    arg0 = (elfPicture*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    elfSetPictureScale(arg0, arg1, arg2);
    return 0;
}

/*
static int lua_CreateTextField(lua_State* L)
{
    elfTextField* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateTextField", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateTextField", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateTextField(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreateTextField(lua_State* L)
{
    elfTextField* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    int arg4;
    const char* arg5;
    if (lua_gettop(L) != 6)
    {
        return lua_fail_arg_count(L, "CreateTextField", lua_gettop(L), 6);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateTextField", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateTextField", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateTextField", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateTextField", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "CreateTextField", 5, "number");
    }
    if (!lua_isstring(L, 6))
    {
        return lua_fail_arg(L, "CreateTextField", 6, "string");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    arg5 = lua_tostring(L, 6);
    result = elfCreateTextField(arg0, arg1, arg2, arg3, arg4, arg5);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetTextFieldTexture(lua_State* L)
{
    elfTexture* result;
    elfTextField* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextFieldTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetTextFieldTexture", 1, "elfTextField");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextFieldTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetTextFieldFont(lua_State* L)
{
    elfFont* result;
    elfTextField* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextFieldFont", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetTextFieldFont", 1, "elfTextField");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextFieldFont(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetTextFieldTextColor(lua_State* L)
{
    elfColor result;
    elfTextField* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextFieldTextColor", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetTextFieldTextColor", 1, "elfTextField");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextFieldTextColor(arg0);
    lua_create_elfColor(L, result);
    return 1;
}
static int lua_GetTextFieldOffset(lua_State* L)
{
    elfVec2i result;
    elfTextField* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextFieldOffset", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetTextFieldOffset", 1, "elfTextField");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextFieldOffset(arg0);
    lua_create_elfVec2i(L, result);
    return 1;
}
static int lua_GetTextFieldText(lua_State* L)
{
    const char* result;
    elfTextField* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextFieldText", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "GetTextFieldText", 1, "elfTextField");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextFieldText(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_SetTextFieldFont(lua_State* L)
{
    elfTextField* arg0;
    elfFont* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextFieldFont", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldFont", 1, "elfTextField");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "SetTextFieldFont", 2, "elfFont");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetTextFieldFont(arg0, arg1);
    return 0;
}
static int lua_SetTextFieldWidth(lua_State* L)
{
    elfTextField* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextFieldWidth", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldWidth", 1, "elfTextField");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextFieldWidth", 2, "number");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetTextFieldWidth(arg0, arg1);
    return 0;
}
static int lua_SetTextFieldTexture(lua_State* L)
{
    elfTextField* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextFieldTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldTexture", 1, "elfTextField");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetTextFieldTexture", 2, "elfTexture");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetTextFieldTexture(arg0, arg1);
    return 0;
}
static int lua_SetTextFieldTextColor(lua_State* L)
{
    elfTextField* arg0;
    float arg1;
    float arg2;
    float arg3;
    float arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "SetTextFieldTextColor", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldTextColor", 1, "elfTextField");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextFieldTextColor", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetTextFieldTextColor", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "SetTextFieldTextColor", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "SetTextFieldTextColor", 5, "number");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    arg2 = (float)lua_tonumber(L, 3);
    arg3 = (float)lua_tonumber(L, 4);
    arg4 = (float)lua_tonumber(L, 5);
    elfSetTextFieldTextColor(arg0, arg1, arg2, arg3, arg4);
    return 0;
}
static int lua_SetTextFieldOffset(lua_State* L)
{
    elfTextField* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetTextFieldOffset", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldOffset", 1, "elfTextField");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextFieldOffset", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetTextFieldOffset", 3, "number");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetTextFieldOffset(arg0, arg1, arg2);
    return 0;
}
static int lua_SetTextFieldCursorPosition(lua_State* L)
{
    elfTextField* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextFieldCursorPosition", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldCursorPosition", 1, "elfTextField");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextFieldCursorPosition", 2, "number");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetTextFieldCursorPosition(arg0, arg1);
    return 0;
}
static int lua_SetTextFieldText(lua_State* L)
{
    elfTextField* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextFieldText", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_FIELD)
    {
        return lua_fail_arg(L, "SetTextFieldText", 1, "elfTextField");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "SetTextFieldText", 2, "string");
    }
    arg0 = (elfTextField*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfSetTextFieldText(arg0, arg1);
    return 0;
}

/*
static int lua_CreateSlider(lua_State* L)
{
    elfSlider* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateSlider", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateSlider", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateSlider(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreateSlider(lua_State* L)
{
    elfSlider* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    float arg6;
    if (lua_gettop(L) != 7)
    {
        return lua_fail_arg_count(L, "CreateSlider", lua_gettop(L), 7);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateSlider", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateSlider", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateSlider", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateSlider", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "CreateSlider", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "CreateSlider", 6, "number");
    }
    if (!lua_isnumber(L, 7))
    {
        return lua_fail_arg(L, "CreateSlider", 7, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    arg5 = (int)lua_tonumber(L, 6);
    arg6 = (float)lua_tonumber(L, 7);
    result = elfCreateSlider(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetSliderBackgroundTexture(lua_State* L)
{
    elfTexture* result;
    elfSlider* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSliderBackgroundTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "GetSliderBackgroundTexture", 1, "elfSlider");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSliderBackgroundTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSliderSliderTexture(lua_State* L)
{
    elfTexture* result;
    elfSlider* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSliderSliderTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "GetSliderSliderTexture", 1, "elfSlider");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSliderSliderTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetSliderValue(lua_State* L)
{
    float result;
    elfSlider* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSliderValue", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "GetSliderValue", 1, "elfSlider");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSliderValue(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_SetSliderSize(lua_State* L)
{
    elfSlider* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetSliderSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "SetSliderSize", 1, "elfSlider");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSliderSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetSliderSize", 3, "number");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetSliderSize(arg0, arg1, arg2);
    return 0;
}
static int lua_SetSliderBackgroundTexture(lua_State* L)
{
    elfSlider* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSliderBackgroundTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "SetSliderBackgroundTexture", 1, "elfSlider");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetSliderBackgroundTexture", 2, "elfTexture");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetSliderBackgroundTexture(arg0, arg1);
    return 0;
}
static int lua_SetSliderSliderTexture(lua_State* L)
{
    elfSlider* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSliderSliderTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "SetSliderSliderTexture", 1, "elfSlider");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetSliderSliderTexture", 2, "elfTexture");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetSliderSliderTexture(arg0, arg1);
    return 0;
}
static int lua_SetSliderValue(lua_State* L)
{
    elfSlider* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSliderValue", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SLIDER)
    {
        return lua_fail_arg(L, "SetSliderValue", 1, "elfSlider");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSliderValue", 2, "number");
    }
    arg0 = (elfSlider*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetSliderValue(arg0, arg1);
    return 0;
}

/*
static int lua_CreateScreen(lua_State* L)
{
    elfScreen* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateScreen", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateScreen", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateScreen(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreateScreen(lua_State* L)
{
    elfScreen* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    if (lua_gettop(L) != 6)
    {
        return lua_fail_arg_count(L, "CreateScreen", lua_gettop(L), 6);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateScreen", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateScreen", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateScreen", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateScreen", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "CreateScreen", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "CreateScreen", 6, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    arg5 = (int)lua_tonumber(L, 6);
    result = elfCreateScreen(arg0, arg1, arg2, arg3, arg4, arg5);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_SetScreenSize(lua_State* L)
{
    elfScreen* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetScreenSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "SetScreenSize", 1, "elfScreen");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetScreenSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetScreenSize", 3, "number");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetScreenSize(arg0, arg1, arg2);
    return 0;
}
static int lua_GetScreenTexture(lua_State* L)
{
    elfTexture* result;
    elfScreen* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetScreenTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "GetScreenTexture", 1, "elfScreen");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetScreenTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetScreenTexture(lua_State* L)
{
    elfScreen* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetScreenTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "SetScreenTexture", 1, "elfScreen");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetScreenTexture", 2, "elfTexture");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetScreenTexture(arg0, arg1);
    return 0;
}
static int lua_SetScreenToTop(lua_State* L)
{
    elfScreen* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetScreenToTop", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "SetScreenToTop", 1, "elfScreen");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfSetScreenToTop(arg0);
    return 0;
}
static int lua_ForceScreenFocus(lua_State* L)
{
    elfScreen* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ForceScreenFocus", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "ForceScreenFocus", 1, "elfScreen");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfForceScreenFocus(arg0);
    return 0;
}
static int lua_ReleaseScreenFocus(lua_State* L)
{
    elfScreen* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ReleaseScreenFocus", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SCREEN)
    {
        return lua_fail_arg(L, "ReleaseScreenFocus", 1, "elfScreen");
    }
    arg0 = (elfScreen*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfReleaseScreenFocus(arg0);
    return 0;
}

/*
static int lua_CreateTextList(lua_State* L)
{
    elfTextList* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateTextList", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateTextList", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateTextList(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreateTextList(lua_State* L)
{
    elfTextList* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    if (lua_gettop(L) != 6)
    {
        return lua_fail_arg_count(L, "CreateTextList", lua_gettop(L), 6);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateTextList", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateTextList", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateTextList", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateTextList", 4, "number");
    }
    if (!lua_isnumber(L, 5))
    {
        return lua_fail_arg(L, "CreateTextList", 5, "number");
    }
    if (!lua_isnumber(L, 6))
    {
        return lua_fail_arg(L, "CreateTextList", 6, "number");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (int)lua_tonumber(L, 5);
    arg5 = (int)lua_tonumber(L, 6);
    result = elfCreateTextList(arg0, arg1, arg2, arg3, arg4, arg5);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetTextListFont(lua_State* L)
{
    elfFont* result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListFont", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListFont", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListFont(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetTextListRowCount(lua_State* L)
{
    int result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListRowCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListRowCount", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListRowCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextListItemCount(lua_State* L)
{
    int result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListItemCount", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListItemCount", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListItemCount(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextListSelectionIndex(lua_State* L)
{
    int result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListSelectionIndex", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListSelectionIndex", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListSelectionIndex(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextListOffset(lua_State* L)
{
    int result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListOffset", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListOffset", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListOffset(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}
static int lua_GetTextListItem(lua_State* L)
{
    const char* result;
    elfTextList* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "GetTextListItem", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListItem", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "GetTextListItem", 2, "number");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfGetTextListItem(arg0, arg1);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetTextListSelectedItem(lua_State* L)
{
    const char* result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListSelectedItem", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListSelectedItem", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListSelectedItem(arg0);
    lua_pushstring(L, result);
    return 1;
}
static int lua_GetTextListItemDrag(lua_State* L)
{
    unsigned char result;
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetTextListItemDrag", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "GetTextListItemDrag", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetTextListItemDrag(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_SetTextListFont(lua_State* L)
{
    elfTextList* arg0;
    elfFont* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextListFont", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListFont", 1, "elfTextList");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_FONT)
    {
        return lua_fail_arg(L, "SetTextListFont", 2, "elfFont");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfFont*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetTextListFont(arg0, arg1);
    return 0;
}
static int lua_SetTextListSize(lua_State* L)
{
    elfTextList* arg0;
    int arg1;
    int arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetTextListSize", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListSize", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextListSize", 2, "number");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "SetTextListSize", 3, "number");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    elfSetTextListSize(arg0, arg1, arg2);
    return 0;
}
static int lua_AddTextListItem(lua_State* L)
{
    elfTextList* arg0;
    const char* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "AddTextListItem", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "AddTextListItem", 1, "elfTextList");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "AddTextListItem", 2, "string");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    elfAddTextListItem(arg0, arg1);
    return 0;
}
static int lua_SetTextListItem(lua_State* L)
{
    elfTextList* arg0;
    int arg1;
    const char* arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "SetTextListItem", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListItem", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextListItem", 2, "number");
    }
    if (!lua_isstring(L, 3))
    {
        return lua_fail_arg(L, "SetTextListItem", 3, "string");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    arg2 = lua_tostring(L, 3);
    elfSetTextListItem(arg0, arg1, arg2);
    return 0;
}
static int lua_RemoveTextListItem(lua_State* L)
{
    unsigned char result;
    elfTextList* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "RemoveTextListItem", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "RemoveTextListItem", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "RemoveTextListItem", 2, "number");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    result = elfRemoveTextListItem(arg0, arg1);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_RemoveTextListItems(lua_State* L)
{
    elfTextList* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "RemoveTextListItems", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "RemoveTextListItems", 1, "elfTextList");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfRemoveTextListItems(arg0);
    return 0;
}
static int lua_SetTextListOffset(lua_State* L)
{
    elfTextList* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextListOffset", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListOffset", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextListOffset", 2, "number");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetTextListOffset(arg0, arg1);
    return 0;
}
static int lua_SetTextListSelection(lua_State* L)
{
    elfTextList* arg0;
    int arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextListSelection", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListSelection", 1, "elfTextList");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetTextListSelection", 2, "number");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (int)lua_tonumber(L, 2);
    elfSetTextListSelection(arg0, arg1);
    return 0;
}
static int lua_SetTextListItemDrag(lua_State* L)
{
    elfTextList* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetTextListItemDrag", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_TEXT_LIST)
    {
        return lua_fail_arg(L, "SetTextListItemDrag", 1, "elfTextList");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetTextListItemDrag", 2, "boolean");
    }
    arg0 = (elfTextList*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetTextListItemDrag(arg0, arg1);
    return 0;
}

/*
static int lua_CreateCheckBox(lua_State* L)
{
    elfCheckBox* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "CreateCheckBox", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "CreateCheckBox", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfCreateCheckBox(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
*/

static int lua_CreateCheckBox(lua_State* L)
{
    elfCheckBox* result;
    elfGuiObject* arg0;
    const char* arg1;
    int arg2;
    int arg3;
    unsigned char arg4;
    if (lua_gettop(L) != 5)
    {
        return lua_fail_arg_count(L, "CreateCheckBox", lua_gettop(L), 5);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        !elfIsGuiObject(((lua_elfObject*)lua_touserdata(L, 1))->object))
    {
        return lua_fail_arg(L, "CreateCheckBox", 1, "elfGuiObject");
    }
    if (!lua_isstring(L, 2))
    {
        return lua_fail_arg(L, "CreateCheckBox", 2, "string");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "CreateCheckBox", 3, "number");
    }
    if (!lua_isnumber(L, 4))
    {
        return lua_fail_arg(L, "CreateCheckBox", 4, "number");
    }
    if (!lua_isboolean(L, 5))
    {
        return lua_fail_arg(L, "CreateCheckBox", 5, "boolean");
    }
    arg0 = (elfGuiObject*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = lua_tostring(L, 2);
    arg2 = (int)lua_tonumber(L, 3);
    arg3 = (int)lua_tonumber(L, 4);
    arg4 = (unsigned char)lua_toboolean(L, 5);
    result = elfCreateCheckBox(arg0, arg1, arg2, arg3, arg4);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

static int lua_GetCheckBoxState(lua_State* L)
{
    unsigned char result;
    elfCheckBox* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCheckBoxState", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "GetCheckBoxState", 1, "elfCheckBox");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCheckBoxState(arg0);
    lua_pushboolean(L, result);
    return 1;
}
static int lua_GetCheckBoxOffTexture(lua_State* L)
{
    elfTexture* result;
    elfCheckBox* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCheckBoxOffTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "GetCheckBoxOffTexture", 1, "elfCheckBox");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCheckBoxOffTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_GetCheckBoxOnTexture(lua_State* L)
{
    elfTexture* result;
    elfCheckBox* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetCheckBoxOnTexture", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "GetCheckBoxOnTexture", 1, "elfCheckBox");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetCheckBoxOnTexture(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}
static int lua_SetCheckBoxOffTexture(lua_State* L)
{
    elfCheckBox* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCheckBoxOffTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "SetCheckBoxOffTexture", 1, "elfCheckBox");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetCheckBoxOffTexture", 2, "elfTexture");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetCheckBoxOffTexture(arg0, arg1);
    return 0;
}
static int lua_SetCheckBoxOnTexture(lua_State* L)
{
    elfCheckBox* arg0;
    elfTexture* arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCheckBoxOnTexture", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "SetCheckBoxOnTexture", 1, "elfCheckBox");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_TEXTURE)
    {
        return lua_fail_arg(L, "SetCheckBoxOnTexture", 2, "elfTexture");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfTexture*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    elfSetCheckBoxOnTexture(arg0, arg1);
    return 0;
}
static int lua_SetCheckBoxState(lua_State* L)
{
    elfCheckBox* arg0;
    unsigned char arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetCheckBoxState", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_CHECK_BOX)
    {
        return lua_fail_arg(L, "SetCheckBoxState", 1, "elfCheckBox");
    }
    if (!lua_isboolean(L, 2))
    {
        return lua_fail_arg(L, "SetCheckBoxState", 2, "boolean");
    }
    arg0 = (elfCheckBox*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (unsigned char)lua_toboolean(L, 2);
    elfSetCheckBoxState(arg0, arg1);
    return 0;
}
static int lua_CreateGui(lua_State* L)
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
static int lua_AddGuiObject(lua_State* L)
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
static int lua_GetGuiObjectByName(lua_State* L)
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
static int lua_GetGuiObjectByIndex(lua_State* L)
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
static int lua_RemoveGuiObjectByName(lua_State* L)
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
static int lua_RemoveGuiObjectByIndex(lua_State* L)
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
static int lua_RemoveGuiObjectByObject(lua_State* L)
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
static int lua_GetGuiTrace(lua_State* L)
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
static int lua_GetGuiFocus(lua_State* L)
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
static int lua_GetGuiActiveTextField(lua_State* L)
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
static int lua_GetGuiDragging(lua_State* L)
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
static int lua_GetGuiDragObject(lua_State* L)
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
static int lua_GetGuiDragContent(lua_State* L)
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
static int lua_EmptyGui(lua_State* L)
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
static int lua_CreateRequest(lua_State* L)
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
static int lua_SendRequest(lua_State* L)
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
static int lua_GetRequestUrl(lua_State* L)
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
static int lua_SetRequestUrl(lua_State* L)
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
static int lua_GetRequestMethod(lua_State* L)
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
static int lua_SetRequestMethod(lua_State* L)
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

static int lua_GetRequestState(lua_State* L)
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

static const luaL_Reg lua_elf_functions[] = {{"IncRef", lua_IncRef},
                                             {"DecRef", lua_DecRef},
                                             {"GetObjectType", lua_GetObjectType},
                                             {"GetObjectRefCount", lua_GetObjectRefCount},
                                             {"GetGlobalRefCount", lua_GetGlobalRefCount},
                                             {"GetGlobalObjCount", lua_GetGlobalObjCount},
                                             {"IsActor", lua_IsActor},
                                             {"IsGuiObject", lua_IsGuiObject},
                                             {"CreateList", lua_CreateList},
                                             {"GetListLength", lua_GetListLength},
                                             {"InsertListObject", lua_InsertListObject},
                                             {"AppendListObject", lua_AppendListObject},
                                             {"RemoveListObject", lua_RemoveListObject},
                                             {"GetListObject", lua_GetListObject},
                                             {"BeginList", lua_BeginList},
                                             {"GetListNext", lua_GetListNext},
                                             {"RBeginList", lua_RBeginList},
                                             {"GetListRNext", lua_GetListRNext},
                                             {"SeekList", lua_SeekList},
                                             {"RSeekList", lua_RSeekList},
                                             {"ReadConfig", lua_ReadConfig},
                                             {"SetConfigWindowSize", lua_SetConfigWindowSize},
                                             {"SetConfigMultisamples", lua_SetConfigMultisamples},
                                             {"SetConfigFullscreen", lua_SetConfigFullscreen},
                                             {"SetConfigTextureCompress", lua_SetConfigTextureCompress},
                                             {"SetConfigTextureAnisotropy", lua_SetConfigTextureAnisotropy},
                                             {"SetConfigShadowMapSize", lua_SetConfigShadowMapSize},
                                             {"SetConfigStart", lua_SetConfigStart},
                                             {"SetConfigLogPath", lua_SetConfigLogPath},
                                             {"GetConfigWindowSize", lua_GetConfigWindowSize},
                                             {"GetConfigMultisamples", lua_GetConfigMultisamples},
                                             {"GetConfigFullscreen", lua_GetConfigFullscreen},
                                             {"GetConfigTextureCompress", lua_GetConfigTextureCompress},
                                             {"GetConfigTextureAnisotropy", lua_GetConfigTextureAnisotropy},
                                             {"GetConfigShadowMapSize", lua_GetConfigShadowMapSize},
                                             {"GetConfigStart", lua_GetConfigStart},
                                             {"GetConfigLogPath", lua_GetConfigLogPath},
                                             {"WriteLogLine", lua_WriteLogLine},
                                             {"SetTitle", lua_SetTitle},
                                             {"GetWindowWidth", lua_GetWindowWidth},
                                             {"GetWindowHeight", lua_GetWindowHeight},
                                             {"GetVideoModeCount", lua_GetVideoModeCount},
                                             {"GetVideoMode", lua_GetVideoMode},
                                             {"GetMultisamples", lua_GetMultisamples},
                                             {"IsFullscreen", lua_IsFullscreen},
                                             {"GetTitle", lua_GetTitle},
                                             {"GetTime", lua_GetTime},
                                             {"Sleep", lua_Sleep},
                                             {"IsWindowOpened", lua_IsWindowOpened},
                                             {"GetMousePosition", lua_GetMousePosition},
                                             {"GetMouseForce", lua_GetMouseForce},
                                             {"SetMousePosition", lua_SetMousePosition},
                                             {"HideMouse", lua_HideMouse},
                                             {"IsMouseHidden", lua_IsMouseHidden},
                                             {"GetMouseWheel", lua_GetMouseWheel},
                                             {"GetMouseButtonState", lua_GetMouseButtonState},
                                             {"GetKeyState", lua_GetKeyState},
                                             {"GetJoystickPresent", lua_GetJoystickPresent},
                                             {"GetJoystickAxis", lua_GetJoystickAxis},
                                             {"GetJoystickButtonState", lua_GetJoystickButtonState},
                                             {"GetEventCount", lua_GetEventCount},
                                             {"GetEvent", lua_GetEvent},
                                             {"GetKeyEventKey", lua_GetKeyEventKey},
                                             {"GetKeyEventState", lua_GetKeyEventState},
                                             {"Init", lua_Init},
                                             {"Deinit", lua_Deinit},
                                             {"ResizeWindow", lua_ResizeWindow},
                                             {"GetPlatform", lua_GetPlatform},
                                             {"GetVersionMajor", lua_GetVersionMajor},
                                             {"GetVersionMinor", lua_GetVersionMinor},
                                             {"GetVersionRelease", lua_GetVersionRelease},
                                             {"GetVersion", lua_GetVersion},
                                             {"GetCurrentDirectory", lua_GetCurrentDirectory},
                                             {"GetErrorString", lua_GetErrorString},
                                             {"GetError", lua_GetError},
                                             {"Run", lua_Run},
                                             {"Quit", lua_Quit},
                                             {"SetF10Exit", lua_SetF10Exit},
                                             {"GetF10Exit", lua_GetF10Exit},
                                             {"LoadScene", lua_LoadScene},
                                             {"SetScene", lua_SetScene},
                                             {"GetScene", lua_GetScene},
                                             {"SetGui", lua_SetGui},
                                             {"GetGui", lua_GetGui},
                                             {"GetSync", lua_GetSync},
                                             {"GetFps", lua_GetFps},
                                             {"SaveScreenShot", lua_SaveScreenShot},
                                             {"SetFpsLimit", lua_SetFpsLimit},
                                             {"GetFpsLimit", lua_GetFpsLimit},
                                             {"SetTickRate", lua_SetTickRate},
                                             {"GetTickRate", lua_GetTickRate},
                                             {"SetSpeed", lua_SetSpeed},
                                             {"GetSpeed", lua_GetSpeed},
                                             {"SetTextureCompress", lua_SetTextureCompress},
                                             {"GetTextureCompress", lua_GetTextureCompress},
                                             {"SetTextureAnisotropy", lua_SetTextureAnisotropy},
                                             {"GetTextureAnisotropy", lua_GetTextureAnisotropy},
                                             {"SetShadowMapSize", lua_SetShadowMapSize},
                                             {"GetShadowMapSize", lua_GetShadowMapSize},
                                             {"GetPolygonsRendered", lua_GetPolygonsRendered},
                                             {"SetBloom", lua_SetBloom},
                                             {"DisableBloom", lua_DisableBloom},
                                             {"GetBloomThreshold", lua_GetBloomThreshold},
                                             {"SetDof", lua_SetDof},
                                             {"DisableDof", lua_DisableDof},
                                             {"GetDofFocalRange", lua_GetDofFocalRange},
                                             {"GetDofFocalDistance", lua_GetDofFocalDistance},
                                             {"SetSsao", lua_SetSsao},
                                             {"DisableSsao", lua_DisableSsao},
                                             {"GetSsaoAmount", lua_GetSsaoAmount},
                                             {"SetLightShafts", lua_SetLightShafts},
                                             {"DisableLightShafts", lua_DisableLightShafts},
                                             {"GetLightShaftsIntensity", lua_GetLightShaftsIntensity},
                                             {"IsBloom", lua_IsBloom},
                                             {"IsSsao", lua_IsSsao},
                                             {"IsDof", lua_IsDof},
                                             {"IsLightShafts", lua_IsLightShafts},
                                             {"GetActor", lua_GetActor},
                                             {"ReadDirectory", lua_ReadDirectory},
                                             {"GetDirectoryPath", lua_GetDirectoryPath},
                                             {"GetDirectoryItemCount", lua_GetDirectoryItemCount},
                                             {"GetDirectoryItem", lua_GetDirectoryItem},
                                             {"GetDirectoryItemName", lua_GetDirectoryItemName},
                                             {"GetDirectoryItemType", lua_GetDirectoryItemType},
                                             {"CreateColor", lua_CreateColor},
                                             {"CreateVec2f", lua_CreateVec2f},
                                             {"CreateVec3f", lua_CreateVec3f},
                                             {"CreateQua", lua_CreateQua},
                                             {"CreateQuaFromEuler", lua_CreateQuaFromEuler},
                                             {"CreateQuaFromAngleAxis", lua_CreateQuaFromAngleAxis},
                                             {"AddVec2fVec2f", lua_AddVec2fVec2f},
                                             {"SubVec2fVec2f", lua_SubVec2fVec2f},
                                             {"MulVec2fVec2f", lua_MulVec2fVec2f},
                                             {"GetVec2fLength", lua_GetVec2fLength},
                                             {"AddVec3fVec3f", lua_AddVec3fVec3f},
                                             {"SubVec3fVec3f", lua_SubVec3fVec3f},
                                             {"MulVec3fVec3f", lua_MulVec3fVec3f},
                                             {"GetVec3fLength", lua_GetVec3fLength},
                                             {"RotateVec3f", lua_RotateVec3f},
                                             {"MulQuaVec3f", lua_MulQuaVec3f},
                                             {"MulQuaQua", lua_MulQuaQua},
                                             {"GetQuaInverted", lua_GetQuaInverted},
                                             {"AboutSame", lua_AboutSame},
                                             {"AboutZero", lua_AboutZero},
                                             {"FloatAbs", lua_FloatAbs},
                                             {"FloatMin", lua_FloatMin},
                                             {"FloatMax", lua_FloatMax},
                                             {"RandomFloat", lua_RandomFloat},
                                             {"RandomFloatRange", lua_RandomFloatRange},
                                             {"RandomInt", lua_RandomInt},
                                             {"RandomIntRange", lua_RandomIntRange},
                                             {"GetResourcesTexture", lua_GetResourcesTexture},
                                             {"GetOrLoadResourcesTexture", lua_GetOrLoadResourcesTexture},
                                             {"CreateFramePlayer", lua_CreateFramePlayer},
                                             {"UpdateFramePlayer", lua_UpdateFramePlayer},
                                             {"SetFramePlayerFrame", lua_SetFramePlayerFrame},
                                             {"PlayFramePlayer", lua_PlayFramePlayer},
                                             {"LoopFramePlayer", lua_LoopFramePlayer},
                                             {"StopFramePlayer", lua_StopFramePlayer},
                                             {"PauseFramePlayer", lua_PauseFramePlayer},
                                             {"ResumeFramePlayer", lua_ResumeFramePlayer},
                                             {"GetFramePlayerStart", lua_GetFramePlayerStart},
                                             {"GetFramePlayerEnd", lua_GetFramePlayerEnd},
                                             {"GetFramePlayerSpeed", lua_GetFramePlayerSpeed},
                                             {"GetFramePlayerFrame", lua_GetFramePlayerFrame},
                                             {"IsFramePlayerPlaying", lua_IsFramePlayerPlaying},
                                             {"IsFramePlayerPaused", lua_IsFramePlayerPaused},
                                             {"CreateTimer", lua_CreateTimer},
                                             {"StartTimer", lua_StartTimer},
                                             {"GetElapsedTime", lua_GetElapsedTime},
                                             {"CreateEmptyImage", lua_CreateEmptyImage},
                                             {"CreateImageFromFile", lua_CreateImageFromFile},
                                             {"SetImagePixel", lua_SetImagePixel},
                                             {"GetImageWidth", lua_GetImageWidth},
                                             {"GetImageHeight", lua_GetImageHeight},
                                             {"GetImageBitsPerPixel", lua_GetImageBitsPerPixel},
                                             {"GetImagePixel", lua_GetImagePixel},
                                             {"CreateTextureFromFile", lua_CreateTextureFromFile},
                                             {"CreateTextureFromImage", lua_CreateTextureFromImage},
                                             {"CreateCubeMapFromFiles", lua_CreateCubeMapFromFiles},
                                             {"SetTextureName", lua_SetTextureName},
                                             {"GetTextureName", lua_GetTextureName},
                                             {"GetTextureFilePath", lua_GetTextureFilePath},
                                             {"GetTextureWidth", lua_GetTextureWidth},
                                             {"GetTextureHeight", lua_GetTextureHeight},
                                             {"GetTextureFormat", lua_GetTextureFormat},
                                             {"GetTextureDataFormat", lua_GetTextureDataFormat},
                                             {"CreateMaterial", lua_CreateMaterial},
                                             {"SetMaterialDiffuseColor", lua_SetMaterialDiffuseColor},
                                             {"SetMaterialSpecularColor", lua_SetMaterialSpecularColor},
                                             {"SetMaterialAmbientColor", lua_SetMaterialAmbientColor},
                                             {"SetMaterialSpecularPower", lua_SetMaterialSpecularPower},
                                             {"SetMaterialLighting", lua_SetMaterialLighting},
                                             {"SetMaterialName", lua_SetMaterialName},
                                             {"GetMaterialName", lua_GetMaterialName},
                                             {"GetMaterialFilePath", lua_GetMaterialFilePath},
                                             {"GetMaterialDiffuseColor", lua_GetMaterialDiffuseColor},
                                             {"GetMaterialSpecularColor", lua_GetMaterialSpecularColor},
                                             {"GetMaterialAmbientColor", lua_GetMaterialAmbientColor},
                                             {"GetMaterialSpecularPower", lua_GetMaterialSpecularPower},
                                             {"GetMaterialLighting", lua_GetMaterialLighting},
                                             {"SetMaterialDiffuseMap", lua_SetMaterialDiffuseMap},
                                             {"SetMaterialNormalMap", lua_SetMaterialNormalMap},
                                             {"SetMaterialHeightMap", lua_SetMaterialHeightMap},
                                             {"SetMaterialSpecularMap", lua_SetMaterialSpecularMap},
                                             {"SetMaterialLightMap", lua_SetMaterialLightMap},
                                             {"SetMaterialCubeMap", lua_SetMaterialCubeMap},
                                             {"ClearMaterialDiffuseMap", lua_ClearMaterialDiffuseMap},
                                             {"ClearMaterialNormalMap", lua_ClearMaterialNormalMap},
                                             {"ClearMaterialHeightMap", lua_ClearMaterialHeightMap},
                                             {"ClearMaterialSpecularMap", lua_ClearMaterialSpecularMap},
                                             {"ClearMaterialLightMap", lua_ClearMaterialLightMap},
                                             {"ClearMaterialCubeMap", lua_ClearMaterialCubeMap},
                                             {"GetMaterialDiffuseMap", lua_GetMaterialDiffuseMap},
                                             {"GetMaterialNormalMap", lua_GetMaterialNormalMap},
                                             {"GetMaterialHeightMap", lua_GetMaterialHeightMap},
                                             {"GetMaterialSpecularMap", lua_GetMaterialSpecularMap},
                                             {"GetMaterialLightMap", lua_GetMaterialLightMap},
                                             {"GetMaterialCubeMap", lua_GetMaterialCubeMap},
                                             {"SetMaterialParallaxScale", lua_SetMaterialParallaxScale},
                                             {"SetMaterialAlphaTest", lua_SetMaterialAlphaTest},
                                             {"SetMaterialAlphaThreshold", lua_SetMaterialAlphaThreshold},
                                             {"GetMaterialParallaxScale", lua_GetMaterialParallaxScale},
                                             {"GetMaterialAlphaTest", lua_GetMaterialAlphaTest},
                                             {"GetMaterialAlphaThreshold", lua_GetMaterialAlphaThreshold},
                                             {"CreateBezierPoint", lua_CreateBezierPoint},
                                             {"SetBezierPointPosition", lua_SetBezierPointPosition},
                                             {"SetBezierPointControl1", lua_SetBezierPointControl1},
                                             {"SetBezierPointControl2", lua_SetBezierPointControl2},
                                             {"GetBezierPointPosition", lua_GetBezierPointPosition},
                                             {"GetBezierPointControl1", lua_GetBezierPointControl1},
                                             {"GetBezierPointControl2", lua_GetBezierPointControl2},
                                             {"CreateBezierCurve", lua_CreateBezierCurve},
                                             {"SetBezierCurveType", lua_SetBezierCurveType},
                                             {"GetBezierCurveType", lua_GetBezierCurveType},
                                             {"AddBezierCurvePoint", lua_AddBezierCurvePoint},
                                             {"GetPointFromBezierCurve", lua_GetPointFromBezierCurve},
                                             {"GetBezierCurveValue", lua_GetBezierCurveValue},
                                             {"CreateIpo", lua_CreateIpo},
                                             {"AddIpoCurve", lua_AddIpoCurve},
                                             {"GetIpoCurveCount", lua_GetIpoCurveCount},
                                             {"GetCurveFromIpo", lua_GetCurveFromIpo},
                                             {"GetIpoLoc", lua_GetIpoLoc},
                                             {"GetIpoRot", lua_GetIpoRot},
                                             {"GetIpoScale", lua_GetIpoScale},
                                             {"GetIpoQua", lua_GetIpoQua},
                                             {"CreateProperty", lua_CreateProperty},
                                             {"GetPropertyType", lua_GetPropertyType},
                                             {"GetPropertyInt", lua_GetPropertyInt},
                                             {"GetPropertyFloat", lua_GetPropertyFloat},
                                             {"GetPropertyString", lua_GetPropertyString},
                                             {"GetPropertyBool", lua_GetPropertyBool},
                                             {"SetPropertyInt", lua_SetPropertyInt},
                                             {"SetPropertyFloat", lua_SetPropertyFloat},
                                             {"SetPropertyString", lua_SetPropertyString},
                                             {"SetPropertyBool", lua_SetPropertyBool},
                                             {"GetActorName", lua_GetActorName},
                                             {"GetActorFilePath", lua_GetActorFilePath},
                                             {"GetActorScript", lua_GetActorScript},
                                             {"SetActorName", lua_SetActorName},
                                             {"SetActorScript", lua_SetActorScript},
                                             {"ClearActorScript", lua_ClearActorScript},
                                             {"SetActorPosition", lua_SetActorPosition},
                                             {"SetActorRotation", lua_SetActorRotation},
                                             {"SetActorOrientation", lua_SetActorOrientation},
                                             {"RotateActor", lua_RotateActor},
                                             {"RotateActorLocal", lua_RotateActorLocal},
                                             {"MoveActor", lua_MoveActor},
                                             {"MoveActorLocal", lua_MoveActorLocal},
                                             {"SetActorPositionRelativeTo", lua_SetActorPositionRelativeTo},
                                             {"SetActorRotationRelativeTo", lua_SetActorRotationRelativeTo},
                                             {"SetActorOrientationRelativeTo", lua_SetActorOrientationRelativeTo},
                                             {"GetActorPosition", lua_GetActorPosition},
                                             {"GetActorRotation", lua_GetActorRotation},
                                             {"GetActorOrientation", lua_GetActorOrientation},
                                             {"SetActorPhysics", lua_SetActorPhysics},
                                             {"SetActorShape", lua_SetActorShape},
                                             {"SetActorBoundingLengths", lua_SetActorBoundingLengths},
                                             {"SetActorBoundingOffset", lua_SetActorBoundingOffset},
                                             {"SetActorMass", lua_SetActorMass},
                                             {"SetActorDamping", lua_SetActorDamping},
                                             {"SetActorSleep", lua_SetActorSleep},
                                             {"SetActorRestitution", lua_SetActorRestitution},
                                             {"SetActorAnisotropicFriction", lua_SetActorAnisotropicFriction},
                                             {"SetActorLinearFactor", lua_SetActorLinearFactor},
                                             {"SetActorAngularFactor", lua_SetActorAngularFactor},
                                             {"AddActorForce", lua_AddActorForce},
                                             {"AddActorForceLocal", lua_AddActorForceLocal},
                                             {"AddActorTorque", lua_AddActorTorque},
                                             {"SetActorLinearVelocity", lua_SetActorLinearVelocity},
                                             {"SetActorLinearVelocityLocal", lua_SetActorLinearVelocityLocal},
                                             {"SetActorAngularVelocity", lua_SetActorAngularVelocity},
                                             {"GetActorPhysics", lua_GetActorPhysics},
                                             {"GetActorBoundingLengths", lua_GetActorBoundingLengths},
                                             {"GetActorBoundingOffset", lua_GetActorBoundingOffset},
                                             {"GetActorShape", lua_GetActorShape},
                                             {"GetActorMass", lua_GetActorMass},
                                             {"GetActorLinearDamping", lua_GetActorLinearDamping},
                                             {"GetActorAngularDamping", lua_GetActorAngularDamping},
                                             {"GetActorLinearSleep", lua_GetActorLinearSleep},
                                             {"GetActorAngularSleep", lua_GetActorAngularSleep},
                                             {"GetActorRestitution", lua_GetActorRestitution},
                                             {"GetActorAnisotropicFriction", lua_GetActorAnisotropicFriction},
                                             {"GetActorLinearFactor", lua_GetActorLinearFactor},
                                             {"GetActorAngularFactor", lua_GetActorAngularFactor},
                                             {"GetActorLinearVelocity", lua_GetActorLinearVelocity},
                                             {"GetActorAngularVelocity", lua_GetActorAngularVelocity},
                                             {"AddActorHingeJoint", lua_AddActorHingeJoint},
                                             {"AddActorBallJoint", lua_AddActorBallJoint},
                                             {"AddActorConeTwistJoint", lua_AddActorConeTwistJoint},
                                             {"GetActorJoint", lua_GetActorJoint},
                                             {"GetActorJointByIndex", lua_GetActorJointByIndex},
                                             {"RemoveActorJoint", lua_RemoveActorJoint},
                                             {"RemoveActorJointByIndex", lua_RemoveActorJointByIndex},
                                             {"RemoveActorJointByObject", lua_RemoveActorJointByObject},
                                             {"SetActorIpo", lua_SetActorIpo},
                                             {"GetActorIpo", lua_GetActorIpo},
                                             {"SetActorIpoFrame", lua_SetActorIpoFrame},
                                             {"PlayActorIpo", lua_PlayActorIpo},
                                             {"LoopActorIpo", lua_LoopActorIpo},
                                             {"StopActorIpo", lua_StopActorIpo},
                                             {"PauseActorIpo", lua_PauseActorIpo},
                                             {"ResumeActorIpo", lua_ResumeActorIpo},
                                             {"GetActorIpoStart", lua_GetActorIpoStart},
                                             {"GetActorIpoEnd", lua_GetActorIpoEnd},
                                             {"GetActorIpoSpeed", lua_GetActorIpoSpeed},
                                             {"GetActorIpoFrame", lua_GetActorIpoFrame},
                                             {"IsActorIpoPlaying", lua_IsActorIpoPlaying},
                                             {"IsActorIpoPaused", lua_IsActorIpoPaused},
                                             {"GetActorCollisionCount", lua_GetActorCollisionCount},
                                             {"GetActorCollision", lua_GetActorCollision},
                                             {"GetActorPropertyCount", lua_GetActorPropertyCount},
                                             {"AddPropertyToActor", lua_AddPropertyToActor},
                                             {"GetActorPropertyByName", lua_GetActorPropertyByName},
                                             {"GetActorPropertyByIndex", lua_GetActorPropertyByIndex},
                                             {"RemoveActorPropertyByName", lua_RemoveActorPropertyByName},
                                             {"RemoveActorPropertyByIndex", lua_RemoveActorPropertyByIndex},
                                             {"RemoveActorPropertyByObject", lua_RemoveActorPropertyByObject},
                                             {"RemoveActorProperties", lua_RemoveActorProperties},
                                             {"SetActorSelected", lua_SetActorSelected},
                                             {"GetActorSelected", lua_GetActorSelected},
                                             {"CreateCamera", lua_CreateCamera},
                                             {"SetCameraViewport", lua_SetCameraViewport},
                                             {"SetCameraOrthoViewport", lua_SetCameraOrthoViewport},
                                             {"SetCameraMode", lua_SetCameraMode},
                                             {"SetCameraFov", lua_SetCameraFov},
                                             {"SetCameraAspect", lua_SetCameraAspect},
                                             {"SetCameraClip", lua_SetCameraClip},
                                             {"GetCameraViewportSize", lua_GetCameraViewportSize},
                                             {"GetCameraViewportOffset", lua_GetCameraViewportOffset},
                                             {"GetCameraOrthoViewportSize", lua_GetCameraOrthoViewportSize},
                                             {"GetCameraOrthoViewportOffset", lua_GetCameraOrthoViewportOffset},
                                             {"GetCameraMode", lua_GetCameraMode},
                                             {"GetCameraFov", lua_GetCameraFov},
                                             {"GetCameraAspect", lua_GetCameraAspect},
                                             {"GetCameraClip", lua_GetCameraClip},
                                             {"GetCameraFarPlaneSize", lua_GetCameraFarPlaneSize},
                                             {"ProjectCameraPoint", lua_ProjectCameraPoint},
                                             {"UnProjectCameraPoint", lua_UnProjectCameraPoint},
                                             {"CreateVertex", lua_CreateVertex},
                                             {"SetVertexPosition", lua_SetVertexPosition},
                                             {"SetVertexNormal", lua_SetVertexNormal},
                                             {"SetVertexTexCoord", lua_SetVertexTexCoord},
                                             {"GetVertexPosition", lua_GetVertexPosition},
                                             {"GetVertexNormal", lua_GetVertexNormal},
                                             {"GetVertexTexCoord", lua_GetVertexTexCoord},
                                             {"GetFaceV1", lua_GetFaceV1},
                                             {"GetFaceV2", lua_GetFaceV2},
                                             {"GetFaceV3", lua_GetFaceV3},
                                             {"CreateMeshData", lua_CreateMeshData},
                                             {"GetMeshDataVertexCount", lua_GetMeshDataVertexCount},
                                             {"GetMeshDataFaceCount", lua_GetMeshDataFaceCount},
                                             {"AddMeshDataVertex", lua_AddMeshDataVertex},
                                             {"AddMeshDataFace", lua_AddMeshDataFace},
                                             {"GetVertexFromMeshData", lua_GetVertexFromMeshData},
                                             {"GetFaceFromMeshData", lua_GetFaceFromMeshData},
                                             {"CreateModelFromMeshData", lua_CreateModelFromMeshData},
                                             {"SetModelName", lua_SetModelName},
                                             {"GetModelName", lua_GetModelName},
                                             {"GetModelFilePath", lua_GetModelFilePath},
                                             {"GetModelVertexCount", lua_GetModelVertexCount},
                                             {"GetModelIndiceCount", lua_GetModelIndiceCount},
                                             {"GetModelBoundingBoxMin", lua_GetModelBoundingBoxMin},
                                             {"GetModelBoundingBoxMax", lua_GetModelBoundingBoxMax},
                                             {"CreateEntity", lua_CreateEntity},
                                             {"GenerateEntityTangents", lua_GenerateEntityTangents},
                                             {"SetEntityScale", lua_SetEntityScale},
                                             {"GetEntityScale", lua_GetEntityScale},
                                             {"SetEntityModel", lua_SetEntityModel},
                                             {"ClearEntityModel", lua_ClearEntityModel},
                                             {"GetEntityModel", lua_GetEntityModel},
                                             {"GetEntityMaterialCount", lua_GetEntityMaterialCount},
                                             {"AddEntityMaterial", lua_AddEntityMaterial},
                                             {"SetEntityMaterial", lua_SetEntityMaterial},
                                             {"RemoveEntityMaterial", lua_RemoveEntityMaterial},
                                             {"GetEntityMaterial", lua_GetEntityMaterial},
                                             {"SetEntityVisible", lua_SetEntityVisible},
                                             {"GetEntityVisible", lua_GetEntityVisible},
                                             {"SetEntityOccluder", lua_SetEntityOccluder},
                                             {"GetEntityOccluder", lua_GetEntityOccluder},
                                             {"SetEntityArmature", lua_SetEntityArmature},
                                             {"SetEntityArmatureFrame", lua_SetEntityArmatureFrame},
                                             {"PlayEntityArmature", lua_PlayEntityArmature},
                                             {"LoopEntityArmature", lua_LoopEntityArmature},
                                             {"StopEntityArmature", lua_StopEntityArmature},
                                             {"PauseEntityArmature", lua_PauseEntityArmature},
                                             {"ResumeEntityArmature", lua_ResumeEntityArmature},
                                             {"GetEntityArmatureStart", lua_GetEntityArmatureStart},
                                             {"GetEntityArmatureEnd", lua_GetEntityArmatureEnd},
                                             {"GetEntityArmatureSpeed", lua_GetEntityArmatureSpeed},
                                             {"GetEntityArmatureFrame", lua_GetEntityArmatureFrame},
                                             {"IsEntityArmaturePlaying", lua_IsEntityArmaturePlaying},
                                             {"IsEntityArmaturePaused", lua_IsEntityArmaturePaused},
                                             {"GetEntityArmature", lua_GetEntityArmature},
                                             {"GetEntityChanged", lua_GetEntityChanged},
                                             {"CreateLight", lua_CreateLight},
                                             {"SetLightType", lua_SetLightType},
                                             {"SetLightColor", lua_SetLightColor},
                                             {"SetLightRange", lua_SetLightRange},
                                             {"SetLightShadows", lua_SetLightShadows},
                                             {"SetLightVisible", lua_SetLightVisible},
                                             {"SetLightCone", lua_SetLightCone},
                                             {"SetLightShaft", lua_SetLightShaft},
                                             {"SetLightShaftSize", lua_SetLightShaftSize},
                                             {"SetLightShaftIntensity", lua_SetLightShaftIntensity},
                                             {"SetLightShaftFadeOff", lua_SetLightShaftFadeOff},
                                             {"GetLightType", lua_GetLightType},
                                             {"GetLightColor", lua_GetLightColor},
                                             {"GetLightRange", lua_GetLightRange},
                                             {"GetLightFadeRange", lua_GetLightFadeRange},
                                             {"GetLightShadows", lua_GetLightShadows},
                                             {"GetLightVisible", lua_GetLightVisible},
                                             {"GetLightInnerCone", lua_GetLightInnerCone},
                                             {"GetLightOuterCone", lua_GetLightOuterCone},
                                             {"GetLightShaft", lua_GetLightShaft},
                                             {"GetLightShaftSize", lua_GetLightShaftSize},
                                             {"GetLightShaftIntensity", lua_GetLightShaftIntensity},
                                             {"GetLightShaftFadeOff", lua_GetLightShaftFadeOff},
                                             {"GetBoneArmature", lua_GetBoneArmature},
                                             {"GetBoneParent", lua_GetBoneParent},
                                             {"GetBoneChildByName", lua_GetBoneChildByName},
                                             {"GetBoneChildById", lua_GetBoneChildById},
                                             {"GetBoneChildByIndex", lua_GetBoneChildByIndex},
                                             {"GetBonePosition", lua_GetBonePosition},
                                             {"GetBoneRotation", lua_GetBoneRotation},
                                             {"GetBoneOrientation", lua_GetBoneOrientation},
                                             {"CreateArmature", lua_CreateArmature},
                                             {"GetBoneFromArmatureByName", lua_GetBoneFromArmatureByName},
                                             {"GetBoneFromArmatureById", lua_GetBoneFromArmatureById},
                                             {"CreateParticles", lua_CreateParticles},
                                             {"GetParticlesName", lua_GetParticlesName},
                                             {"GetParticlesFilePath", lua_GetParticlesFilePath},
                                             {"SetParticlesMaxCount", lua_SetParticlesMaxCount},
                                             {"SetParticlesDrawMode", lua_SetParticlesDrawMode},
                                             {"SetParticlesTexture", lua_SetParticlesTexture},
                                             {"ClearParticlesTexture", lua_ClearParticlesTexture},
                                             {"SetParticlesModel", lua_SetParticlesModel},
                                             {"ClearParticlesModel", lua_ClearParticlesModel},
                                             {"SetParticlesEntity", lua_SetParticlesEntity},
                                             {"ClearParticlesEntity", lua_ClearParticlesEntity},
                                             {"SetParticlesGravity", lua_SetParticlesGravity},
                                             {"SetParticlesSpawnCount", lua_SetParticlesSpawnCount},
                                             {"SetParticlesSpawn", lua_SetParticlesSpawn},
                                             {"SetParticlesSize", lua_SetParticlesSize},
                                             {"SetParticlesSizeGrowth", lua_SetParticlesSizeGrowth},
                                             {"SetParticlesRotation", lua_SetParticlesRotation},
                                             {"SetParticlesRotationGrowth", lua_SetParticlesRotationGrowth},
                                             {"SetParticlesLifeSpan", lua_SetParticlesLifeSpan},
                                             {"SetParticlesFadeSpeed", lua_SetParticlesFadeSpeed},
                                             {"SetParticlesPositionMin", lua_SetParticlesPositionMin},
                                             {"SetParticlesPositionMax", lua_SetParticlesPositionMax},
                                             {"SetParticlesVelocityMin", lua_SetParticlesVelocityMin},
                                             {"SetParticlesVelocityMax", lua_SetParticlesVelocityMax},
                                             {"SetParticlesColorMin", lua_SetParticlesColorMin},
                                             {"SetParticlesColorMax", lua_SetParticlesColorMax},
                                             {"GetParticlesMaxCount", lua_GetParticlesMaxCount},
                                             {"GetParticlesCount", lua_GetParticlesCount},
                                             {"GetParticlesDrawMode", lua_GetParticlesDrawMode},
                                             {"GetParticlesTexture", lua_GetParticlesTexture},
                                             {"GetParticlesModel", lua_GetParticlesModel},
                                             {"GetParticlesEntity", lua_GetParticlesEntity},
                                             {"GetParticlesGravity", lua_GetParticlesGravity},
                                             {"GetParticlesSpawnCount", lua_GetParticlesSpawnCount},
                                             {"GetParticlesSpawn", lua_GetParticlesSpawn},
                                             {"GetParticlesSizeMin", lua_GetParticlesSizeMin},
                                             {"GetParticlesSizeMax", lua_GetParticlesSizeMax},
                                             {"GetParticlesSizeGrowthMin", lua_GetParticlesSizeGrowthMin},
                                             {"GetParticlesSizeGrowthMax", lua_GetParticlesSizeGrowthMax},
                                             {"GetParticlesRotationMin", lua_GetParticlesRotationMin},
                                             {"GetParticlesRotationMax", lua_GetParticlesRotationMax},
                                             {"GetParticlesRotationGrowthMin", lua_GetParticlesRotationGrowthMin},
                                             {"GetParticlesRotationGrowthMax", lua_GetParticlesRotationGrowthMax},
                                             {"GetParticlesLifeSpanMin", lua_GetParticlesLifeSpanMin},
                                             {"GetParticlesLifeSpanMax", lua_GetParticlesLifeSpanMax},
                                             {"GetParticlesFadeSpeedMin", lua_GetParticlesFadeSpeedMin},
                                             {"GetParticlesFadeSpeedMax", lua_GetParticlesFadeSpeedMax},
                                             {"GetParticlesPositionMin", lua_GetParticlesPositionMin},
                                             {"GetParticlesPositionMax", lua_GetParticlesPositionMax},
                                             {"GetParticlesVelocityMin", lua_GetParticlesVelocityMin},
                                             {"GetParticlesVelocityMax", lua_GetParticlesVelocityMax},
                                             {"GetParticlesColorMin", lua_GetParticlesColorMin},
                                             {"GetParticlesColorMax", lua_GetParticlesColorMax},
                                             {"CreateSprite", lua_CreateSprite},
                                             {"SetSpriteMaterial", lua_SetSpriteMaterial},
                                             {"SetSpriteScale", lua_SetSpriteScale},
                                             {"SetSpriteFaceCamera", lua_SetSpriteFaceCamera},
                                             {"GetSpriteMaterial", lua_GetSpriteMaterial},
                                             {"GetSpriteScale", lua_GetSpriteScale},
                                             {"GetSpriteFaceCamera", lua_GetSpriteFaceCamera},
                                             {"SetSpriteVisible", lua_SetSpriteVisible},
                                             {"GetSpriteVisible", lua_GetSpriteVisible},
                                             {"SetSpriteOccluder", lua_SetSpriteOccluder},
                                             {"GetSpriteOccluder", lua_GetSpriteOccluder},
                                             {"GetSceneName", lua_GetSceneName},
                                             {"GetSceneFilePath", lua_GetSceneFilePath},
                                             {"CreateScene", lua_CreateScene},
                                             {"CreateSceneFromFile", lua_CreateSceneFromFile},
                                             {"SaveScene", lua_SaveScene},
                                             {"SetScenePhysics", lua_SetScenePhysics},
                                             {"GetScenePhysics", lua_GetScenePhysics},
                                             {"SetSceneRunScripts", lua_SetSceneRunScripts},
                                             {"GetSceneRunScripts", lua_GetSceneRunScripts},
                                             {"SetSceneDebugDraw", lua_SetSceneDebugDraw},
                                             {"GetSceneDebugDraw", lua_GetSceneDebugDraw},
                                             {"SetSceneOcclusionCulling", lua_SetSceneOcclusionCulling},
                                             {"GetSceneOcclusionCulling", lua_GetSceneOcclusionCulling},
                                             {"SetSceneGravity", lua_SetSceneGravity},
                                             {"GetSceneGravity", lua_GetSceneGravity},
                                             {"SetSceneAmbientColor", lua_SetSceneAmbientColor},
                                             {"GetSceneAmbientColor", lua_GetSceneAmbientColor},
                                             {"SetSceneFog", lua_SetSceneFog},
                                             {"SetSceneFogStart", lua_SetSceneFogStart},
                                             {"SetSceneFogEnd", lua_SetSceneFogEnd},
                                             {"SetSceneFogColor", lua_SetSceneFogColor},
                                             {"GetSceneFog", lua_GetSceneFog},
                                             {"GetSceneFogStart", lua_GetSceneFogStart},
                                             {"GetFogEnd", lua_GetFogEnd},
                                             {"GetFogColor", lua_GetFogColor},
                                             {"GetSceneCameraCount", lua_GetSceneCameraCount},
                                             {"GetSceneEntityCount", lua_GetSceneEntityCount},
                                             {"GetSceneLightCount", lua_GetSceneLightCount},
                                             {"GetSceneArmatureCount", lua_GetSceneArmatureCount},
                                             {"GetSceneParticlesCount", lua_GetSceneParticlesCount},
                                             {"GetSceneSpriteCount", lua_GetSceneSpriteCount},
                                             {"AddSceneCamera", lua_AddSceneCamera},
                                             {"AddSceneEntity", lua_AddSceneEntity},
                                             {"AddSceneLight", lua_AddSceneLight},
                                             {"AddSceneParticles", lua_AddSceneParticles},
                                             {"AddSceneSprite", lua_AddSceneSprite},
                                             {"SetSceneActiveCamera", lua_SetSceneActiveCamera},
                                             {"GetSceneActiveCamera", lua_GetSceneActiveCamera},
                                             {"GetSceneRayCastResult", lua_GetSceneRayCastResult},
                                             {"GetSceneRayCastResults", lua_GetSceneRayCastResults},
                                             {"GetDebugSceneRayCastResult", lua_GetDebugSceneRayCastResult},
                                             {"GetDebugSceneRayCastResults", lua_GetDebugSceneRayCastResults},
                                             {"GetSceneCameraByIndex", lua_GetSceneCameraByIndex},
                                             {"GetSceneEntityByIndex", lua_GetSceneEntityByIndex},
                                             {"GetSceneLightByIndex", lua_GetSceneLightByIndex},
                                             {"GetSceneArmatureByIndex", lua_GetSceneArmatureByIndex},
                                             {"GetSceneParticlesByIndex", lua_GetSceneParticlesByIndex},
                                             {"GetSceneSpriteByIndex", lua_GetSceneSpriteByIndex},
                                             {"GetSceneTexture", lua_GetSceneTexture},
                                             {"GetSceneMaterial", lua_GetSceneMaterial},
                                             {"GetSceneModel", lua_GetSceneModel},
                                             {"GetSceneScript", lua_GetSceneScript},
                                             {"GetSceneCamera", lua_GetSceneCamera},
                                             {"GetSceneEntity", lua_GetSceneEntity},
                                             {"GetSceneLight", lua_GetSceneLight},
                                             {"GetSceneArmature", lua_GetSceneArmature},
                                             {"GetSceneParticles", lua_GetSceneParticles},
                                             {"GetSceneSprite", lua_GetSceneSprite},
                                             {"GetSceneActor", lua_GetSceneActor},
                                             {"RemoveSceneCamera", lua_RemoveSceneCamera},
                                             {"RemoveSceneEntity", lua_RemoveSceneEntity},
                                             {"RemoveSceneLight", lua_RemoveSceneLight},
                                             {"RemoveSceneParticles", lua_RemoveSceneParticles},
                                             {"RemoveSceneSprite", lua_RemoveSceneSprite},
                                             {"RemoveSceneCameraByIndex", lua_RemoveSceneCameraByIndex},
                                             {"RemoveSceneEntityByIndex", lua_RemoveSceneEntityByIndex},
                                             {"RemoveSceneLightByIndex", lua_RemoveSceneLightByIndex},
                                             {"RemoveSceneParticlesByIndex", lua_RemoveSceneParticlesByIndex},
                                             {"RemoveSceneSpriteByIndex", lua_RemoveSceneSpriteByIndex},
                                             {"RemoveSceneCameraByObject", lua_RemoveSceneCameraByObject},
                                             {"RemoveSceneEntityByObject", lua_RemoveSceneEntityByObject},
                                             {"RemoveSceneLightByObject", lua_RemoveSceneLightByObject},
                                             {"RemoveSceneParticlesByObject", lua_RemoveSceneParticlesByObject},
                                             {"RemoveSceneSpriteByObject", lua_RemoveSceneSpriteByObject},
                                             {"RemoveSceneActorByObject", lua_RemoveSceneActorByObject},
                                             {"GetSceneScripts", lua_GetSceneScripts},
                                             {"GetSceneTextures", lua_GetSceneTextures},
                                             {"GetSceneMaterials", lua_GetSceneMaterials},
                                             {"GetSceneModels", lua_GetSceneModels},
                                             {"CreateScript", lua_CreateScript},
                                             {"CreateScriptFromFile", lua_CreateScriptFromFile},
                                             {"SetScriptName", lua_SetScriptName},
                                             {"GetScriptName", lua_GetScriptName},
                                             {"GetScriptFilePath", lua_GetScriptFilePath},
                                             {"SetScriptText", lua_SetScriptText},
                                             {"IsScriptError", lua_IsScriptError},
                                             {"RunString", lua_RunString},
                                             {"RunScript", lua_RunScript},
                                             {"SetAudioVolume", lua_SetAudioVolume},
                                             {"GetAudioVolume", lua_GetAudioVolume},
                                             {"SetAudioRolloff", lua_SetAudioRolloff},
                                             {"GetAudioRolloff", lua_GetAudioRolloff},
                                             {"LoadSound", lua_LoadSound},
                                             {"LoadStreamedSound", lua_LoadStreamedSound},
                                             {"GetSoundFileType", lua_GetSoundFileType},
                                             {"PlaySound", lua_PlaySound},
                                             {"PlayEntitySound", lua_PlayEntitySound},
                                             {"LoopSound", lua_LoopSound},
                                             {"LoopEntitySound", lua_LoopEntitySound},
                                             {"SetSoundVolume", lua_SetSoundVolume},
                                             {"GetSoundVolume", lua_GetSoundVolume},
                                             {"PauseSound", lua_PauseSound},
                                             {"ResumeSound", lua_ResumeSound},
                                             {"StopSound", lua_StopSound},
                                             {"IsSoundPlaying", lua_IsSoundPlaying},
                                             {"IsSoundPaused", lua_IsSoundPaused},
                                             {"GetCollisionActor", lua_GetCollisionActor},
                                             {"GetCollisionPosition", lua_GetCollisionPosition},
                                             {"GetCollisionNormal", lua_GetCollisionNormal},
                                             {"GetCollisionDepth", lua_GetCollisionDepth},
                                             {"GetJointName", lua_GetJointName},
                                             {"GetJointType", lua_GetJointType},
                                             {"GetJointActorA", lua_GetJointActorA},
                                             {"GetJointActorB", lua_GetJointActorB},
                                             {"GetJointPivot", lua_GetJointPivot},
                                             {"GetJointAxis", lua_GetJointAxis},
                                             {"CreateFontFromFile", lua_CreateFontFromFile},
                                             {"GetDefaultFont", lua_GetDefaultFont},
                                             {"GetFontName", lua_GetFontName},
                                             {"GetFontFilePath", lua_GetFontFilePath},
                                             {"GetFontSize", lua_GetFontSize},
                                             {"GetStringWidth", lua_GetStringWidth},
                                             {"GetStringHeight", lua_GetStringHeight},
                                             {"GetGuiObjectName", lua_GetGuiObjectName},
                                             {"GetGuiObjectPosition", lua_GetGuiObjectPosition},
                                             {"GetGuiObjectSize", lua_GetGuiObjectSize},
                                             {"GetGuiObjectColor", lua_GetGuiObjectColor},
                                             {"GetGuiObjectVisible", lua_GetGuiObjectVisible},
                                             {"GetGuiObjectActive", lua_GetGuiObjectActive},
                                             {"GetGuiObjectScript", lua_GetGuiObjectScript},
                                             {"GetGuiObjectEvent", lua_GetGuiObjectEvent},
                                             {"SetGuiObjectPosition", lua_SetGuiObjectPosition},
                                             {"SetGuiObjectColor", lua_SetGuiObjectColor},
                                             {"SetGuiObjectVisible", lua_SetGuiObjectVisible},
                                             {"SetGuiObjectActive", lua_SetGuiObjectActive},
                                             {"SetGuiObjectScript", lua_SetGuiObjectScript},
                                             {"CreateLabel", lua_CreateLabel},
                                             {"GetLabelFont", lua_GetLabelFont},
                                             {"GetLabelText", lua_GetLabelText},
                                             {"SetLabelFont", lua_SetLabelFont},
                                             {"SetLabelText", lua_SetLabelText},
                                             {"CreateButton", lua_CreateButton},
                                             {"GetButtonState", lua_GetButtonState},
                                             {"GetButtonText", lua_GetButtonText},
                                             {"GetButtonFont", lua_GetButtonFont},
                                             {"GetButtonOffTexture", lua_GetButtonOffTexture},
                                             {"GetButtonOverTexture", lua_GetButtonOverTexture},
                                             {"GetButtonOnTexture", lua_GetButtonOnTexture},
                                             {"GetButtonTextColor", lua_GetButtonTextColor},
                                             {"SetButtonTextColor", lua_SetButtonTextColor},
                                             {"SetButtonText", lua_SetButtonText},
                                             {"SetButtonFont", lua_SetButtonFont},
                                             {"SetButtonSize", lua_SetButtonSize},
                                             {"SetButtonOffTexture", lua_SetButtonOffTexture},
                                             {"SetButtonOverTexture", lua_SetButtonOverTexture},
                                             {"SetButtonOnTexture", lua_SetButtonOnTexture},
                                             {"CreatePicture", lua_CreatePicture},
                                             {"GetPictureTexture", lua_GetPictureTexture},
                                             {"GetPictureScale", lua_GetPictureScale},
                                             {"SetPictureTexture", lua_SetPictureTexture},
                                             {"SetPictureScale", lua_SetPictureScale},
                                             {"CreateTextField", lua_CreateTextField},
                                             {"GetTextFieldTexture", lua_GetTextFieldTexture},
                                             {"GetTextFieldFont", lua_GetTextFieldFont},
                                             {"GetTextFieldTextColor", lua_GetTextFieldTextColor},
                                             {"GetTextFieldOffset", lua_GetTextFieldOffset},
                                             {"GetTextFieldText", lua_GetTextFieldText},
                                             {"SetTextFieldFont", lua_SetTextFieldFont},
                                             {"SetTextFieldWidth", lua_SetTextFieldWidth},
                                             {"SetTextFieldTexture", lua_SetTextFieldTexture},
                                             {"SetTextFieldTextColor", lua_SetTextFieldTextColor},
                                             {"SetTextFieldOffset", lua_SetTextFieldOffset},
                                             {"SetTextFieldCursorPosition", lua_SetTextFieldCursorPosition},
                                             {"SetTextFieldText", lua_SetTextFieldText},
                                             {"CreateSlider", lua_CreateSlider},
                                             {"GetSliderBackgroundTexture", lua_GetSliderBackgroundTexture},
                                             {"GetSliderSliderTexture", lua_GetSliderSliderTexture},
                                             {"GetSliderValue", lua_GetSliderValue},
                                             {"SetSliderSize", lua_SetSliderSize},
                                             {"SetSliderBackgroundTexture", lua_SetSliderBackgroundTexture},
                                             {"SetSliderSliderTexture", lua_SetSliderSliderTexture},
                                             {"SetSliderValue", lua_SetSliderValue},
                                             {"CreateScreen", lua_CreateScreen},
                                             {"SetScreenSize", lua_SetScreenSize},
                                             {"GetScreenTexture", lua_GetScreenTexture},
                                             {"SetScreenTexture", lua_SetScreenTexture},
                                             {"SetScreenToTop", lua_SetScreenToTop},
                                             {"ForceScreenFocus", lua_ForceScreenFocus},
                                             {"ReleaseScreenFocus", lua_ReleaseScreenFocus},
                                             {"CreateTextList", lua_CreateTextList},
                                             {"GetTextListFont", lua_GetTextListFont},
                                             {"GetTextListRowCount", lua_GetTextListRowCount},
                                             {"GetTextListItemCount", lua_GetTextListItemCount},
                                             {"GetTextListSelectionIndex", lua_GetTextListSelectionIndex},
                                             {"GetTextListOffset", lua_GetTextListOffset},
                                             {"GetTextListItem", lua_GetTextListItem},
                                             {"GetTextListSelectedItem", lua_GetTextListSelectedItem},
                                             {"GetTextListItemDrag", lua_GetTextListItemDrag},
                                             {"SetTextListFont", lua_SetTextListFont},
                                             {"SetTextListSize", lua_SetTextListSize},
                                             {"AddTextListItem", lua_AddTextListItem},
                                             {"SetTextListItem", lua_SetTextListItem},
                                             {"RemoveTextListItem", lua_RemoveTextListItem},
                                             {"RemoveTextListItems", lua_RemoveTextListItems},
                                             {"SetTextListOffset", lua_SetTextListOffset},
                                             {"SetTextListSelection", lua_SetTextListSelection},
                                             {"SetTextListItemDrag", lua_SetTextListItemDrag},
                                             {"CreateCheckBox", lua_CreateCheckBox},
                                             {"GetCheckBoxState", lua_GetCheckBoxState},
                                             {"GetCheckBoxOffTexture", lua_GetCheckBoxOffTexture},
                                             {"GetCheckBoxOnTexture", lua_GetCheckBoxOnTexture},
                                             {"SetCheckBoxOffTexture", lua_SetCheckBoxOffTexture},
                                             {"SetCheckBoxOnTexture", lua_SetCheckBoxOnTexture},
                                             {"SetCheckBoxState", lua_SetCheckBoxState},
                                             {"CreateGui", lua_CreateGui},
                                             {"AddGuiObject", lua_AddGuiObject},
                                             {"GetGuiObjectByName", lua_GetGuiObjectByName},
                                             {"GetGuiObjectByIndex", lua_GetGuiObjectByIndex},
                                             {"RemoveGuiObjectByName", lua_RemoveGuiObjectByName},
                                             {"RemoveGuiObjectByIndex", lua_RemoveGuiObjectByIndex},
                                             {"RemoveGuiObjectByObject", lua_RemoveGuiObjectByObject},
                                             {"GetGuiTrace", lua_GetGuiTrace},
                                             {"GetGuiFocus", lua_GetGuiFocus},
                                             {"GetGuiActiveTextField", lua_GetGuiActiveTextField},
                                             {"GetGuiDragging", lua_GetGuiDragging},
                                             {"GetGuiDragObject", lua_GetGuiDragObject},
                                             {"GetGuiDragContent", lua_GetGuiDragContent},
                                             {"EmptyGui", lua_EmptyGui},
                                             {"CreateRequest", lua_CreateRequest},
                                             {"SendRequest", lua_SendRequest},
                                             {"GetRequestUrl", lua_GetRequestUrl},
                                             {"SetRequestUrl", lua_SetRequestUrl},
                                             {"GetRequestMethod", lua_GetRequestMethod},
                                             {"SetRequestMethod", lua_SetRequestMethod},
                                             {"GetRequestState", lua_GetRequestState},
                                             {NULL, NULL}};

// TODO Hacky fix to get old lua wrapper behavior
#define luaL_register(L, _, funcs) luaL_setfuncs((L), funcs, 0)

int luaopen_elf(lua_State* L)
{
    lua_pushglobaltable(L);
    // lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, NULL, lua_elf_functions);
    lua_pushstring(L, "NONE");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "FALSE");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "TRUE");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_ESC");
    lua_pushnumber(L, 135);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F1");
    lua_pushnumber(L, 136);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F2");
    lua_pushnumber(L, 137);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F3");
    lua_pushnumber(L, 138);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F4");
    lua_pushnumber(L, 139);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F5");
    lua_pushnumber(L, 140);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F6");
    lua_pushnumber(L, 141);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F7");
    lua_pushnumber(L, 142);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F8");
    lua_pushnumber(L, 143);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F9");
    lua_pushnumber(L, 144);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F10");
    lua_pushnumber(L, 145);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F11");
    lua_pushnumber(L, 146);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F12");
    lua_pushnumber(L, 147);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_UP");
    lua_pushnumber(L, 148);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_DOWN");
    lua_pushnumber(L, 149);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_LEFT");
    lua_pushnumber(L, 150);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_RIGHT");
    lua_pushnumber(L, 151);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_LSHIFT");
    lua_pushnumber(L, 152);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_RSHIFT");
    lua_pushnumber(L, 153);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_LCTRL");
    lua_pushnumber(L, 154);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_RCTRL");
    lua_pushnumber(L, 155);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_LALT");
    lua_pushnumber(L, 156);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_RALT");
    lua_pushnumber(L, 157);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_TAB");
    lua_pushnumber(L, 158);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_ENTER");
    lua_pushnumber(L, 159);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_BACKSPACE");
    lua_pushnumber(L, 160);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_INSERT");
    lua_pushnumber(L, 161);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_DEL");
    lua_pushnumber(L, 162);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_PAGEUP");
    lua_pushnumber(L, 163);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_PAGEDOWN");
    lua_pushnumber(L, 164);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_HOME");
    lua_pushnumber(L, 165);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_END");
    lua_pushnumber(L, 166);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_0");
    lua_pushnumber(L, 167);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_1");
    lua_pushnumber(L, 168);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_2");
    lua_pushnumber(L, 169);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_3");
    lua_pushnumber(L, 170);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_4");
    lua_pushnumber(L, 171);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_5");
    lua_pushnumber(L, 172);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_6");
    lua_pushnumber(L, 173);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_7");
    lua_pushnumber(L, 174);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_8");
    lua_pushnumber(L, 175);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_9");
    lua_pushnumber(L, 176);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_DIVIDE");
    lua_pushnumber(L, 177);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_MULTIPLY");
    lua_pushnumber(L, 178);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_SUBTRACT");
    lua_pushnumber(L, 179);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_ADD");
    lua_pushnumber(L, 180);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_DECIMAL");
    lua_pushnumber(L, 181);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_EQUAL");
    lua_pushnumber(L, 182);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_KP_ENTER");
    lua_pushnumber(L, 183);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_SPACE");
    lua_pushnumber(L, 32);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_0");
    lua_pushnumber(L, 48);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_1");
    lua_pushnumber(L, 49);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_2");
    lua_pushnumber(L, 50);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_3");
    lua_pushnumber(L, 51);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_4");
    lua_pushnumber(L, 52);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_5");
    lua_pushnumber(L, 53);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_6");
    lua_pushnumber(L, 54);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_7");
    lua_pushnumber(L, 55);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_8");
    lua_pushnumber(L, 56);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_9");
    lua_pushnumber(L, 57);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_A");
    lua_pushnumber(L, 65);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_B");
    lua_pushnumber(L, 66);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_C");
    lua_pushnumber(L, 67);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_D");
    lua_pushnumber(L, 68);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_E");
    lua_pushnumber(L, 69);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_F");
    lua_pushnumber(L, 70);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_G");
    lua_pushnumber(L, 71);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_H");
    lua_pushnumber(L, 72);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_I");
    lua_pushnumber(L, 73);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_J");
    lua_pushnumber(L, 74);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_K");
    lua_pushnumber(L, 75);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_L");
    lua_pushnumber(L, 76);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_M");
    lua_pushnumber(L, 77);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_N");
    lua_pushnumber(L, 78);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_O");
    lua_pushnumber(L, 79);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_P");
    lua_pushnumber(L, 80);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_Q");
    lua_pushnumber(L, 81);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_R");
    lua_pushnumber(L, 82);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_S");
    lua_pushnumber(L, 83);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_T");
    lua_pushnumber(L, 84);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_U");
    lua_pushnumber(L, 85);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_V");
    lua_pushnumber(L, 86);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_W");
    lua_pushnumber(L, 87);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_X");
    lua_pushnumber(L, 88);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_Y");
    lua_pushnumber(L, 89);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_Z");
    lua_pushnumber(L, 90);
    lua_settable(L, -3);
    lua_pushstring(L, "UP");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "PRESSED");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "DOWN");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "RELEASED");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "OFF");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "ON");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "OVER");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "BUTTON_LEFT");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "BUTTON_MIDDLE");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "BUTTON_RIGHT");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "TEXTURE");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "MATERIAL");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "MODEL");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "CAMERA");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "ENTITY");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "LIGHT");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "SCENE");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "LIST");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "PAK");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "PAK_INDEX");
    lua_pushnumber(L, 0x0009);
    lua_settable(L, -3);
    lua_pushstring(L, "KEY_EVENT");
    lua_pushnumber(L, 0x000A);
    lua_settable(L, -3);
    lua_pushstring(L, "CHAR_EVENT");
    lua_pushnumber(L, 0x000B);
    lua_settable(L, -3);
    lua_pushstring(L, "CONTEXT");
    lua_pushnumber(L, 0x000C);
    lua_settable(L, -3);
    lua_pushstring(L, "ENGINE");
    lua_pushnumber(L, 0x000D);
    lua_settable(L, -3);
    lua_pushstring(L, "TIMER");
    lua_pushnumber(L, 0x000E);
    lua_settable(L, -3);
    lua_pushstring(L, "IMAGE");
    lua_pushnumber(L, 0x000F);
    lua_settable(L, -3);
    lua_pushstring(L, "POST_PROCESS");
    lua_pushnumber(L, 0x0010);
    lua_settable(L, -3);
    lua_pushstring(L, "SCRIPT");
    lua_pushnumber(L, 0x0011);
    lua_settable(L, -3);
    lua_pushstring(L, "AUDIO_DEVICE");
    lua_pushnumber(L, 0x0012);
    lua_settable(L, -3);
    lua_pushstring(L, "AUDIO_SOURCE");
    lua_pushnumber(L, 0x0013);
    lua_settable(L, -3);
    lua_pushstring(L, "SOUND");
    lua_pushnumber(L, 0x0014);
    lua_settable(L, -3);
    lua_pushstring(L, "BONE");
    lua_pushnumber(L, 0x0015);
    lua_settable(L, -3);
    lua_pushstring(L, "ARMATURE");
    lua_pushnumber(L, 0x0016);
    lua_settable(L, -3);
    lua_pushstring(L, "STRING");
    lua_pushnumber(L, 0x0017);
    lua_settable(L, -3);
    lua_pushstring(L, "FONT");
    lua_pushnumber(L, 0x0018);
    lua_settable(L, -3);
    lua_pushstring(L, "LABEL");
    lua_pushnumber(L, 0x0019);
    lua_settable(L, -3);
    lua_pushstring(L, "BUTTON");
    lua_pushnumber(L, 0x001A);
    lua_settable(L, -3);
    lua_pushstring(L, "PICTURE");
    lua_pushnumber(L, 0x001B);
    lua_settable(L, -3);
    lua_pushstring(L, "TEXT_FIELD");
    lua_pushnumber(L, 0x001C);
    lua_settable(L, -3);
    lua_pushstring(L, "SLIDER");
    lua_pushnumber(L, 0x001D);
    lua_settable(L, -3);
    lua_pushstring(L, "SCREEN");
    lua_pushnumber(L, 0x001E);
    lua_settable(L, -3);
    lua_pushstring(L, "TEXT_LIST");
    lua_pushnumber(L, 0x001F);
    lua_settable(L, -3);
    lua_pushstring(L, "CHECK_BOX");
    lua_pushnumber(L, 0x0020);
    lua_settable(L, -3);
    lua_pushstring(L, "GUI");
    lua_pushnumber(L, 0x0021);
    lua_settable(L, -3);
    lua_pushstring(L, "DIRECTORY_ITEM");
    lua_pushnumber(L, 0x0022);
    lua_settable(L, -3);
    lua_pushstring(L, "DIRECTORY");
    lua_pushnumber(L, 0x0023);
    lua_settable(L, -3);
    lua_pushstring(L, "COLLISION");
    lua_pushnumber(L, 0x0034);
    lua_settable(L, -3);
    lua_pushstring(L, "PHYSICS_OBJECT");
    lua_pushnumber(L, 0x0035);
    lua_settable(L, -3);
    lua_pushstring(L, "PHYSICS_WORLD");
    lua_pushnumber(L, 0x0036);
    lua_settable(L, -3);
    lua_pushstring(L, "JOINT");
    lua_pushnumber(L, 0x0037);
    lua_settable(L, -3);
    lua_pushstring(L, "PARTICLE");
    lua_pushnumber(L, 0x0038);
    lua_settable(L, -3);
    lua_pushstring(L, "PARTICLES");
    lua_pushnumber(L, 0x0039);
    lua_settable(L, -3);
    lua_pushstring(L, "CONFIG");
    lua_pushnumber(L, 0x003A);
    lua_settable(L, -3);
    lua_pushstring(L, "BEZIER_POINT");
    lua_pushnumber(L, 0x003B);
    lua_settable(L, -3);
    lua_pushstring(L, "BEZIER_CURVE");
    lua_pushnumber(L, 0x003C);
    lua_settable(L, -3);
    lua_pushstring(L, "IPO");
    lua_pushnumber(L, 0x003D);
    lua_settable(L, -3);
    lua_pushstring(L, "FRAME_PLAYER");
    lua_pushnumber(L, 0x003E);
    lua_settable(L, -3);
    lua_pushstring(L, "PROPERTY");
    lua_pushnumber(L, 0x003F);
    lua_settable(L, -3);
    lua_pushstring(L, "SCRIPTING");
    lua_pushnumber(L, 0x0040);
    lua_settable(L, -3);
    lua_pushstring(L, "PHYSICS_TRI_MESH");
    lua_pushnumber(L, 0x0041);
    lua_settable(L, -3);
    lua_pushstring(L, "SPRITE");
    lua_pushnumber(L, 0x0042);
    lua_settable(L, -3);
    lua_pushstring(L, "VIDEO_MODE");
    lua_pushnumber(L, 0x0043);
    lua_settable(L, -3);
    lua_pushstring(L, "GENERAL");
    lua_pushnumber(L, 0x0044);
    lua_settable(L, -3);
    lua_pushstring(L, "VERTEX");
    lua_pushnumber(L, 0x0045);
    lua_settable(L, -3);
    lua_pushstring(L, "FACE");
    lua_pushnumber(L, 0x0046);
    lua_settable(L, -3);
    lua_pushstring(L, "MESH_DATA");
    lua_pushnumber(L, 0x0047);
    lua_settable(L, -3);
    lua_pushstring(L, "LIST_PTR");
    lua_pushnumber(L, 0x0048);
    lua_settable(L, -3);
    lua_pushstring(L, "RESOURCES");
    lua_pushnumber(L, 0x0049);
    lua_settable(L, -3);
    lua_pushstring(L, "RENDER_STATION");
    lua_pushnumber(L, 0x004A);
    lua_settable(L, -3);
    lua_pushstring(L, "REQUEST");
    lua_pushnumber(L, 0x004B);
    lua_settable(L, -3);
    lua_pushstring(L, "OBJECT_TYPE_COUNT");
    lua_pushnumber(L, 0x004C);
    lua_settable(L, -3);
    lua_pushstring(L, "PERSPECTIVE");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "ORTHOGRAPHIC");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "BOX");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "SPHERE");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "MESH");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "CAPSULE_X");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "CAPSULE_Y");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "CAPSULE_Z");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "CONE_X");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "CONE_Y");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "CONE_Z");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "HINGE");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "BALL");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "CONE_TWIST");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "POINT_LIGHT");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "SUN_LIGHT");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "SPOT_LIGHT");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "LUMINANCE");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "LUMINANCE_ALPHA");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "RGB");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "RGBA");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "BGR");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "BGRA");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "RGB16F");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "RGB32F");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "RGBA16F");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "RGBA32F");
    lua_pushnumber(L, 0x0009);
    lua_settable(L, -3);
    lua_pushstring(L, "ALPHA32F");
    lua_pushnumber(L, 0x000A);
    lua_settable(L, -3);
    lua_pushstring(L, "DEPTH_COMPONENT");
    lua_pushnumber(L, 0x000B);
    lua_settable(L, -3);
    lua_pushstring(L, "COMPRESSED_RGB");
    lua_pushnumber(L, 0x000C);
    lua_settable(L, -3);
    lua_pushstring(L, "COMPRESSED_RGBA");
    lua_pushnumber(L, 0x000D);
    lua_settable(L, -3);
    lua_pushstring(L, "FLOAT");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "INT");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "UINT");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "SHORT");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "USHORT");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "BYTE");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "UBYTE");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "PROPERTY_INT");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "PROPERTY_FLOAT");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "PROPERTY_STRING");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "PROPERTY_BOOL");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "COLOR_MAP");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "NORMAL_MAP");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "HEIGHT_MAP");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "SPECULAR_MAP");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "DETAIL_MAP");
    lua_pushnumber(L, 0x0010);
    lua_settable(L, -3);
    lua_pushstring(L, "SHADOW_MAP");
    lua_pushnumber(L, 0x0020);
    lua_settable(L, -3);
    lua_pushstring(L, "COLOR_RAMP_MAP");
    lua_pushnumber(L, 0x0040);
    lua_settable(L, -3);
    lua_pushstring(L, "LIGHT_MAP");
    lua_pushnumber(L, 0x0080);
    lua_settable(L, -3);
    lua_pushstring(L, "CUBE_MAP");
    lua_pushnumber(L, 0x0100);
    lua_settable(L, -3);
    lua_pushstring(L, "TRANSPARENT");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "ADD");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "DIR");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "FILE");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "CLICKED");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "VALUE_CHANGED");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "GAIN_FOCUS");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "LOSE_FOCUS");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "CHAR_INPUT");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "SELECTION_CHANGED");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "STATE_CHANGED");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "DROP");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_1");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_2");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_3");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_4");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_5");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_6");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_7");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_8");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_9");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_10");
    lua_pushnumber(L, 0x0009);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_11");
    lua_pushnumber(L, 0x000A);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_12");
    lua_pushnumber(L, 0x000B);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_13");
    lua_pushnumber(L, 0x000C);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_14");
    lua_pushnumber(L, 0x000D);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_15");
    lua_pushnumber(L, 0x000E);
    lua_settable(L, -3);
    lua_pushstring(L, "JOYSTICK_BUTTON_16");
    lua_pushnumber(L, 0x000F);
    lua_settable(L, -3);
    lua_pushstring(L, "LOC_X");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "LOC_Y");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "LOC_Z");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "ROT_X");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "ROT_Y");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "ROT_Z");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "SCALE_X");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "SCALE_Y");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "SCALE_Z");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "QUA_X");
    lua_pushnumber(L, 0x0009);
    lua_settable(L, -3);
    lua_pushstring(L, "QUA_Y");
    lua_pushnumber(L, 0x000A);
    lua_settable(L, -3);
    lua_pushstring(L, "QUA_Z");
    lua_pushnumber(L, 0x000B);
    lua_settable(L, -3);
    lua_pushstring(L, "QUA_W");
    lua_pushnumber(L, 0x000C);
    lua_settable(L, -3);
    lua_pushstring(L, "OGG");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "WAV");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "NO_ERROR");
    lua_pushnumber(L, 0x0000);
    lua_settable(L, -3);
    lua_pushstring(L, "INVALID_FILE");
    lua_pushnumber(L, 0x0001);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_OPEN_FILE");
    lua_pushnumber(L, 0x0002);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_OPEN_DIRECTORY");
    lua_pushnumber(L, 0x0003);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_INITIALIZE");
    lua_pushnumber(L, 0x0004);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_RUN_STRING");
    lua_pushnumber(L, 0x0005);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_RUN_SCRIPT");
    lua_pushnumber(L, 0x0006);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_CREATE");
    lua_pushnumber(L, 0x0007);
    lua_settable(L, -3);
    lua_pushstring(L, "CANT_RESIZE");
    lua_pushnumber(L, 0x0008);
    lua_settable(L, -3);
    lua_pushstring(L, "INVALID_SIZE");
    lua_pushnumber(L, 0x0009);
    lua_settable(L, -3);
    lua_pushstring(L, "UNKNOWN_FORMAT");
    lua_pushnumber(L, 0x000A);
    lua_settable(L, -3);
    lua_pushstring(L, "UNKNOWN_TYPE");
    lua_pushnumber(L, 0x000B);
    lua_settable(L, -3);
    lua_pushstring(L, "INVALID_HANDLE");
    lua_pushnumber(L, 0x000C);
    lua_settable(L, -3);
    lua_pushstring(L, "MISSING_FEATURE");
    lua_pushnumber(L, 0x000D);
    lua_settable(L, -3);
    lua_pushstring(L, "INVALID_MESH");
    lua_pushnumber(L, 0x000E);
    lua_settable(L, -3);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfVec2i_mt");
    luaL_register(L, NULL, lua_elfVec2i_mt);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfVec2f_mt");
    luaL_register(L, NULL, lua_elfVec2f_mt);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfVec3f_mt");
    luaL_register(L, NULL, lua_elfVec3f_mt);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfVec4f_mt");
    luaL_register(L, NULL, lua_elfVec4f_mt);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfColor_mt");
    luaL_register(L, NULL, lua_elfColor_mt);
    lua_pop(L, 1);
    luaL_newmetatable(L, "lua_elfObject_mt");
    luaL_register(L, NULL, lua_elfObject_mt);
    lua_pop(L, 1);
    return 0;
}
