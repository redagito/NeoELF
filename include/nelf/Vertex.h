#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

// TODO Why do these have object headers?
//      Are these refcounted? Performance???
struct elfVertex
{
    ELF_OBJECT_HEADER;
    elfVec3f position;
    elfVec3f normal;
    elfVec2f texCoord;
};

// TODO Seriously each vertex is cosntructed and refcounted?
elfVertex* elfCreateVertex();
void elfDestroyVertex(void* data);

void elfSetVertexPosition(elfVertex* vertice, float x, float y, float z);
void elfSetVertexNormal(elfVertex* vertice, float x, float y, float z);
void elfSetVertexTexCoord(elfVertex* vertice, float u, float v);

elfVec3f elfGetVertexPosition(elfVertex* vertice);
elfVec3f elfGetVertexNormal(elfVertex* vertice);
elfVec2f elfGetVertexTexCoord(elfVertex* vertice);