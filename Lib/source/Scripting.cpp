#include "nelf/Scripting.h"

#include <cstdlib>
#include <cstring>
#include <lua.hpp>

#include "nelf/General.h"
#include "nelf/Log.h"
#include "nelf/Object.h"
#include "nelf/bind/Bind.h"
#include "nelf/errorCode.h"
#include "nelf/objectType.h"
#include "nelf/resource/Script.h"

struct elfScripting
{
    ELF_OBJECT_HEADER;
    lua_State* L = nullptr;
};

static elfScripting* scr = nullptr;

static void elfDestroyScripting(void* data)
{
    elfScripting* scripting = (elfScripting*)data;

    if (scripting->L)
        lua_close(scripting->L);

    free(scripting);

    elfDecObj(ELF_SCRIPTING);
}

static elfScripting* elfCreateScripting()
{
    elfScripting* scripting;

    scripting = (elfScripting*)malloc(sizeof(elfScripting));
    memset(scripting, 0x0, sizeof(elfScripting));
    scripting->objType = ELF_SCRIPTING;
    scripting->objDestr = elfDestroyScripting;

    // scripting->L = lua_open();
    scripting->L = luaL_newstate();

    if (!scripting->L)
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: failed to initialize lua\n");
        elfDestroyScripting(scripting);
        return NULL;
    }
    luaL_openlibs(scripting->L);
    luaopen_elf(scripting->L);

    elfIncObj(ELF_SCRIPTING);

    return scripting;
}

bool elfInitScripting()
{
    if (scr)
    {
        elfLogWrite("warning: cannot initialize scripting twice\n");
        return true;
    }

    scr = elfCreateScripting();
    if (!scr)
        return false;

    elfIncRef((elfObject*)scr);

    return true;
}

void elfUpdateScripting()
{
    if (scr)
        lua_gc(scr->L, LUA_GCCOLLECT, 0);
}

void elfDeinitScripting()
{
    if (scr == nullptr)
        return;

    elfDecRef((elfObject*)scr);
    scr = nullptr;
}

bool elfRunString(const char* str)
{
    if (scr == nullptr)
        return false;
    if (str == nullptr)
        return false;

    int err = luaL_dostring(scr->L, str);
    if (err)
    {
        elfSetError(ELF_CANT_RUN_STRING, "error: can't run string \"%s\"\n%s\n", str, lua_tostring(scr->L, -1));
        return false;
    }

    return true;
}

bool elfRunScript(elfScript* script)
{
    if (scr == nullptr)
        return false;
    if (script == nullptr)
        return false;
    if (script->text == nullptr)
        return false;
    if (script->error)
        return false;

    int err = luaL_dostring(scr->L, script->text);
    if (err)
    {
        elfSetError(ELF_CANT_RUN_SCRIPT, "error: can't run script \"%s\"\n%s\n", script->name,
                    lua_tostring(scr->L, -1));

        script->error = true;
        return false;
    }

    return true;
}
