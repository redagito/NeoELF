#pragma once

#include "nelf/objectHeader.h"

/**
 * Property with name and dynamic type
 */
struct elfProperty
{
    ELF_OBJECT_HEADER;
    char* name;
    unsigned char propertyType;
    // TODO Union?
    int ival;
    float fval;
    char* sval;
    bool bval;
};

elfProperty* elfCreateProperty(const char* name);

void elfDestroyProperty(void* data);

/**
 * Getter
 */
int elfGetPropertyType(elfProperty* property);

int elfGetPropertyInt(elfProperty* property);

float elfGetPropertyFloat(elfProperty* property);

const char* elfGetPropertyString(elfProperty* property);

bool elfGetPropertyBool(elfProperty* property);

/**
 * Setter
 */
void elfSetPropertyInt(elfProperty* property, int ival);

void elfSetPropertyFloat(elfProperty* property, float fval);

void elfSetPropertyString(elfProperty* property, const char* sval);

void elfSetPropertyBool(elfProperty* property, bool bval);