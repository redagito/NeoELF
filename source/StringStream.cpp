#include "nelf/StringStream.h"

#include <cstdlib>
#include <cstring>

#include "nelf/String.h"

char* elfReadSstNext(const char* text, int* pos, const char* sep)
{
    int start, end;
    char* str;

    start = *pos;

    while (elfRFindCharFromString(text[start], sep) != -1 && text[start] != '\0')
    {
        (*pos)++;
        start++;
    }

    end = start;

    while (elfRFindCharFromString(text[end], sep) == -1 && text[end] != '\0')
    {
        (*pos)++;
        end++;
    }

    if (start == end)
        return nullptr;

    str = (char*)malloc(sizeof(char) * (end - start + 1));
    memcpy(str, &text[start], sizeof(char) * (end - start));
    str[end - start] = '\0';

    return str;
}

char* elfReadSstText(const char* text, int* pos)
{
    int start, end;
    char* str;

    start = *pos;

    while (text[start] != '\"' && text[start] != '\0')
    {
        (*pos)++;
        start++;
    }

    if (text[start] == '\"')
    {
        end = ++start;
        (*pos)++;
    }
    else
    {
        end = start;
    }

    while (text[end] != '\"' && text[end] != '\0')
    {
        (*pos)++;
        end++;
    }

    if (text[end] == '\"')
        (*pos)++;

    if (start == end)
        return NULL;

    str = (char*)malloc(sizeof(char) * (end - start + 1));
    memcpy(str, &text[start], sizeof(char) * (end - start));
    str[end - start] = '\0';

    return str;
}

char* elfReadSstString(const char* text, int* pos) { return elfReadSstNext(text, pos, " \t\n\r"); }

float elfReadSstFloat(const char* text, int* pos)
{
    char* str;
    float val;

    str = elfReadSstNext(text, pos, " \t\n\r");
    val = (float)atof(str);
    free(str);

    return val;
}

void elfReadSstFloats(const char* text, int* pos, int n, float* params)
{
    int i;
    char* str;

    for (i = 0; i < n && (str = elfReadSstNext(text, pos, " \t\n\r")); i++)
    {
        params[i] = (float)atof(str);
        free(str);
    }
}

int elfReadSstInt(const char* text, int* pos)
{
    char* str;
    int val;

    str = elfReadSstNext(text, pos, " \t\n\r");
    val = atoi(str);
    free(str);

    return val;
}

void elfReadSstInts(const char* text, int* pos, int n, int* params)
{
    int i;
    char* str;

    for (i = 0; i < n && (str = elfReadSstNext(text, pos, " \t\n\r")); i++)
    {
        params[i] = atoi(str);
        free(str);
    }
}

bool elfReadSstBool(const char* text, int* pos)
{
    char* str = nullptr;
    bool result;

    str = elfReadSstNext(text, pos, " \t\n\r");

    // Check for supported false strings
    // TODO Default should probably be false
    //      and test for true string instead
    if (strcmp(str, "FALSE") == 0)
        result = false;
    else if (strcmp(str, "False") == 0)
        result = false;
    else if (strcmp(str, "false") == 0)
        result = false;
    else
        result = true;

    free(str);

    return result;
}