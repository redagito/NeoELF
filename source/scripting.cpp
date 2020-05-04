
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blendelf.h"
#include "gfx.h"
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include "types.h"

int luaopen_elf(lua_State* L);

struct elfScripting
{
    ELF_OBJECT_HEADER;
    struct lua_State* L;
};

elfScripting* scr = NULL;

elfScripting* elfCreateScripting()
{
    elfScripting* scripting;

    scripting = (elfScripting*)malloc(sizeof(elfScripting));
    memset(scripting, 0x0, sizeof(elfScripting));
    scripting->objType = ELF_SCRIPTING;
    scripting->objDestr = elfDestroyScripting;

    scripting->L = lua_open();
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

void elfDestroyScripting(void* data)
{
    elfScripting* scripting = (elfScripting*)data;

    if (scripting->L)
        lua_close(scripting->L);

    free(scripting);

    elfDecObj(ELF_SCRIPTING);
}

unsigned char elfInitScripting()
{
    if (scr)
    {
        elfLogWrite("warning: cannot initialize scripting twice\n");
        return ELF_TRUE;
    }

    scr = elfCreateScripting();
    if (!scr)
        return ELF_FALSE;

    elfIncRef((elfObject*)scr);

    return ELF_TRUE;
}

void elfUpdateScripting()
{
    if (scr)
        lua_gc(scr->L, LUA_GCCOLLECT, 0);
}

void elfDeinitScripting()
{
    if (!scr)
        return;

    elfDecRef((elfObject*)scr);
    scr = NULL;
}

unsigned char elfRunString(const char* str)
{
    int err;

    if (!scr || !str)
        return ELF_FALSE;

    err = luaL_dostring(scr->L, str);
    if (err)
    {
        elfSetError(ELF_CANT_RUN_STRING, "error: can't run string \"%s\"\n%s\n", str, lua_tostring(scr->L, -1));
        return ELF_FALSE;
    }

    return ELF_TRUE;
}

unsigned char elfRunScript(elfScript* script)
{
    int err;

    if (!scr || !script->text || script->error)
        return ELF_FALSE;

    err = luaL_dostring(scr->L, script->text);
    if (err)
    {
        elfSetError(ELF_CANT_RUN_SCRIPT, "error: can't run script \"%s\"\n%s\n", script->name,
                    lua_tostring(scr->L, -1));

        script->error = ELF_TRUE;
        return ELF_FALSE;
    }

    return ELF_TRUE;
}
