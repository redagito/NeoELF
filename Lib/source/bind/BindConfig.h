#pragma once

#include "bind/BindUtil.h"

int lua_ReadConfig(lua_State* L);

int lua_SetConfigWindowSize(lua_State* L);
int lua_SetConfigMultisamples(lua_State* L);
int lua_SetConfigFullscreen(lua_State* L);
int lua_SetConfigTextureCompress(lua_State* L);
int lua_SetConfigTextureAnisotropy(lua_State* L);
int lua_SetConfigShadowMapSize(lua_State* L);
int lua_SetConfigStart(lua_State* L);
int lua_SetConfigLogPath(lua_State* L);

int lua_GetConfigWindowSize(lua_State* L);
int lua_GetConfigMultisamples(lua_State* L);
int lua_GetConfigFullscreen(lua_State* L);
int lua_GetConfigTextureCompress(lua_State* L);
int lua_GetConfigTextureAnisotropy(lua_State* L);
int lua_GetConfigShadowMapSize(lua_State* L);
int lua_GetConfigStart(lua_State* L);
int lua_GetConfigLogPath(lua_State* L);