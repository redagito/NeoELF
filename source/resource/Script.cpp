#include "nelf/resource/Script.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/String.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"
#include "nelf/resource/Resources.h"

elfScript* elfCreateScript(const char* name)
{
    elfScript* script;

    script = (elfScript*)malloc(sizeof(elfScript));
    memset(script, 0x0, sizeof(elfScript));
    script->objType = ELF_SCRIPT;
    script->objDestr = elfDestroyScript;

    script->id = ++res->scriptIdCounter;

    if (name)
        script->name = elfCreateString(name);

    elfIncObj(ELF_SCRIPT);

    return script;
}

elfScript* elfCreateScriptFromFile(const char* name, const char* filePath)
{
    elfScript* script = NULL;
    FILE* file;
    int length;
    char* text;

    file = fopen(filePath, "r");
    if (!file)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (length > 0)
    {
        text = (char*)malloc(sizeof(char) * length + 1);
        memset(text, 0x0, sizeof(char) * length + 1);
        fread(text, sizeof(char), length, file);

        script = elfCreateScript(name);

        script->filePath = elfCreateString(filePath);

        elfSetScriptText(script, text);

        free(text);
    }

    fclose(file);

    return script;
}

void elfDestroyScript(void* data)
{
    elfScript* script = (elfScript*)data;

    if (script->name)
        elfDestroyString(script->name);
    if (script->filePath)
        elfDestroyString(script->filePath);
    if (script->text)
        elfDestroyString(script->text);

    elfDecObj(ELF_SCRIPT);

    free(script);
}

void elfSetScriptName(elfScript* script, const char* name)
{
    if (script->name)
        elfDestroyString(script->name);
    script->name = elfCreateString(name);
}

const char* elfGetScriptName(elfScript* script) { return script->name; }

const char* elfGetScriptFilePath(elfScript* script) { return script->filePath; }

void elfSetScriptText(elfScript* script, const char* text)
{
    if (script->text)
        elfDestroyString(script->text);
    script->text = NULL;
    if (text)
        script->text = elfCreateString(text);
    script->error = false;
}

bool elfIsScriptError(elfScript* script) { return script->error; }