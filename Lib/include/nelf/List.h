#pragma once

#include "nelf/objectHeader.h"

struct elfObject;

// List node, double linked
struct elfListPtr
{
    // Previous node
    elfListPtr* prev = nullptr;
    // Next node
    elfListPtr* next = nullptr;
    // Node data
    elfObject* obj = 0;
};

struct elfList
{
    ELF_OBJECT_HEADER;
    // First node in list
    elfListPtr* first = nullptr;
    // Last node in list
    elfListPtr* last = nullptr;
    // Current node
    elfListPtr* cur = nullptr;
    // For iterating the list
    // Reverse iteration sets this to cur->prev
    // Forward iteration to cur->next
    // TODO List should NOT store iteration state
    //      Create iterator struct, or just replace with std::list
    elfListPtr* next = nullptr;
    // Total amount of nodes in list
    int length = 0;
};

elfListPtr* elfCreateListPtr();

void elfDestroyListPtr(elfListPtr* ptr);
void elfDestroyListPtrs(elfListPtr* ptr);

void elfSetListCurPtr(elfList* list, elfObject* ptr);

// Lua API
elfList* elfCreateList();
void elfDestroyList(void* data);

int elfGetListLength(elfList* list);

void elfInsertListObject(elfList* list, int idx, elfObject* obj);
void elfAppendListObject(elfList* list, elfObject* obj);

// Get by index
elfObject* elfGetListObject(elfList* list, int idx);
bool elfRemoveListObject(elfList* list, elfObject* obj);

elfObject* elfBeginList(elfList* list);
elfObject* elfGetListNext(elfList* list);

elfObject* elfRBeginList(elfList* list);
elfObject* elfGetListRNext(elfList* list);

void elfSeekList(elfList* list, elfObject* ptr);
void elfRSeekList(elfList* list, elfObject* ptr);