#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

// Per vertex data in model space
// TODO Why do these have object headers?
//      Are these refcounted? Performance???
struct elfVertex
{
    ELF_OBJECT_HEADER;
    // Position in model space
    elfVec3f position;
    // Normal vector in model space
    elfVec3f normal;
    // UV texture coordinate
    elfVec2f texCoord;
};

// TODO Seriously each vertex is constructed and refcounted?
elfVertex* elfCreateVertex();
void elfDestroyVertex(void* data);

void elfSetVertexPosition(elfVertex* vertice, float x, float y, float z);
void elfSetVertexNormal(elfVertex* vertice, float x, float y, float z);
void elfSetVertexTexCoord(elfVertex* vertice, float u, float v);

elfVec3f elfGetVertexPosition(elfVertex* vertice);
elfVec3f elfGetVertexNormal(elfVertex* vertice);
elfVec2f elfGetVertexTexCoord(elfVertex* vertice);