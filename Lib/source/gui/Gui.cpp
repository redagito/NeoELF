#include "nelf/gui/Gui.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Context.h"
#include "nelf/Engine.h"
#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/Scripting.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gui/Area.h"
#include "nelf/gui/Button.h"
#include "nelf/gui/CheckBox.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/gui/Label.h"
#include "nelf/gui/Picture.h"
#include "nelf/gui/Screen.h"
#include "nelf/gui/Slider.h"
#include "nelf/gui/TextField.h"
#include "nelf/gui/TextList.h"
#include "nelf/gui/buttonState.h"
#include "nelf/gui/guiEvent.h"
#include "nelf/keyCode.h"
#include "nelf/keyState.h"
#include "nelf/mouseButtonCode.h"
#include "nelf/objectType.h"

elfGui* elfCreateGui()
{
    elfGui* gui;

    gui = (elfGui*)malloc(sizeof(elfGui));
    memset(gui, 0x0, sizeof(elfGui));
    gui->objType = ELF_GUI;
    gui->objDestr = elfDestroyGui;

    gui->visible = true;
    gui->active = true;

    gui->children = elfCreateList();
    gui->screens = elfCreateList();

    elfIncRef((elfObject*)gui->children);
    elfIncRef((elfObject*)gui->screens);

    gui->root = gui;

    gui->pos.x = gui->pos.y = 0;
    gui->updateSize = true;
    gui->width = elfGetWindowWidth();
    gui->height = elfGetWindowHeight();

    elfIncObj(ELF_GUI);

    return gui;
}

void elfDestroyGui(void* data)
{
    elfGuiObject* object;
    elfGui* gui = (elfGui*)data;

    if (gui->name)
        elfDestroyString(gui->name);
    if (gui->dragObject)
        elfDecRef((elfObject*)gui->dragObject);
    if (gui->dragContent)
        elfDestroyString(gui->dragContent);

    for (object = (elfGuiObject*)elfBeginList(gui->children); object;
         object = (elfGuiObject*)elfGetListNext(gui->children))
    {
        elfClearGuiObjectRoot(object);
        object->parent = nullptr;
    }

    elfDecRef((elfObject*)gui->children);
    elfDecRef((elfObject*)gui->screens);

    free(gui);

    elfDecObj(ELF_GUI);
}

void elfSendGuiCharEvent(elfGui* gui, char chr)
{
    char* str;

    if (chr < 32 || chr > 126)
        return;

    if (gui->activeTextField)
    {
        str = elfInsertCharToString(gui->activeTextField->text, gui->activeTextField->cursorPos, chr);
        elfDestroyString(gui->activeTextField->text);
        gui->activeTextField->text = str;
        elfMoveTextFieldCursorRight(gui->activeTextField);

        gui->activeTextField->event = ELF_CHAR_INPUT;
        if (gui->activeTextField->script)
        {
            eng->actor = (elfObject*)gui->activeTextField;
            elfIncRef((elfObject*)gui->activeTextField);

            // TODO Wut?
            elfRunString("me = GetActor(); event = CHAR_INPUT");
            elfRunScript(gui->activeTextField->script);
            elfRunString("me = nil; event = 0");

            elfDecRef((elfObject*)gui->activeTextField);
            eng->actor = nullptr;
        }
    }
}

void elfSendGuiKeyEvent(elfGui* gui, int key)
{
    char* str;

    if (gui->activeTextField)
    {
        if (key == ELF_KEY_LEFT)
        {
            elfMoveTextFieldCursorLeft(gui->activeTextField);
        }
        else if (key == ELF_KEY_RIGHT)
        {
            elfMoveTextFieldCursorRight(gui->activeTextField);
        }
        else if (key == ELF_KEY_DEL)
        {
            if (gui->activeTextField->cursorPos < (int)strlen(gui->activeTextField->text))
            {
                str = elfRemoveCharFromString(gui->activeTextField->text, gui->activeTextField->cursorPos);
                elfDestroyString(gui->activeTextField->text);
                gui->activeTextField->text = str;
            }
        }
        else if (key == ELF_KEY_BACKSPACE)
        {
            if (gui->activeTextField->cursorPos > 0 && strlen(gui->activeTextField->text) > 0)
            {
                str = elfRemoveCharFromString(gui->activeTextField->text, gui->activeTextField->cursorPos - 1);
                elfDestroyString(gui->activeTextField->text);
                gui->activeTextField->text = str;
                elfMoveTextFieldCursorLeft(gui->activeTextField);
            }
        }
        else if (key == ELF_KEY_ENTER)
        {
            gui->activeTextField->event = ELF_LOSE_FOCUS;
            if (gui->activeTextField->script)
            {
                eng->actor = (elfObject*)gui->activeTextField;
                elfIncRef((elfObject*)gui->activeTextField);

                elfRunString("me = GetActor(); event = LOSE_FOCUS");
                elfRunScript(gui->activeTextField->script);
                elfRunString("me = nil; event = 0");

                elfDecRef((elfObject*)gui->activeTextField);
                eng->actor = nullptr;
            }

            gui->activeTextField = nullptr;
        }
    }
}

void elfUpdateGui(elfGui* gui, float step)
{
    elfVec2i mouseForce;
    elfVec2i mousePosition;
    bool moved;
    elfGuiObject* prevTrace;
    elfObject* event;
    elfSlider* slider;
    elfTextList* textList;
    int i;

    if (!gui->visible || !gui->active)
        return;

    if (gui->updateSize)
    {
        gui->width = elfGetWindowWidth();
        gui->height = elfGetWindowHeight();
        elfRecalcGuiObject((elfGuiObject*)gui);
    }

    elfResetGuiObjectEvents((elfGuiObject*)gui);

    mouseForce = elfGetMouseForce();
    mousePosition = elfGetMousePosition();

    moved = false;
    if (mouseForce.x || mouseForce.y)
        moved = true;

    prevTrace = gui->trace;
    gui->trace = elfTraceTopObject((elfGuiObject*)gui, elfGetMouseButtonState(ELF_BUTTON_LEFT) == ELF_PRESSED);

    if (gui->trace && gui->trace->active)
    {
        if (gui->trace->objType == ELF_BUTTON)
        {
            if (!gui->target)
            {
                ((elfButton*)gui->trace)->state = ELF_OVER;
            }
            else if (gui->target == gui->trace)
            {
                ((elfButton*)gui->trace)->state = ELF_ON;
            }
        }
    }

    if (prevTrace && prevTrace != gui->trace)
    {
        if (prevTrace->objType == ELF_BUTTON)
        {
            ((elfButton*)prevTrace)->state = ELF_OFF;
        }
    }

    if (elfGetMouseButtonState(ELF_BUTTON_LEFT) == ELF_PRESSED)
    {
        if (gui->activeTextField && gui->activeTextField != (elfTextField*)gui->trace)
        {
            gui->activeTextField->event = ELF_LOSE_FOCUS;
            if (gui->activeTextField->script)
            {
                eng->actor = (elfObject*)gui->activeTextField;
                elfIncRef((elfObject*)gui->activeTextField);

                elfRunString("me = GetActor(); event = LOSE_FOCUS");
                elfRunScript(gui->activeTextField->script);
                elfRunString("me = nil; event = 0");

                elfDecRef((elfObject*)gui->activeTextField);
                eng->actor = nullptr;
            }

            gui->activeTextField = nullptr;
        }

        gui->target = gui->trace;

        if (gui->target && gui->target->active)
        {
            if (gui->target->objType == ELF_BUTTON)
            {
                ((elfButton*)gui->target)->state = ELF_ON;
            }
            else if (gui->target->objType == ELF_TEXT_FIELD)
            {
                gui->activeTextField = (elfTextField*)gui->target;

                gui->activeTextField->event = ELF_GAIN_FOCUS;
                if (gui->activeTextField->script)
                {
                    eng->actor = (elfObject*)gui->activeTextField;
                    elfIncRef((elfObject*)gui->activeTextField);

                    elfRunString("me = GetActor(); event = GAIN_FOCUS");
                    elfRunScript(gui->activeTextField->script);
                    elfRunString("me = nil; event = 0");

                    elfDecRef((elfObject*)gui->activeTextField);
                    eng->actor = nullptr;
                }
            }
            else if (gui->target->objType == ELF_SLIDER)
            {
                slider = (elfSlider*)gui->target;
                if (slider->width > slider->height)
                {
                    slider->value = (float)(mousePosition.x - slider->pos.x) / ((float)slider->width);
                    if (slider->value < 0.0f)
                        slider->value = 0.0f;
                    if (slider->value > 1.0f)
                        slider->value = 1.0f;
                }
                else
                {
                    slider->value =
                        (float)((elfGetWindowHeight() - mousePosition.y) - slider->pos.y) / ((float)slider->height);
                    if (slider->value < 0.0f)
                        slider->value = 0.0f;
                    if (slider->value > 1.0f)
                        slider->value = 1.0f;
                }

                slider->event = ELF_VALUE_CHANGED;
                if (slider->script)
                {
                    eng->actor = (elfObject*)gui->target;
                    elfIncRef((elfObject*)gui->target);

                    elfRunString("me = GetActor(); event = VALUE_CHANGED");
                    elfRunScript(slider->script);
                    elfRunString("me = nil; event = 0");

                    elfDecRef((elfObject*)gui->target);
                    eng->actor = nullptr;
                }
            }
            else if (gui->target->objType == ELF_TEXT_LIST)
            {
                textList = (elfTextList*)gui->target;
                if (textList->font && elfGetListLength(textList->items) > 0)
                {
                    textList->selection = textList->rows - 1 -
                                          (elfGetWindowHeight() - mousePosition.y - textList->pos.y) /
                                              (textList->font->size + textList->font->offsetY) +
                                          textList->offset;

                    if (textList->selection > elfGetListLength(textList->items) - 1)
                        textList->selection = -1;
                    else if (textList->itemDrag)
                    {
                        if (gui->dragContent)
                            elfDestroyString(gui->dragContent);
                        if (gui->dragObject)
                            elfDecRef((elfObject*)gui->dragObject);
                        gui->dragging = true;
                        gui->dragContent = elfCreateString(elfGetTextListItem(textList, textList->selection));
                        gui->dragObject = (elfGuiObject*)textList;
                        elfIncRef((elfObject*)gui->dragObject);
                    }

                    textList->event = ELF_SELECTION_CHANGED;

                    if (textList->script)
                    {
                        eng->actor = (elfObject*)gui->target;
                        elfIncRef((elfObject*)gui->target);

                        elfRunString("me = GetActor(); event = SELECTION_CHANGED");
                        elfRunScript(textList->script);
                        elfRunString("me = nil; event = 0");

                        elfDecRef((elfObject*)gui->target);
                        eng->actor = nullptr;
                    }
                }
            }
            else if (gui->target->objType == ELF_CHECK_BOX)
            {
                ((elfCheckBox*)gui->target)->state = !((elfCheckBox*)gui->target)->state;
                ((elfCheckBox*)gui->target)->event = ELF_STATE_CHANGED;

                if (((elfCheckBox*)gui->target)->script)
                {
                    eng->actor = (elfObject*)gui->target;
                    elfIncRef((elfObject*)gui->target);

                    elfRunString("me = GetActor(); event = STATE_CHANGED");
                    elfRunScript(((elfCheckBox*)gui->target)->script);
                    elfRunString("me = nil; event = 0");

                    elfDecRef((elfObject*)gui->target);
                    eng->actor = nullptr;
                }
            }
        }
    }
    else if (elfGetMouseButtonState(ELF_BUTTON_LEFT) == ELF_RELEASED)
    {
        if (gui->target && gui->target->active)
        {
            if (gui->target->objType == ELF_BUTTON)
            {
                if (gui->target == gui->trace)
                {
                    ((elfButton*)gui->target)->event = ELF_CLICKED;
                    ((elfButton*)gui->target)->state = ELF_OFF;

                    if (((elfButton*)gui->target)->script)
                    {
                        eng->actor = (elfObject*)gui->target;
                        elfIncRef((elfObject*)gui->target);

                        elfRunString("me = GetActor(); event = CLICKED");
                        elfRunScript(((elfButton*)gui->target)->script);
                        elfRunString("me = nil; event = 0");

                        elfDecRef((elfObject*)gui->target);
                        eng->actor = nullptr;
                    }
                }
            }
        }
        gui->target = nullptr;

        if (gui->trace && gui->trace->active)
        {
            if (gui->trace->objType == ELF_TEXT_FIELD)
            {
                if (gui->dragging)
                {
                    ((elfTextField*)gui->trace)->event = ELF_DROP;

                    if (((elfTextField*)gui->trace)->script)
                    {
                        eng->actor = (elfObject*)gui->trace;
                        elfIncRef((elfObject*)gui->trace);

                        elfRunString("me = GetActor(); event = DROP");
                        elfRunScript(((elfTextField*)gui->trace)->script);
                        elfRunString("me = nil; event = 0");

                        elfDecRef((elfObject*)gui->trace);
                        eng->actor = nullptr;
                    }
                }
            }
        }

        gui->dragging = false;
    }
    else if (elfGetMouseButtonState(ELF_BUTTON_LEFT) == ELF_DOWN && moved)
    {
        if (gui->target && gui->target->active)
        {
            if (gui->target->objType == ELF_SLIDER)
            {
                slider = (elfSlider*)gui->target;
                if (slider->width > slider->height)
                {
                    slider->value = (float)(mousePosition.x - slider->pos.x) / ((float)slider->width);
                    if (slider->value < 0.0f)
                        slider->value = 0.0f;
                    if (slider->value > 1.0f)
                        slider->value = 1.0f;
                }
                else
                {
                    slider->value =
                        (float)((elfGetWindowHeight() - mousePosition.y) - slider->pos.y) / ((float)slider->height);
                    if (slider->value < 0.0f)
                        slider->value = 0.0f;
                    if (slider->value > 1.0f)
                        slider->value = 1.0f;
                }

                slider->event = ELF_VALUE_CHANGED;
                if (slider->script)
                {
                    eng->actor = (elfObject*)gui->target;
                    elfIncRef((elfObject*)gui->target);

                    elfRunString("me = GetActor(); event = VALUE_CHANGED");
                    elfRunScript(slider->script);
                    elfRunString("me = nil; event = 0");

                    elfDecRef((elfObject*)gui->target);
                    eng->actor = nullptr;
                }
            }
        }
    }

#ifndef ELF_MACOSX
    if (gui->curKey)
    {
        gui->keyStep += step;

        if (gui->keyRepeat)
        {
            if (gui->keyStep >= 0.05f)
            {
                elfSendGuiKeyEvent(gui, gui->curKey);
                gui->keyStep = gui->keyStep - 0.05f;
            }
        }
        else
        {
            if (gui->keyStep >= 0.35f)
            {
                elfSendGuiKeyEvent(gui, gui->curKey);
                gui->keyStep = gui->keyStep - 0.35f;
                gui->keyRepeat = true;
            }
        }
    }

    if (gui->curChar)
    {
        gui->charStep += step;

        if (gui->charRepeat)
        {
            if (gui->charStep >= 0.05f)
            {
                elfSendGuiCharEvent(gui, gui->curChar);
                gui->charStep = gui->charStep - 0.05f;
            }
        }
        else
        {
            if (gui->charStep >= 0.35f)
            {
                elfSendGuiCharEvent(gui, gui->curChar);
                gui->charStep = gui->charStep - 0.35f;
                gui->charRepeat = true;
            }
        }
    }
#endif

    for (i = 0; i < elfGetEventCount(); i++)
    {
        event = elfGetEvent(i);

        if (event->objType == ELF_CHAR_EVENT)
        {
            if (((elfCharEvent*)event)->state == true)
            {
                elfSendGuiCharEvent(gui, (char)((elfCharEvent*)event)->code);

                gui->curChar = (char)((elfCharEvent*)event)->code;
                gui->charStep = 0.0f;
            }
            else
            {
                gui->curChar = 0;
                gui->charStep = 0.0f;
                gui->charRepeat = false;
            }
        }

        if (event->objType == ELF_KEY_EVENT)
        {
            if (((elfKeyEvent*)event)->state == true)
            {
                elfSendGuiKeyEvent(gui, ((elfKeyEvent*)event)->key);

                gui->curKey = ((elfKeyEvent*)event)->key;
                gui->keyStep = 0.0f;
            }
            else
            {
                gui->curKey = 0;
                gui->keyStep = 0.0f;
                gui->keyRepeat = false;
            }
        }
    }
}

void elfDrawGui(elfGui* gui)
{
    elfGuiObject* object;
    elfArea area;

    if (!gui->visible)
        return;

    area.pos.x = gui->pos.x;
    area.pos.y = gui->pos.y;
    area.size.x = gui->width;
    area.size.y = gui->height;

    gfxSetShaderParamsDefault(&gui->shaderParams);
    gui->shaderParams.renderParams.multisample = false;
    gui->shaderParams.renderParams.depthWrite = false;
    gui->shaderParams.renderParams.depthTest = false;
    gui->shaderParams.renderParams.blendMode = GFX_TRANSPARENT;
    elfSetOrtho(gui->pos.x, gui->pos.y, gui->width, gui->height, &gui->shaderParams);

    for (object = (elfGuiObject*)elfBeginList(gui->children); object;
         object = (elfGuiObject*)elfGetListNext(gui->children))
    {
        if (object->objType == ELF_LABEL)
            elfDrawLabel((elfLabel*)object, &gui->shaderParams);
        else if (object->objType == ELF_BUTTON)
            elfDrawButton((elfButton*)object, &gui->shaderParams);
        else if (object->objType == ELF_PICTURE)
            elfDrawPicture((elfPicture*)object, &gui->shaderParams);
        else if (object->objType == ELF_TEXT_FIELD)
        {
            elfDrawTextField((elfTextField*)object, &area, &gui->shaderParams);
            elfSetOrtho(gui->pos.x, gui->pos.y, gui->width, gui->height, &gui->shaderParams);
        }
        else if (object->objType == ELF_TEXT_LIST)
        {
            elfDrawTextList((elfTextList*)object, &area, &gui->shaderParams);
            elfSetOrtho(gui->pos.x, gui->pos.y, gui->width, gui->height, &gui->shaderParams);
        }
        else if (object->objType == ELF_SLIDER)
            elfDrawSlider((elfSlider*)object, &gui->shaderParams);
        else if (object->objType == ELF_CHECK_BOX)
            elfDrawCheckBox((elfCheckBox*)object, &gui->shaderParams);
    }

    for (object = (elfGuiObject*)elfBeginList(gui->screens); object;
         object = (elfGuiObject*)elfGetListNext(gui->screens))
    {
        area.pos.x = gui->pos.x;
        area.pos.y = gui->pos.y;
        area.size.x = gui->width;
        area.size.y = gui->height;
        elfDrawScreen((elfScreen*)object, &area, &gui->shaderParams);
        elfSetOrtho(gui->pos.x, gui->pos.y, gui->width, gui->height, &gui->shaderParams);
    }

    if (gui->dragging && gui->trace && gui->trace->active && gui->trace->objType != ELF_TEXT_LIST)
    {
        elfVec2i mousePos = elfGetMousePosition();

        elfSetOrtho(gui->pos.x, gui->pos.y, gui->width, gui->height, &gui->shaderParams);
        gfxSetColor(&gui->shaderParams.materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 0.15f);

        gfxSetShaderParams(&gui->shaderParams);

        elfDrawCircle((float)mousePos.x, (float)(elfGetWindowHeight() - mousePos.y), 16, 12.0f);
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&gui->shaderParams);
    gfxSetShaderParams(&gui->shaderParams);
}

elfGuiObject* elfGetGuiTrace(elfGui* gui)
{
    if (!gui->trace || gui->trace->objType == ELF_GUI)
        return NULL;
    return gui->trace;
}

elfGuiObject* elfGetGuiFocus(elfGui* gui) { return (elfGuiObject*)gui->focusScreen; }

elfGuiObject* elfGetGuiActiveTextField(elfGui* gui) { return (elfGuiObject*)gui->activeTextField; }

unsigned char elfGetGuiDragging(elfGui* gui) { return gui->dragging; }

elfGuiObject* elfGetGuiDragObject(elfGui* gui) { return gui->dragObject; }

const char* elfGetGuiDragContent(elfGui* gui) { return gui->dragContent; }

void elfEmptyGui(elfGui* gui)
{
    elfDecRef((elfObject*)gui->children);
    elfDecRef((elfObject*)gui->screens);
    gui->children = elfCreateList();
    gui->screens = elfCreateList();
    elfIncRef((elfObject*)gui->children);
    elfIncRef((elfObject*)gui->screens);
}