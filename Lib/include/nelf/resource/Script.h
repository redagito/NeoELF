#pragma once

#include "nelf/resource/resourceHeader.h"

struct elfScript
{
    ELF_RESOURCE_HEADER;
    // Path to script file
    // TODO Hot reloading?
    char* filePath = nullptr;

    // Source code
    char* text = nullptr;

    // Script execution resulted in an error
    bool error = false;
};

elfScript* elfCreateScript(const char* name);
elfScript* elfCreateScriptFromFile(const char* name, const char* filePath);

void elfDestroyScript(void* data);

void elfSetScriptName(elfScript* script, const char* name);

const char* elfGetScriptName(elfScript* script);
const char* elfGetScriptFilePath(elfScript* script);

void elfSetScriptText(elfScript* script, const char* text);

bool elfIsScriptError(elfScript* script);