#pragma once

#include "nelf/gui/guiObjectHeader.h"

struct elfFont;
struct elfTextField;
struct elfScreen;
struct gfxShaderParams;

struct elfGui
{
    ELF_GUI_OBJECT_HEADER;
    elfFont* defFont = nullptr;
    // TODO Not a pointer?
    gfxShaderParams shaderParams;
    elfGuiObject* trace = nullptr;
    elfGuiObject* target = nullptr;
    elfTextField* activeTextField = nullptr;
    elfScreen* focusScreen = nullptr;
    unsigned char dragging;
    char* dragContent = nullptr;
    elfGuiObject* dragObject = nullptr;

    bool updateSize = true;

    int curKey = 0;
    float keyStep = 0.f;
    bool keyRepeat = false;

    char curChar = 0;
    float charStep = 0.f;
    bool charRepeat = false;
};

elfGui* elfCreateGui();

void elfDestroyGui(void* data);

void elfSendGuiCharEvent(elfGui* gui, char chr);

void elfSendGuiKeyEvent(elfGui* gui, int key);

void elfUpdateGui(elfGui* gui, float step);

void elfDrawGui(elfGui* gui);

elfGuiObject* elfGetGuiTrace(elfGui* gui);

elfGuiObject* elfGetGuiFocus(elfGui* gui);

elfGuiObject* elfGetGuiActiveTextField(elfGui* gui);

unsigned char elfGetGuiDragging(elfGui* gui);

elfGuiObject* elfGetGuiDragObject(elfGui* gui);

const char* elfGetGuiDragContent(elfGui* gui);

void elfEmptyGui(elfGui* gui);