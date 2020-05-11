#include "nelf/gui/TextList.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Font.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gui/Area.h"
#include "nelf/gui/GuiObject.h"
#include "nelf/objectType.h"

elfTextList* elfCreateTextList(const char* name)
{
    elfTextList* textList;

    textList = (elfTextList*)malloc(sizeof(elfTextList));
    memset(textList, 0x0, sizeof(elfTextList));
    textList->objType = ELF_TEXT_LIST;
    textList->objDestr = elfDestroyTextList;

    textList->color.r = textList->color.g = textList->color.b = textList->color.a = 1.0f;
    textList->visible = true;
    textList->active = true;
    textList->rows = 16;
    textList->listWidth = 256;
    textList->selection = -1;

    textList->items = elfCreateList();
    elfIncRef((elfObject*)textList->items);

    if (name)
        textList->name = elfCreateString(name);

    elfIncObj(ELF_TEXT_LIST);

    return textList;
}

void elfDestroyTextList(void* data)
{
    elfTextList* textList = (elfTextList*)data;

    if (textList->name)
        elfDestroyString(textList->name);
    if (textList->font)
        elfDecRef((elfObject*)textList->font);

    elfDecRef((elfObject*)textList->items);

    free(textList);

    elfDecObj(ELF_TEXT_LIST);
}

void elfDrawTextList(elfTextList* textList, elfArea* area, gfxShaderParams* shaderParams)
{
    int x, y, width, height;
    int offset;
    int i;
    bool light;
    elfString* strObj;
    elfColor col1, col2;

    if (!textList->visible || !textList->font || elfGetListLength(textList->items) < 1)
        return;

    x = textList->pos.x;
    y = textList->pos.y;
    width = textList->width;
    height = textList->height;
    if (textList->parent)
    {
        if (x < area->pos.x)
        {
            width -= area->pos.x - x;
            x = area->pos.x;
        }
        if (y < area->pos.y)
        {
            height -= area->pos.y - y;
            y = area->pos.y;
        }
        if (x + width > area->pos.x + area->size.x)
            width -= (x + width) - (area->pos.x + area->size.x);
        if (y + height > area->pos.y + area->size.y)
            height -= (y + height) - (area->pos.y + area->size.y);
    }

    elfSetOrtho(x, y, width, height, shaderParams);

    light = true;
    offset = textList->font->size + textList->font->offsetY;
    for (i = 0, strObj = (elfString*)elfBeginList(textList->items); strObj;
         strObj = (elfString*)elfGetListNext(textList->items), i++)
    {
        if (i < textList->offset)
            continue;
        if (i - textList->offset > textList->rows - 1)
            break;

        if (i == textList->selection)
        {
            shaderParams->textureParams[0].texture = NULL;
            shaderParams->renderParams.vertexColor = true;
            gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
            gfxSetShaderParams(shaderParams);
            col1.r = col1.g = col1.b = 0.35f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.4f;
            col2.a = 1.0f;
            elfDrawHorGradient(textList->pos.x, textList->pos.y + textList->height - offset, textList->listWidth,
                               textList->font->size + textList->font->offsetY, col1, col2);
        }
        else
        {
            shaderParams->textureParams[0].texture = NULL;
            shaderParams->renderParams.vertexColor = true;
            gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 1.0f);
            gfxSetShaderParams(shaderParams);
            col1.r = col1.g = col1.b = 0.17f;
            col1.a = 1.0f;
            col2.r = col2.g = col2.b = 0.14f;
            col2.a = 1.0f;
            elfDrawHorGradient(textList->pos.x, textList->pos.y + textList->height - offset, textList->listWidth,
                               textList->font->size + textList->font->offsetY, col1, col2);
        }

        light = !light;

        shaderParams->renderParams.vertexColor = false;
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 1.0f, 1.0f, 0.6f);
        elfDrawString(textList->font, strObj->str, textList->pos.x, textList->pos.y + textList->height - offset,
                      shaderParams);

        offset += textList->font->size + textList->font->offsetY;
    }

    shaderParams->textureParams[0].texture = NULL;
}

void elfRecalcTextList(elfTextList* textList)
{
    if (textList->font)
    {
        textList->height = (textList->font->size + textList->font->offsetY) * textList->rows;
        textList->width = textList->listWidth;
    }
    else
    {
        textList->height = 0;
        textList->width = 0;
    }
}

elfFont* elfGetTextListFont(elfTextList* textList) { return textList->font; }

int elfGetTextListRowCount(elfTextList* textList) { return textList->rows; }

int elfGetTextListItemCount(elfTextList* textList) { return elfGetListLength(textList->items); }

int elfGetTextListSelectionIndex(elfTextList* textList) { return textList->selection; }

int elfGetTextListOffset(elfTextList* textList) { return textList->offset; }

const char* elfGetTextListItem(elfTextList* textList, int idx)
{
    int i;
    elfString* strObj;

    if (idx < 0 || idx > elfGetListLength(textList->items) - 1)
        return "";

    for (i = 0, strObj = (elfString*)elfBeginList(textList->items); strObj;
         strObj = (elfString*)elfGetListNext(textList->items), i++)
    {
        if (idx == i)
            return strObj->str;
    }

    return "";
}

const char* elfGetTextListSelectedItem(elfTextList* textList)
{
    int i;
    elfString* strObj;

    if (textList->selection < 0 || textList->selection > elfGetListLength(textList->items) - 1)
        return "";

    for (i = 0, strObj = (elfString*)elfBeginList(textList->items); strObj;
         strObj = (elfString*)elfGetListNext(textList->items), i++)
    {
        if (textList->selection == i)
            return strObj->str;
    }

    return "";
}

unsigned char elfGetTextListItemDrag(elfTextList* textList) { return textList->itemDrag; }

void elfSetTextListFont(elfTextList* textList, elfFont* font)
{
    if (textList->font)
        elfDecRef((elfObject*)textList->font);
    textList->font = font;
    if (textList->font)
        elfIncRef((elfObject*)textList->font);
    elfRecalcGuiObject((elfGuiObject*)textList);
}

void elfSetTextListSize(elfTextList* textList, int rows, int width)
{
    textList->rows = rows;
    textList->listWidth = width;
    if (textList->rows < 1)
        textList->rows = 1;
    if (textList->listWidth < 1)
        textList->width = 1;
    elfRecalcGuiObject((elfGuiObject*)textList);
}

void elfAddTextListItem(elfTextList* textList, const char* text)
{
    elfString* strObj;

    if (!text)
        return;

    strObj = elfCreateStringObject();
    strObj->str = elfCreateString(text);

    elfAppendListObject(textList->items, (elfObject*)strObj);
}

void elfSetTextListItem(elfTextList* textList, int idx, const char* text)
{
    elfString* strObj;

    strObj = (elfString*)elfGetListObject(textList->items, idx);
    if (strObj)
    {
        if (strObj->str)
            elfDestroyString(strObj->str);
        strObj->str = elfCreateString(text);
    }
}

bool elfRemoveTextListItem(elfTextList* textList, int idx)
{
    int i;
    elfString* strObj;

    if (idx < 0 || idx > elfGetListLength(textList->items) - 1)
        return false;

    for (i = 0, strObj = (elfString*)elfBeginList(textList->items); strObj;
         strObj = (elfString*)elfGetListNext(textList->items), i++)
    {
        if (idx == i)
        {
            if (idx == textList->selection)
                textList->selection = -1;
            elfRemoveListObject(textList->items, (elfObject*)strObj);
            return true;
        }
    }

    return false;
}

void elfRemoveTextListItems(elfTextList* textList)
{
    elfDecRef((elfObject*)textList->items);
    textList->items = elfCreateList();
    elfIncRef((elfObject*)textList->items);
    textList->offset = 0;
    textList->selection = 0;
}

void elfSetTextListOffset(elfTextList* textList, int offset)
{
    textList->offset = offset;
    if (textList->offset < 0)
        textList->offset = 0;
}

void elfSetTextListSelection(elfTextList* textList, int selection)
{
    textList->selection = selection;
    if (textList->selection < 0)
        textList->selection = -1;
    if (textList->selection > elfGetListLength(textList->items) - 1)
        textList->selection = elfGetListLength(textList->items) - 1;
    if (elfGetListLength(textList->items) == 0)
        textList->selection = -1;
}

void elfSetTextListItemDrag(elfTextList* textList, bool itemDrag) { textList->itemDrag = itemDrag; }