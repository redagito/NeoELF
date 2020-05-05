#pragma once

// Version info for the product

const char* elfGetProgramName();
const char* elfGetPlatform();

int elfGetVersionMajor();
int elfGetVersionMinor();
int elfGetVersionPatch();

// Basically pre-release id?
const char* elfGetVersionRelease();

// Full version
// Includes program name, version numbers, release and platform info
const char* elfGetVersion();