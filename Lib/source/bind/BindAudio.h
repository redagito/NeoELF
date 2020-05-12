#pragma once

#include <lua.hpp>

// Lua to C++ bindings for the Audio module

// elfAudioDevice
int lua_SetAudioVolume(lua_State* L);
int lua_GetAudioVolume(lua_State* L);
int lua_SetAudioRolloff(lua_State* L);
int lua_GetAudioRolloff(lua_State* L);

// elfSound
int lua_LoadSound(lua_State* L);
int lua_LoadStreamedSound(lua_State* L);
int lua_GetSoundFileType(lua_State* L);
int lua_PlaySound(lua_State* L);
int lua_PlayEntitySound(lua_State* L);
int lua_LoopSound(lua_State* L);
int lua_LoopEntitySound(lua_State* L);

// elfAudioSource
int lua_SetSoundVolume(lua_State* L);
int lua_GetSoundVolume(lua_State* L);
int lua_PauseSound(lua_State* L);
int lua_ResumeSound(lua_State* L);
int lua_StopSound(lua_State* L);
int lua_IsSoundPlaying(lua_State* L);
int lua_IsSoundPaused(lua_State* L);