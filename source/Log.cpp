#include "nelf/Log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Path to log file
static char* log = nullptr;

// Last error that happened
static char* errStr = nullptr;

// Last error code
static int errCode = 0;

// Keep file open for appending
static FILE* file = nullptr;

void elfSetLogFilePath(const char* filePath)
{
    FILE* file = fopen(filePath, "a");
    if (!file)
    {
        file = fopen(filePath, "w");
        if (!file)
        {
            printf("error: can't open log file \"%s\", reverting to default\n", filePath);
            return;
        }
    }

    if (log)
        free(log);

    log = (char*)malloc(sizeof(char) * (strlen(filePath) + 1));
    memcpy(log, filePath, sizeof(char) * strlen(filePath));
    log[strlen(filePath)] = '\0';
}

void elfInitLog()
{
    if (log != nullptr)
    {
        printf("error: log system already initialized");
        return;
    }

    log = (char*)malloc(sizeof(char) * 8);
    memcpy(log, "elf.log", sizeof(char) * 7);
    log[7] = '\0';
}

void elfDeinitLog()
{
    if (log != nullptr)
        free(log);
    if (errStr != nullptr)
        free(errStr);
    if (file != nullptr)
        fclose(file);
    log = nullptr;
    errStr = nullptr;
    errCode = 0;
}

void elfStartLog()
{
    if (file)
        fclose(file);

    // Make sure log exists
    file = fopen(log, "w");

    if (file)
    {
        // Reopen for appending
        fclose(file);
        file = fopen(log, "a");
    }
}

void elfLogWrite(const char* fmt, ...)
{
    va_list args;
    if (file)
    {
        va_start(args, fmt);
        vfprintf(file, fmt, args);
        va_end(args);
    }

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void elfSetError(int code, const char* fmt, ...)
{
    va_list args;
    int len = -1;

    if (file)
    {
        va_start(args, fmt);
        len = vfprintf(file, fmt, args);
        va_end(args);
    }

    if (len > 0)
    {
        if (errStr)
            free(errStr);
        errStr = (char*)malloc(sizeof(char) * (len + 1));

        va_start(args, fmt);
        vsprintf(errStr, fmt, args);
        va_end(args);

        errStr[len] = '\0';
    }

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    errCode = code;
}

void elfWriteLogLine(const char* str)
{
    if (file)
    {
        fprintf(file, "%s\n", str);
    }
    printf("%s\n", str);
}

const char* elfGetErrorString() { return errStr; }

int elfGetError() { return errCode; }