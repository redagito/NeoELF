#include "nelf/Log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

// TODO Move stuff from general to here
#include "nelf/General.h"

// TODO This should either receive a file name for logging
//      or just provide an additional set for the log file name
//      but it should be managed here instead of general
void elfStartLog()
{
    FILE* file;

    file = fopen(gen->log, "w");
    if (file)
        fclose(file);
}

void elfLogWrite(const char* fmt, ...)
{
    va_list args;
    FILE* file;

    file = fopen(gen->log, "a");
    if (file)
    {
        va_start(args, fmt);
        vfprintf(file, fmt, args);
        va_end(args);

        fclose(file);
    }

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void elfSetError(int code, const char* fmt, ...)
{
    va_list args;
    FILE* file;
    int len = -1;

    file = fopen(gen->log, "a");
    if (file)
    {
        va_start(args, fmt);
        len = vfprintf(file, fmt, args);
        va_end(args);

        fclose(file);
    }

    if (len > 0)
    {
        if (gen->errStr)
            free(gen->errStr);
        gen->errStr = (char*)malloc(sizeof(char) * (len + 1));

        va_start(args, fmt);
        vsprintf(gen->errStr, fmt, args);
        va_end(args);

        gen->errStr[len] = '\0';
    }

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    gen->errCode = code;
}

void elfWriteLogLine(const char* str)
{
    FILE* file;

    file = fopen(gen->log, "a");
    if (file)
    {
        fprintf(file, "%s\n", (char*)str);
        fclose(file);
    }
}