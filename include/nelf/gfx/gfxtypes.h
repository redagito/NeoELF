

struct gfxTransform
{
    float position[3];
    float rotation[3];
    float scale[3];
    float orient[4];
    float matrix[16];
    float normalMatrix[9];
    unsigned char recalcMatrix;
    unsigned char cameraMode;
};

struct gfxVertexData
{
    GFX_OBJECT_HEADER;
    unsigned int vbo;
    int count;
    int format;
    int sizeBytes;
    int dataType;
    void* data;
    unsigned char changed;
};

typedef struct gfxVarr
{
    gfxVertexData* data;
    int vertexCount;
    int elementCount;
    int vertexSizeBytes;
} gfxVarr;

struct gfxVertexArray
{
    GFX_OBJECT_HEADER;
    int vertexCount;
    gfxVarr varrs[GFX_MAX_VERTEX_ARRAYS];
    unsigned char gpuData;
};

struct gfxVertexIndex
{
    GFX_OBJECT_HEADER;
    int indiceCount;
    gfxVertexData* data;
    unsigned char gpuData;
};
