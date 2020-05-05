#pragma once

#include "nelf/Vector.h"
#include "nelf/gui/guiObjectHeader.h"

struct elfGuiObject
{
    ELF_GUI_OBJECT_HEADER;
};

const char* elfGetGuiObjectName(elfGuiObject* object);
elfVec2i elfGetGuiObjectPosition(elfGuiObject* object);
elfVec2i elfGetGuiObjectSize(elfGuiObject* object);
elfColor elfGetGuiObjectColor(elfGuiObject* object);

bool elfGetGuiObjectVisible(elfGuiObject* object);
bool elfGetGuiObjectActive(elfGuiObject* object);

elfScript* elfGetGuiObjectScript(elfGuiObject* object);
int elfGetGuiObjectEvent(elfGuiObject* object);

void elfSetGuiObjectPosition(elfGuiObject* object, float x, float y);
void elfSetGuiObjectColor(elfGuiObject* object, float r, float g, float b, float a);

void elfSetGuiObjectVisible(elfGuiObject* object, bool visible);
void elfSetGuiObjectActive(elfGuiObject* object, bool active);
void elfSetGuiObjectScript(elfGuiObject* object, elfScript* script);

void elfRecalcGuiObject(elfGuiObject* object);

void elfClearGuiObjectRoot(elfGuiObject* object);

void elfSetGuiObjectRoot(elfGuiObject* object, elfGui* root);

elfGuiObject* elfTraceTopObject(elfGuiObject* object, unsigned char click);

void elfResetGuiObjectEvents(elfGuiObject* object);

bool elfAddGuiObject(elfGuiObject* parent, elfGuiObject* object);

elfGuiObject* elfGetGuiObjectByName(elfGuiObject* parent, const char* name);
elfGuiObject* elfGetGuiObjectByIndex(elfGuiObject* parent, int idx);

bool elfRemoveGuiObjectByName(elfGuiObject* parent, const char* name);
bool elfRemoveGuiObjectByIndex(elfGuiObject* parent, int idx);
bool elfRemoveGuiObjectByObject(elfGuiObject* parent, elfGuiObject* object);