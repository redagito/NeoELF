#pragma once

struct gfxQuery
{
    // Should be a GLint
    unsigned int id = 0;
};

gfxQuery* gfxCreateQuery();
void gfxDestroyQuery(gfxQuery* query);

void gfxBeginQuery(gfxQuery* query);
void gfxEndQuery(gfxQuery* query);

bool gfxIsQueryResult(gfxQuery* query);
int gfxGetQueryResult(gfxQuery* query);