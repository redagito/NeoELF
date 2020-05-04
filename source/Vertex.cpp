#include "nelf/Vertex.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/objectType.h"

elfVertex* elfCreateVertex()
{
    elfVertex* vertice;

    vertice = (elfVertex*)malloc(sizeof(elfVertex));
    memset(vertice, 0x0, sizeof(elfVertex));
    vertice->objType = ELF_VERTEX;
    vertice->objDestr = elfDestroyVertex;

    elfIncObj(ELF_VERTEX);

    return vertice;
}

void elfDestroyVertex(void* data)
{
    elfVertex* vertice = (elfVertex*)data;

    free(vertice);

    elfDecObj(ELF_VERTEX);
}

void elfSetVertexPosition(elfVertex* vertice, float x, float y, float z)
{
    vertice->position.x = x;
    vertice->position.y = y;
    vertice->position.z = z;
}

void elfSetVertexNormal(elfVertex* vertice, float x, float y, float z)
{
    vertice->normal.x = x;
    vertice->normal.y = y;
    vertice->normal.z = z;
}

void elfSetVertexTexCoord(elfVertex* vertice, float u, float v)
{
    vertice->texCoord.x = u;
    vertice->texCoord.y = v;
}

elfVec3f elfGetVertexPosition(elfVertex* vertice) { return vertice->position; }

elfVec3f elfGetVertexNormal(elfVertex* vertice) { return vertice->normal; }

elfVec2f elfGetVertexTexCoord(elfVertex* vertice) { return vertice->texCoord; }