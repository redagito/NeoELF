#pragma once

#include "nelf/Color.h"
#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfGui;
struct elfGuiObject;
struct elfList;
struct elfScript;

#define ELF_GUI_OBJECT_HEADER       \
    ELF_OBJECT_HEADER;              \
    char* name = nullptr;           \
    elfVec2i pos;                   \
    elfVec2i relPos;                \
    int width = 0;                  \
    int height = 0;                 \
    elfColor color;                 \
    bool visible = false;           \
    bool active = false;            \
    elfGui* root = nullptr;         \
    elfGuiObject* parent = nullptr; \
    elfList* children = nullptr;    \
    elfList* screens = nullptr;     \
    elfScript* script = nullptr;    \
    int event = 0