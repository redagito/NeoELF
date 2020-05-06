
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

struct elfSprite
{
    ELF_ACTOR_HEADER;

    elfMaterial* material;
    elfFramePlayer* framePlayer;

    unsigned char faceCamera;

    elfVec3f position;
    elfVec2f scale;
    elfVec2f size;
    float cullRadius;

    gfxQuery* query;
    unsigned char visible;
    unsigned char occluder;
    unsigned char culled;
};

struct elfScene
{
    ELF_RESOURCE_HEADER;
    char* filePath;

    unsigned char physics;
    unsigned char runScripts;
    unsigned char debugDraw;
    unsigned char occlusionCulling;

    elfColor ambientColor;

    unsigned char fog;
    float fogStart;
    float fogEnd;
    elfColor fogColor;

    elfList* scripts;
    elfList* textures;
    elfList* materials;
    elfList* models;
    elfList* cameras;
    elfList* entities;
    elfList* lights;
    elfList* armatures;
    elfList* particles;
    elfList* sprites;

    elfList* entityQueue;
    int entityQueueCount;

    elfList* spriteQueue;
    int spriteQueueCount;

    elfPhysicsWorld* world;
    elfPhysicsWorld* dworld;

    elfCamera* curCamera;

    gfxShaderParams shaderParams;

    gfxShaderProgram* composeFogShdr;

    elfPak* pak;
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
