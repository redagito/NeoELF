#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfFont;
struct elfList;
struct elfArea;
struct gfxShaderParams;

struct elfTextList
{
    ELF_GUI_OBJECT_HEADER;
    elfFont* font = nullptr;
    elfList* items = nullptr;
    int rows = 0;
    int listWidth = 0;
    int selection = 0;
    int offset = 0;
    bool itemDrag = false;
};

elfTextList* elfCreateTextList(const char* name);
void elfDestroyTextList(void* data);

void elfDrawTextList(elfTextList* textList, elfArea* area, gfxShaderParams* shaderParams);

void elfRecalcTextList(elfTextList* textList);

elfFont* elfGetTextListFont(elfTextList* textList);
int elfGetTextListRowCount(elfTextList* textList);
int elfGetTextListItemCount(elfTextList* textList);
int elfGetTextListSelectionIndex(elfTextList* textList);
int elfGetTextListOffset(elfTextList* textList);
const char* elfGetTextListItem(elfTextList* textList, int idx);
const char* elfGetTextListSelectedItem(elfTextList* textList);
unsigned char elfGetTextListItemDrag(elfTextList* textList);

void elfSetTextListFont(elfTextList* textList, elfFont* font);
void elfSetTextListSize(elfTextList* textList, int rows, int width);

void elfAddTextListItem(elfTextList* textList, const char* text);

void elfSetTextListItem(elfTextList* textList, int idx, const char* text);

bool elfRemoveTextListItem(elfTextList* textList, int idx);
void elfRemoveTextListItems(elfTextList* textList);

void elfSetTextListOffset(elfTextList* textList, int offset);
void elfSetTextListSelection(elfTextList* textList, int selection);
void elfSetTextListItemDrag(elfTextList* textList, bool itemDrag);