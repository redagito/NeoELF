#include "nelf/String.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"

char* elfCreateString(const char* str)
{
    if (!str)
        return nullptr;

    char* nstr = (char*)malloc(strlen(str) + 1);
    memset(nstr, 0x0, sizeof(char) * (strlen(str) + 1));

    memcpy(nstr, str, sizeof(char) * strlen(str));

    elfIncObj(ELF_STRING);

    return nstr;
}

void elfDestroyString(char* str)
{
    free(str);

    elfDecObj(ELF_STRING);
}

elfString* elfCreateStringObject()
{
    elfString* string;

    string = (elfString*)malloc(sizeof(elfString));
    memset(string, 0x0, sizeof(elfString));
    string->objType = ELF_STRING;
    string->objDestr = elfDestroyStringObject;

    elfIncObj(ELF_STRING);

    return string;
}

void elfDestroyStringObject(void* data)
{
    elfString* string = (elfString*)data;

    if (string->str)
        elfDestroyString(string->str);

    free(string);

    elfDecObj(ELF_STRING);
}

char* elfRemoveCharFromString(char* str, int idx)
{
    char* nstr;

    if (!strlen(str) || idx > (int)strlen(str) - 1)
        return NULL;

    nstr = (char*)malloc(sizeof(char) * (strlen(str)));

    if (sizeof(char) * idx > 0)
        memcpy(nstr, str, sizeof(char) * idx);
    if (sizeof(char) * (strlen(str) - idx) > 0)
        memcpy(&nstr[idx], &str[idx + 1], sizeof(char) * (strlen(str) - idx));

    elfIncObj(ELF_STRING);

    return nstr;
}

char* elfAppendCharToString(char* str, char c)
{
    char* nstr;

    nstr = (char*)malloc(sizeof(char) * (strlen(str) + 2));
    memcpy(nstr, str, sizeof(char) * strlen(str));
    nstr[strlen(str)] = c;
    nstr[strlen(str) + 1] = '\0';

    elfIncObj(ELF_STRING);

    return nstr;
}

char* elfInsertCharToString(char* str, int idx, char c)
{
    char* nstr;

    if (idx < 0 || idx > (int)strlen(str))
        return NULL;

    nstr = (char*)malloc(sizeof(char) * (strlen(str) + 2));

    if (sizeof(char) * idx > 0)
        memcpy(nstr, str, sizeof(char) * idx);
    if (sizeof(char) * (strlen(str) - idx) > 0)
        memcpy(&nstr[idx + 1], &str[idx], sizeof(char) * (strlen(str) - idx));
    nstr[idx] = c;
    nstr[strlen(str) + 1] = '\0';

    elfIncObj(ELF_STRING);

    return nstr;
}

char* elfMergeStrings(const char* str1, const char* str2)
{
    char* nstr;

    if (!str1 || !str2)
        return NULL;

    nstr = (char*)malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));

    memcpy(nstr, str1, sizeof(char) * strlen(str1));
    memcpy(&nstr[strlen(str1)], str2, sizeof(char) * strlen(str2));
    nstr[strlen(str1) + strlen(str2)] = '\0';

    elfIncObj(ELF_STRING);

    return nstr;
}

char* elfMergeStrings3(const char* str1, const char* str2, const char* str3)
{
    char* nstr;
    int bytes;

    if (!str1 || !str2 || !str3 || (!strlen(str1) && !strlen(str2) && !strlen(str3)))
        return NULL;

    bytes = strlen(str1) + strlen(str2) + strlen(str3) + 1;

    nstr = (char*)malloc(sizeof(char) * bytes);
    memset(nstr, 0x0, sizeof(char) * bytes);

    memcpy(nstr, str1, sizeof(char) * strlen(str1));
    memcpy(&nstr[strlen(str1)], str2, sizeof(char) * strlen(str2));
    memcpy(&nstr[strlen(str1) + strlen(str2)], str3, sizeof(char) * strlen(str3));

    elfIncObj(ELF_STRING);

    return nstr;
}

char* elfSubString(char* str, int start, int len)
{
    char* nstr;

    if (start + len < 1)
    {
        nstr = (char*)malloc(sizeof(char) * 1);
        nstr[0] = '\0';
        elfIncObj(ELF_STRING);
        return nstr;
    }

    if (start < 0 || start > (int)strlen(str) - 1 || len < 0 || start + len > (int)strlen(str))
    {
        nstr = (char*)malloc(sizeof(char) * 1);
        nstr[0] = '\0';
        elfIncObj(ELF_STRING);
        return nstr;
    }

    nstr = (char*)malloc(sizeof(char) * len + 1);
    memcpy(nstr, &str[start], sizeof(char) * len);
    nstr[len] = '\0';

    elfIncObj(ELF_STRING);

    return nstr;
}

bool elfIsCharNumber(char c)
{
    char numChars[14] = "0123456789.-+";
    int i;

    for (i = 0; i < 13; i++)
    {
        if (c == numChars[i])
            return true;
    }

    return false;
}

bool elfIsStringNumber(const char* str)
{
    char numChars[12] = "0123456789.";
    int dots;
    int i;
    int offset;

    if ((str[0] == '-' || str[0] == '+') && strlen(str) > 1)
        offset = 1;

    dots = 0;
    for (i = offset; i < (int)strlen(str); i++)
    {
        if (str[i] == '.')
            dots++;
    }

    if (dots > 1)
        return false;

    numChars[11] = '\0';

    for (i = offset; i < (int)strlen(str); i++)
    {
        if (!strchr(numChars, str[i]))
            return false;
    }

    return true;
}

bool elfIsStringPositiveInt(const char* str)
{
    char numChars[11] = "0123456789";
    int i;

    numChars[10] = '\0';

    for (i = 0; i < (int)strlen(str); i++)
    {
        if (!strchr(numChars, str[i]))
            return false;
    }

    return true;
}

int elfRFindCharFromString(char chr, const char* str)
{
    int result = -1;

    for (result = strlen(str) - 1; result > -1; result--)
    {
        if (str[result] == chr)
            break;
    }

    return result;
}

int elfRFindCharsFromString(char* chrs, const char* str)
{
    int i;
    int result = -1;

    for (i = 0; i < (int)strlen(chrs); i++)
    {
        for (result = strlen(str) - 1; result > -1; result--)
        {
            if (str[result] == chrs[i])
                return result;
        }
    }

    return result;
}

char* elfGetFileFolder(const char* filePath)
{
    char* str;
    char* nstr;

    str = elfCreateString(filePath);

    if (elfRFindCharFromString('/', filePath) != -1)
    {
        nstr = elfSubString(str, 0, elfRFindCharFromString('/', str) + 1);
        elfDestroyString(str);
        str = nstr;
        nstr = NULL;
    }

    if (elfRFindCharFromString('\\', filePath) != -1)
    {
        nstr = elfSubString(str, 0, elfRFindCharFromString('\\', str) + 1);
        elfDestroyString(str);
        str = nstr;
        nstr = NULL;
    }

    return str;
}
