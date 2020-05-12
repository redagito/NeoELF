#include "bind/BindAudio.h"

#include "bind/BindUtil.h"
#include "nelf/Object.h"
#include "nelf/audio/AudioDevice.h"
#include "nelf/audio/AudioSource.h"
#include "nelf/audio/Sound.h"
#include "nelf/objectType.h"

int lua_SetAudioVolume(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetAudioVolume", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetAudioVolume", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetAudioVolume(arg0);
    return 0;
}

int lua_GetAudioVolume(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetAudioVolume", lua_gettop(L), 0);
    }
    result = elfGetAudioVolume();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_SetAudioRolloff(lua_State* L)
{
    float arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "SetAudioRolloff", lua_gettop(L), 1);
    }
    if (!lua_isnumber(L, 1))
    {
        return lua_fail_arg(L, "SetAudioRolloff", 1, "number");
    }
    arg0 = (float)lua_tonumber(L, 1);
    elfSetAudioRolloff(arg0);
    return 0;
}

int lua_GetAudioRolloff(lua_State* L)
{
    float result;
    if (lua_gettop(L) != 0)
    {
        return lua_fail_arg_count(L, "GetAudioRolloff", lua_gettop(L), 0);
    }
    result = elfGetAudioRolloff();
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_LoadSound(lua_State* L)
{
    elfSound* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "LoadSound", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "LoadSound", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfLoadSound(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_LoadStreamedSound(lua_State* L)
{
    elfSound* result;
    const char* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "LoadStreamedSound", lua_gettop(L), 1);
    }
    if (!lua_isstring(L, 1))
    {
        return lua_fail_arg(L, "LoadStreamedSound", 1, "string");
    }
    arg0 = lua_tostring(L, 1);
    result = elfLoadStreamedSound(arg0);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_GetSoundFileType(lua_State* L)
{
    int result;
    elfSound* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSoundFileType", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SOUND)
    {
        return lua_fail_arg(L, "GetSoundFileType", 1, "elfSound");
    }
    arg0 = (elfSound*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSoundFileType(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_PlaySound(lua_State* L)
{
    elfAudioSource* result;
    elfSound* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "PlaySound", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SOUND)
    {
        return lua_fail_arg(L, "PlaySound", 1, "elfSound");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "PlaySound", 2, "number");
    }
    arg0 = (elfSound*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfPlaySound(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_PlayEntitySound(lua_State* L)
{
    elfAudioSource* result;
    elfEntity* arg0;
    elfSound* arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "PlayEntitySound", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "PlayEntitySound", 1, "elfEntity");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SOUND)
    {
        return lua_fail_arg(L, "PlayEntitySound", 2, "elfSound");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "PlayEntitySound", 3, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfSound*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = (float)lua_tonumber(L, 3);
    result = elfPlayEntitySound(arg0, arg1, arg2);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_LoopSound(lua_State* L)
{
    elfAudioSource* result;
    elfSound* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "LoopSound", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_SOUND)
    {
        return lua_fail_arg(L, "LoopSound", 1, "elfSound");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "LoopSound", 2, "number");
    }
    arg0 = (elfSound*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    result = elfLoopSound(arg0, arg1);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_LoopEntitySound(lua_State* L)
{
    elfAudioSource* result;
    elfEntity* arg0;
    elfSound* arg1;
    float arg2;
    if (lua_gettop(L) != 3)
    {
        return lua_fail_arg_count(L, "LoopEntitySound", lua_gettop(L), 3);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_ENTITY)
    {
        return lua_fail_arg(L, "LoopEntitySound", 1, "elfEntity");
    }
    if (!lua_isuserdata(L, 2) || ((lua_elf_userdata*)lua_touserdata(L, 2))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 2))->object) != ELF_SOUND)
    {
        return lua_fail_arg(L, "LoopEntitySound", 2, "elfSound");
    }
    if (!lua_isnumber(L, 3))
    {
        return lua_fail_arg(L, "LoopEntitySound", 3, "number");
    }
    arg0 = (elfEntity*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (elfSound*)((lua_elfObject*)lua_touserdata(L, 2))->object;
    arg2 = (float)lua_tonumber(L, 3);
    result = elfLoopEntitySound(arg0, arg1, arg2);
    if (result)
        lua_create_elfObject(L, (elfObject*)result);
    else
        lua_pushnil(L);
    return 1;
}

int lua_SetSoundVolume(lua_State* L)
{
    elfAudioSource* arg0;
    float arg1;
    if (lua_gettop(L) != 2)
    {
        return lua_fail_arg_count(L, "SetSoundVolume", lua_gettop(L), 2);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "SetSoundVolume", 1, "elfAudioSource");
    }
    if (!lua_isnumber(L, 2))
    {
        return lua_fail_arg(L, "SetSoundVolume", 2, "number");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    arg1 = (float)lua_tonumber(L, 2);
    elfSetSoundVolume(arg0, arg1);
    return 0;
}

int lua_GetSoundVolume(lua_State* L)
{
    float result;
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "GetSoundVolume", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "GetSoundVolume", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfGetSoundVolume(arg0);
    lua_pushnumber(L, (lua_Number)result);
    return 1;
}

int lua_PauseSound(lua_State* L)
{
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "PauseSound", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "PauseSound", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfPauseSound(arg0);
    return 0;
}

int lua_ResumeSound(lua_State* L)
{
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "ResumeSound", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "ResumeSound", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfResumeSound(arg0);
    return 0;
}

int lua_StopSound(lua_State* L)
{
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "StopSound", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "StopSound", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    elfStopSound(arg0);
    return 0;
}

int lua_IsSoundPlaying(lua_State* L)
{
    unsigned char result;
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsSoundPlaying", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "IsSoundPlaying", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsSoundPlaying(arg0);
    lua_pushboolean(L, result);
    return 1;
}

int lua_IsSoundPaused(lua_State* L)
{
    unsigned char result;
    elfAudioSource* arg0;
    if (lua_gettop(L) != 1)
    {
        return lua_fail_arg_count(L, "IsSoundPaused", lua_gettop(L), 1);
    }
    if (!lua_isuserdata(L, 1) || ((lua_elf_userdata*)lua_touserdata(L, 1))->type != LUA_ELF_OBJECT ||
        elfGetObjectType(((lua_elfObject*)lua_touserdata(L, 1))->object) != ELF_AUDIO_SOURCE)
    {
        return lua_fail_arg(L, "IsSoundPaused", 1, "elfAudioSource");
    }
    arg0 = (elfAudioSource*)((lua_elfObject*)lua_touserdata(L, 1))->object;
    result = elfIsSoundPaused(arg0);
    lua_pushboolean(L, result);
    return 1;
}