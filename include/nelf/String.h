#pragma once

#include "nelf/objectHeader.h"

// Of course theres a custom string type
struct elfString
{
    ELF_OBJECT_HEADER;
    char* str = nullptr;
};

/**
 * Lifetime management
 */
char* elfCreateString(const char* str);

void elfDestroyString(char* str);

elfString* elfCreateStringObject();

void elfDestroyStringObject(void* data);

/**
 * Write
 */
char* elfRemoveCharFromString(char* str, int idx);

char* elfAppendCharToString(char* str, char c);

char* elfInsertCharToString(char* str, int idx, char c);

char* elfMergeStrings(const char* str1, const char* str2);

char* elfMergeStrings3(const char* str1, const char* str2, const char* str3);

char* elfSubString(char* str, int start, int len);

/**
 * Read
 */
bool elfIsCharNumber(char c);

bool elfIsStringNumber(const char* str);

bool elfIsStringPositiveInt(const char* str);

int elfRFindCharFromString(char chr, const char* str);

int elfRFindCharsFromString(char* chrs, const char* str);

char* elfGetFileFolder(const char* filePath);