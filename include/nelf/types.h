
struct elfDirectory
{
    ELF_OBJECT_HEADER;
    char* path;
    elfList* items;
};

struct elfDirectoryItem
{
    ELF_OBJECT_HEADER;
    char* name;
    unsigned char itemType;
};

struct elfPakIndex
{
    ELF_OBJECT_HEADER;
    unsigned char indexType;
    char* name;
    unsigned int offset;
};

struct elfPak
{
    ELF_OBJECT_HEADER;
    char* filePath;
    elfList* indexes;

    int textureCount;
    int materialCount;
    int modelCount;
    int cameraCount;
    int entityCount;
    int lightCount;
    int armatureCount;
    int sceneCount;
    int scriptCount;
};

struct elfPostProcess
{
    ELF_OBJECT_HEADER;

    gfxTexture* mainRtColor[2];
    gfxTexture* mainRtDepth;
    gfxRenderTarget* mainRt;

    gfxTexture* rtTexHigh_1;
    gfxTexture* rtTexHigh_2;
    gfxTexture* rtTexHighDepth;
    gfxTexture* rtTexMed_1;
    gfxTexture* rtTexMed_2;
    gfxTexture* rtTexMed_3;
    gfxTexture* rtTexLow_1;
    gfxTexture* rtTexLow_2;
    gfxTexture* rtTexTiny_1;
    gfxTexture* rtTexTiny_2;

    gfxRenderTarget* rtHigh;
    gfxRenderTarget* rtMed;
    gfxRenderTarget* rtLow;
    gfxRenderTarget* rtTiny;

    gfxShaderProgram* hipassShdr;
    gfxShaderProgram* blurShdr;
    gfxShaderProgram* bloomCombineShdr;
    gfxShaderProgram* dofCombineShdr;
    gfxShaderProgram* ssaoShdr;
    gfxShaderProgram* ssaoCombineShdr;
    gfxShaderProgram* lightShaftShdr;

    unsigned char bloom;
    float bloomThreshold;

    unsigned char dof;
    float dofFocalRange;
    float dofFocalDistance;
    unsigned char dofAutoFocus;

    unsigned char ssao;
    float ssaoAmount;

    unsigned char lightShafts;
    float lightShaftsIntensity;
    gfxTransform* lightShaftTransform;

    int bufferWidth;
    int bufferHeight;

    gfxShaderParams shaderParams;
};

struct elfRequest
{
    ELF_RESOURCE_HEADER;
    char* url;
    char* method;
};
