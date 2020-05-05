#include "nelf/gui/GuiObject.h"

#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/gui/Gui.h"
#include "nelf/gui/buttonState.h"
#include "nelf/objectType.h"

// For recalc
#include "nelf/gui/Button.h"
#include "nelf/gui/CheckBox.h"
#include "nelf/gui/Label.h"
#include "nelf/gui/Picture.h"
#include "nelf/gui/Screen.h"
#include "nelf/gui/Slider.h"
#include "nelf/gui/TextField.h"
#include "nelf/gui/TextList.h"

const char* elfGetGuiObjectName(elfGuiObject* object) { return object->name; }

elfVec2i elfGetGuiObjectPosition(elfGuiObject* object) { return object->relPos; }

elfVec2i elfGetGuiObjectSize(elfGuiObject* object)
{
    elfVec2i size;

    size.x = object->width;
    size.y = object->height;

    return size;
}

elfColor elfGetGuiObjectColor(elfGuiObject* object) { return object->color; }

bool elfGetGuiObjectVisible(elfGuiObject* object) { return object->visible; }

bool elfGetGuiObjectActive(elfGuiObject* object) { return object->active; }

elfScript* elfGetGuiObjectScript(elfGuiObject* object) { return object->script; }

int elfGetGuiObjectEvent(elfGuiObject* object) { return object->event; }

void elfSetGuiObjectPosition(elfGuiObject* object, float x, float y)
{
    object->relPos.x = x;
    object->relPos.y = y;
    elfRecalcGuiObject(object);
}

void elfSetGuiObjectColor(elfGuiObject* object, float r, float g, float b, float a)
{
    object->color.r = r;
    object->color.g = g;
    object->color.b = b;
    object->color.a = a;
}

void elfSetGuiObjectVisible(elfGuiObject* object, bool visible) { object->visible = visible; }

void elfSetGuiObjectActive(elfGuiObject* object, bool active)
{
    object->active = active;
    // TODO Why not just override state with active flag in button specific code?
    if (object->objType == ELF_BUTTON)
        ((elfButton*)object)->state = ELF_OFF;
}

void elfSetGuiObjectScript(elfGuiObject* object, elfScript* script)
{
    if (object->script)
        elfDecRef((elfObject*)object->script);
    object->script = script;
    if (object->script)
        elfIncRef((elfObject*)object->script);
}

void elfRecalcGuiObject(elfGuiObject* object)
{
    elfGuiObject* obj;

    if (object->objType == ELF_LABEL)
        elfRecalcLabel((elfLabel*)object);
    else if (object->objType == ELF_BUTTON)
        elfRecalcButton((elfButton*)object);
    else if (object->objType == ELF_PICTURE)
        elfRecalcPicture((elfPicture*)object);
    else if (object->objType == ELF_TEXT_FIELD)
        elfRecalcTextField((elfTextField*)object);
    else if (object->objType == ELF_SLIDER)
        elfRecalcSlider((elfSlider*)object);
    else if (object->objType == ELF_SCREEN)
        elfRecalcScreen((elfScreen*)object);
    else if (object->objType == ELF_TEXT_LIST)
        elfRecalcTextList((elfTextList*)object);
    else if (object->objType == ELF_CHECK_BOX)
        elfRecalcCheckBox((elfCheckBox*)object);

    if (object->parent)
    {
        object->pos.x = object->parent->pos.x + object->relPos.x;
        object->pos.y = object->parent->pos.y + object->parent->height - object->relPos.y - object->height;
    }
    else
    {
        object->pos.x = object->relPos.x;
        object->pos.y = elfGetWindowHeight() - object->relPos.y - object->height;
    }

    if (object->children)
    {
        for (obj = (elfGuiObject*)elfBeginList(object->children); obj;
             obj = (elfGuiObject*)elfGetListNext(object->children))
        {
            elfRecalcGuiObject(obj);
        }
    }

    if (object->screens)
    {
        for (obj = (elfGuiObject*)elfBeginList(object->screens); obj;
             obj = (elfGuiObject*)elfGetListNext(object->screens))
        {
            elfRecalcGuiObject(obj);
        }
    }
}

void elfClearGuiObjectRoot(elfGuiObject* object)
{
    elfGuiObject* cobject;

    if (object->children)
    {
        for (cobject = (elfGuiObject*)elfBeginList(object->children); cobject;
             cobject = (elfGuiObject*)elfGetListNext(object->children))
        {
            elfClearGuiObjectRoot(cobject);
        }
    }

    object->root = nullptr;
}

void elfSetGuiObjectRoot(elfGuiObject* object, elfGui* root)
{
    elfGuiObject* cobject;

    if (object->children)
    {
        for (cobject = (elfGuiObject*)elfBeginList(object->children); cobject;
             cobject = (elfGuiObject*)elfGetListNext(object->children))
        {
            elfSetGuiObjectRoot(cobject, root);
        }
    }

    object->root = root;
}

elfGuiObject* elfTraceTopObject(elfGuiObject* object, unsigned char click)
{
    elfGuiObject* cobject = nullptr;
    elfGuiObject* result = nullptr;
    elfVec2i mousePos;

    if (!object->visible)
        return nullptr;

    mousePos = elfGetMousePosition();
    mousePos.y = elfGetWindowHeight() - mousePos.y;

    if (object->objType == ELF_GUI && ((elfGui*)object)->focusScreen)
    {
        return elfTraceTopObject((elfGuiObject*)((elfGui*)object)->focusScreen, click);
    }

    result = nullptr;
    if (object->screens)
    {
        for (cobject = (elfGuiObject*)elfRBeginList(object->screens); cobject;
             cobject = (elfGuiObject*)elfGetListRNext(object->screens))
        {
            if (mousePos.x >= cobject->pos.x && mousePos.x <= cobject->pos.x + cobject->width &&
                mousePos.y >= cobject->pos.y && mousePos.y <= cobject->pos.y + cobject->height && cobject->visible)
            {
                if (click)
                {
                    elfIncRef((elfObject*)cobject);
                    elfRemoveListObject(object->screens, (elfObject*)cobject);
                    elfAppendListObject(object->screens, (elfObject*)cobject);
                    elfDecRef((elfObject*)cobject);
                }
                result = elfTraceTopObject(cobject, click);
                if (result)
                    return result;
                return cobject;
            }
        }
    }

    result = nullptr;
    if (object->children)
    {
        for (cobject = (elfGuiObject*)elfRBeginList(object->children); cobject;
             cobject = (elfGuiObject*)elfGetListRNext(object->children))
        {
            if (mousePos.x >= cobject->pos.x && mousePos.x <= cobject->pos.x + cobject->width &&
                mousePos.y >= cobject->pos.y && mousePos.y <= cobject->pos.y + cobject->height && cobject->visible)
            {
                result = elfTraceTopObject(cobject, click);
                if (result)
                    return result;
                return cobject;
            }
        }
    }

    if (mousePos.x >= object->pos.x && mousePos.x <= object->pos.x + object->width && mousePos.y >= object->pos.y &&
        mousePos.y <= object->pos.y + object->height)
    {
        return object;
    }

    return nullptr;
}

void elfResetGuiObjectEvents(elfGuiObject* object)
{
    elfGuiObject* obj;

    object->event = 0;

    if (object->children)
    {
        for (obj = (elfGuiObject*)elfBeginList(object->children); obj;
             obj = (elfGuiObject*)elfGetListNext(object->children))
        {
            elfResetGuiObjectEvents(obj);
        }
    }

    if (object->screens)
    {
        for (obj = (elfGuiObject*)elfBeginList(object->screens); obj;
             obj = (elfGuiObject*)elfGetListNext(object->screens))
        {
            elfResetGuiObjectEvents(obj);
        }
    }
}

bool elfAddGuiObject(elfGuiObject* parent, elfGuiObject* object)
{
    if (parent->objType != ELF_GUI && parent->objType != ELF_SCREEN)
        return false;

    if (object->parent)
        elfRemoveGuiObjectByObject(object->parent, object);

    elfSetGuiObjectRoot(object, parent->root);
    object->parent = parent;
    elfRecalcGuiObject(object);

    if (object->objType != ELF_SCREEN)
        elfAppendListObject(parent->children, (elfObject*)object);
    else
        elfAppendListObject(parent->screens, (elfObject*)object);

    return true;
}

elfGuiObject* elfGetGuiObjectByName(elfGuiObject* parent, const char* name)
{
    elfGuiObject* object;

    if (parent->children)
    {
        for (object = (elfGuiObject*)elfBeginList(parent->children); object;
             object = (elfGuiObject*)elfGetListNext(parent->children))
        {
            if (!strcmp(object->name, name))
                return object;
        }
    }

    if (parent->screens)
    {
        for (object = (elfGuiObject*)elfBeginList(parent->screens); object;
             object = (elfGuiObject*)elfGetListNext(parent->screens))
        {
            if (!strcmp(object->name, name))
                return object;
        }
    }

    return nullptr;
}

elfGuiObject* elfGetGuiObjectByIndex(elfGuiObject* parent, int idx)
{
    elfGuiObject* object;
    int i;

    if (parent->objType != ELF_GUI && parent->objType != ELF_SCREEN)
        return nullptr;

    if (idx < 0 || idx > elfGetListLength(parent->children) + elfGetListLength(parent->screens) - 1)
        return nullptr;

    // printf("starting search\n");

    if (idx < elfGetListLength(parent->children))
    {
        for (i = 0, object = (elfGuiObject*)elfBeginList(parent->children); object;
             i++, object = (elfGuiObject*)elfGetListNext(parent->children))
        {
            if (i == idx)
                return object;
        }
    }
    else
    {
        // printf("searching screens\n");
        idx -= elfGetListLength(parent->children);
        for (i = 0, object = (elfGuiObject*)elfBeginList(parent->screens); object;
             i++, object = (elfGuiObject*)elfGetListNext(parent->screens))
        {
            if (i == idx)
                return object;
        }
    }

    return nullptr;
}

bool elfRemoveGuiObjectByName(elfGuiObject* parent, const char* name)
{
    elfGuiObject* object;

    if (parent->objType != ELF_GUI && parent->objType != ELF_SCREEN)
        return false;

    for (object = (elfGuiObject*)elfBeginList(parent->children); object;
         object = (elfGuiObject*)elfGetListNext(parent->children))
    {
        if (!strcmp(object->name, name))
        {
            if (object->root)
            {
                if (object->root->trace == object)
                    object->root->trace = nullptr;
                if (object->root->target == object)
                    object->root->target = nullptr;
                if (object->root->activeTextField == (elfTextField*)object)
                    object->root->activeTextField = nullptr;
            }
            object->parent = nullptr;
            elfClearGuiObjectRoot(object);
            elfRemoveListObject(parent->children, (elfObject*)object);
            return true;
        }
    }

    for (object = (elfGuiObject*)elfBeginList(parent->screens); object;
         object = (elfGuiObject*)elfGetListNext(parent->screens))
    {
        if (!strcmp(object->name, name))
        {
            if (object->root)
            {
                if (object->root->trace == object)
                    object->root->trace = nullptr;
                if (object->root->target == object)
                    object->root->target = nullptr;
                if (object->root->focusScreen == (elfScreen*)object)
                    object->root->focusScreen = nullptr;
            }
            object->parent = nullptr;
            elfClearGuiObjectRoot(object);
            elfRemoveListObject(parent->children, (elfObject*)object);
            return true;
        }
    }

    return false;
}

bool elfRemoveGuiObjectByIndex(elfGuiObject* parent, int idx)
{
    elfGuiObject* object;
    int i;

    if (parent->objType != ELF_GUI && parent->objType == ELF_SCREEN)
        return false;

    if (idx < 0 || idx > elfGetListLength(parent->children) + elfGetListLength(parent->screens) - 1)
        return false;

    if (idx < elfGetListLength(parent->children))
    {
        for (i = 0, object = (elfGuiObject*)elfBeginList(parent->children); object;
             i++, object = (elfGuiObject*)elfGetListNext(parent->children))
        {
            if (i == idx)
            {
                if (object->root)
                {
                    if (object->root->trace == object)
                        object->root->trace = nullptr;
                    if (object->root->target == object)
                        object->root->target = nullptr;
                    if (object->root->activeTextField == (elfTextField*)object)
                        object->root->activeTextField = nullptr;
                }
                object->parent = nullptr;
                elfClearGuiObjectRoot(object);
                elfRemoveListObject(parent->children, (elfObject*)object);
                return true;
            }
        }
    }
    else
    {
        idx -= elfGetListLength(parent->children);
        for (i = 0, object = (elfGuiObject*)elfBeginList(parent->screens); object;
             i++, object = (elfGuiObject*)elfGetListNext(parent->screens))
        {
            if (i == idx)
            {
                if (object->root)
                {
                    if (object->root->trace == object)
                        object->root->trace = nullptr;
                    if (object->root->target == object)
                        object->root->target = nullptr;
                    if (object->root->focusScreen == (elfScreen*)object)
                        object->root->focusScreen = nullptr;
                }
                object->parent = nullptr;
                elfClearGuiObjectRoot(object);
                elfRemoveListObject(parent->children, (elfObject*)object);
                return true;
            }
        }
    }

    return false;
}

bool elfRemoveGuiObjectByObject(elfGuiObject* parent, elfGuiObject* object)
{
    if (parent->objType != ELF_GUI && parent->objType != ELF_SCREEN)
        return false;

    if (object->root)
    {
        if (object->root->trace == object)
            object->root->trace = nullptr;
        if (object->root->target == object)
            object->root->target = nullptr;
        if (object->root->activeTextField == (elfTextField*)object)
            object->root->activeTextField = nullptr;
        if (object->root->focusScreen == (elfScreen*)object)
            object->root->focusScreen = nullptr;
    }

    object->parent = nullptr;
    elfClearGuiObjectRoot(object);

    if (object->objType != ELF_SCREEN)
        return elfRemoveListObject(parent->children, (elfObject*)object);
    else
        return elfRemoveListObject(parent->screens, (elfObject*)object);
}