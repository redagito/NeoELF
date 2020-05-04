
#define GFX_OBJECT_HEADER \
    int objType;          \
    int objRefCount;      \
    void (*objDestr)(void*)

struct gfxObject
{
    GFX_OBJECT_HEADER;
};

struct gfxGeneral
{
    int refCount;
    int objCount;
    int refTable[GFX_OBJECT_TYPE_COUNT];
    int objTable[GFX_OBJECT_TYPE_COUNT];
};

struct gfxDriver
{
    int formatSizes[GFX_MAX_FORMATS];
    int formats[GFX_MAX_FORMATS];
    int drawModes[GFX_MAX_DRAW_MODES];
    int textureInternalFormats[GFX_MAX_TEXTURE_FORMATS];
    int textureDataFormats[GFX_MAX_TEXTURE_FORMATS];
    int vertexDataDrawModes[GFX_MAX_VERTEX_DATA_TYPES];

    gfxRenderTarget* renderTarget;
    gfxShaderProgram* shaderPrograms;
    gfxShaderParams shaderParams;

    int version;
    int maxTextureSize;
    int maxTextureImageUnits;
    int maxDrawBuffers;
    int maxColorAttachments;
    float maxAnisotropy;
    unsigned char dirtyVertexArrays;
    unsigned int verticesDrawn[GFX_MAX_DRAW_MODES];

    gfxShaderConfig shaderConfig;
};

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

struct gfxTexture
{
    GFX_OBJECT_HEADER;
    unsigned int id;
    int type;
    int width;
    int height;
    int format;
    int dataFormat;
};

struct gfxShaderProgram
{
    gfxShaderProgram* next;
    unsigned int id;
    int projectionMatrixLoc;
    int invProjectionMatrixLoc;
    int modelviewMatrixLoc;
    int normalMatrixLoc;
    int texture0Loc;
    int texture1Loc;
    int texture2Loc;
    int texture3Loc;
    int colorMapLoc;
    int normalMapLoc;
    int heightMapLoc;
    int specularMapLoc;
    int colorRampMapLoc;
    int lightMapLoc;
    int cubeMapLoc;
    int shadowProjectionMatrixLoc;
    int shadowMapLoc;
    int ambientColorLoc;
    int diffuseColorLoc;
    int specularColorLoc;
    int shininessLoc;
    int lightPositionLoc;
    int lightColorLoc;
    int lightSpotDirectionLoc;
    int lightRangeLoc;
    int lightFadeRangeLoc;
    int lightInnerConeCosLoc;
    int lightOuterConeCosLoc;
    int cameraPositionLoc;
    int clipStartLoc;
    int clipEndLoc;
    int viewportWidthLoc;
    int viewportHeightLoc;
    int parallaxScaleLoc;
    int alphaThresholdLoc;
    int fogStartLoc;
    int fogEndLoc;
    int fogColorLoc;
    gfxShaderConfig config;
};

struct gfxRenderTarget
{
    GFX_OBJECT_HEADER;
    unsigned int fb;
    unsigned int rb;
    unsigned int width, height;
    unsigned char targets[16];
};

struct gfxQuery
{
    unsigned int id;
};

struct gfxGbuffer
{
    GFX_OBJECT_HEADER;

    int width;
    int height;

    gfxRenderTarget* bufRt;
    gfxRenderTarget* lightRt;
    gfxRenderTarget* mainRt;

    gfxTexture* depthTex;
    gfxTexture* buf1Tex;
    gfxTexture* buf2Tex;
    gfxTexture* buf3Tex;
    gfxTexture* buf4Tex;
    gfxTexture* diffuseTex;
    gfxTexture* specularTex;

    gfxTexture* mainTex;
};
