#include "bind/BindUtil.h"

int lua_fail_with_backtrace(lua_State* L, const char* fmt, ...)
{
    va_list argp;
    lua_Debug ar;
    int i;
    int j;

    va_start(argp, fmt);

    i = 1;
    j = 0;
    while (lua_getstack(L, i, &ar))
    {
        if (j > 0)
        {
            lua_pushfstring(L, "\n");
            j += 1;
        }
        lua_getinfo(L, "Sl", &ar);
        if (ar.currentline > 0)
        {
            if (j == 0)
            {
                lua_pushfstring(L, "%s:%d: ", ar.short_src, ar.currentline);
                lua_pushvfstring(L, fmt, argp);
                j += 1;
            }
            else
            {
                lua_pushfstring(L, "%s:%d ", ar.short_src, ar.currentline);
            }
        }
        else
        {
            lua_pushliteral(L, ""); /* else, no information available... */
        }

        i += 1;
        j += 1;
    }

    va_end(argp);

    lua_concat(L, j);

    return lua_error(L);
}

int lua_fail_arg_count(lua_State* L, const char* func_name, int a, int b)
{
    return lua_fail_with_backtrace(L, "%s: Got %d arguments instead of %d", func_name, a, b);
}

int lua_fail_arg(lua_State* L, const char* func_name, int idx, const char* etype)
{
    return lua_fail_with_backtrace(L, "%s: Argument %d should be of type %s", func_name, idx, etype);
}