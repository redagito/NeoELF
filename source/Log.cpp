#include "nelf/Log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Path to log file
static char* logFileName = nullptr;

// Last error that happened
static char* errStr = nullptr;

// Last error code
static int errCode = 0;

// Keep file open for appending
static FILE* fileHandle = nullptr;

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

    if (logFileName)
        free(logFileName);

    logFileName = (char*)malloc(sizeof(char) * (strlen(filePath) + 1));
    memcpy(log, filePath, sizeof(char) * strlen(filePath));
    logFileName[strlen(filePath)] = '\0';
}

void elfInitLog()
{
    if (logFileName != nullptr)
    {
        printf("error: log system already initialized");
        return;
    }

    logFileName = (char*)malloc(sizeof(char) * 8);
    memcpy(log, "elf.log", sizeof(char) * 7);
    logFileName[7] = '\0';
}

void elfDeinitLog()
{
    if (logFileName != nullptr)
        free(logFileName);
    if (errStr != nullptr)
        free(errStr);
    if (fileHandle != nullptr)
        fclose(fileHandle);

    logFileName = nullptr;
    fileHandle = nullptr;
    errStr = nullptr;
    errCode = 0;
}

void elfStartLog()
{
    if (fileHandle)
        fclose(fileHandle);

    // Make sure log exists
    fileHandle = fopen(logFileName, "w");

    if (fileHandle)
    {
        // Reopen for appending
        fclose(fileHandle);
        fileHandle = fopen(logFileName, "a");
    }
}

void elfLogWrite(const char* fmt, ...)
{
    va_list args;
    if (fileHandle)
    {
        va_start(args, fmt);
        vfprintf(fileHandle, fmt, args);
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

    if (fileHandle)
    {
        va_start(args, fmt);
        len = vfprintf(fileHandle, fmt, args);
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
    if (fileHandle)
    {
        fprintf(fileHandle, "%s\n", str);
    }
    printf("%s\n", str);
}

const char* elfGetErrorString() { return errStr; }

int elfGetError() { return errCode; }