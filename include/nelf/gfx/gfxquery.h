
gfxQuery* gfxCreateQuery()
{
    gfxQuery* query;

    query = (gfxQuery*)malloc(sizeof(gfxQuery));
    memset(query, 0x0, sizeof(gfxQuery));

    glGenQueries(1, &query->id);

    return query;
}

void gfxDestroyQuery(gfxQuery* query)
{
    glDeleteQueries(1, &query->id);
    free(query);
}

void gfxBeginQuery(gfxQuery* query) { glBeginQuery(GL_SAMPLES_PASSED, query->id); }

void gfxEndQuery(gfxQuery* query) { glEndQuery(GL_SAMPLES_PASSED); }

unsigned char gfxIsQueryResult(gfxQuery* query)
{
    int result;

    result = 0;
    glGetQueryObjectiv(query->id, GL_QUERY_RESULT_AVAILABLE, &result);

    return result == GL_TRUE;
}

int gfxGetQueryResult(gfxQuery* query)
{
    int result;

    result = 0;
    glGetQueryObjectiv(query->id, GL_QUERY_RESULT, &result);

    return result;
}
