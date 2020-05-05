

struct elfKeyEvent
{
    ELF_OBJECT_HEADER;
    int key;
    unsigned char state;
};

struct elfCharEvent
{
    ELF_OBJECT_HEADER;
    int code;
    unsigned char state;
};

typedef struct elfJoystick
{
    unsigned char present;
    float axisPos[2];
    unsigned char curButs[16];
    unsigned char prvButs[16];
} elfJoystick;

struct elfVideoMode
{
    ELF_OBJECT_HEADER;
    elfVec2i reso;
};

struct elfContext
{
    ELF_OBJECT_HEADER;
    int width;
    int height;
    unsigned char fullscreen;
    char* title;
    int multisamples;
    elfList* videoModes;
    unsigned char curKeys[256];
    unsigned char prvKeys[256];
    int mousePosition[2];
    int prvMousePosition[2];
    unsigned char hideMouse;
    int mouseWheel;
    unsigned char curMbuts[3];
    unsigned char prvMbuts[3];
    elfJoystick joysticks[16];
    elfList* events;
};

struct elfRenderStation
{
    ELF_OBJECT_HEADER;

    gfxTexture* shadowMap;
    gfxRenderTarget* shadowTarget;

    gfxVertexData* quadVertexData;
    gfxVertexData* quadTexCoordData;
    gfxVertexData* quadNormalData;
    gfxVertexArray* quadVertexArray;

    gfxVertexData* bbVertexData;
    gfxVertexData* bbIndexData;
    gfxVertexArray* bbVertexArray;
    gfxVertexIndex* bbVertexIndex;

    gfxVertexData* lines;
    gfxVertexArray* linesVertexArray;

    gfxVertexData* circleVertexData;
    gfxVertexArray* circleVertexArray;

    int prevCircleVerticeCount;
    float prevCircleSize;
    gfxVertexArray* spriteVertexArray;

    gfxVertexData* gradientVertexData;
    gfxVertexData* gradientColorData;
    gfxVertexArray* gradientVertexArray;
};

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

struct elfLight
{
    ELF_ACTOR_HEADER;
    int lightType;
    elfColor color;
    float range;
    float fadeRange;
    float innerCone;
    float outerCone;
    unsigned char visible;
    unsigned char shaft;
    float shaftSize;
    float shaftIntensity;
    float shaftFadeOff;

    unsigned char shadows;
    elfCamera* shadowCamera;

    float projectionMatrix[16];
};

struct elfParticle
{
    ELF_OBJECT_HEADER;
    float size;
    float sizeGrowth;
    float rotation;
    float rotationGrowth;
    elfColor color;
    float lifeSpan;
    float fadeSpeed;
    elfVec3f position;
    elfVec3f velocity;
};

struct elfParticles
{
    ELF_ACTOR_HEADER;

    int maxCount;
    unsigned char drawMode;
    elfList* particles;
    elfTexture* texture;
    elfModel* model;
    elfEntity* entity;
    gfxVertexArray* vertexArray;
    gfxVertexData* vertices;
    gfxVertexData* texCoords;
    gfxVertexData* colors;

    int spawnCount;
    float spawnDelay;
    unsigned char spawn;
    float curTime;
    elfVec3f gravity;
    float sizeMin;
    float sizeMax;
    float sizeGrowthMin;
    float sizeGrowthMax;
    float rotationMin;
    float rotationMax;
    float rotationGrowthMin;
    float rotationGrowthMax;
    float lifeSpanMin;
    float lifeSpanMax;
    float fadeSpeedMin;
    float fadeSpeedMax;
    elfVec3f velocityMin;
    elfVec3f velocityMax;
    elfVec3f positionMin;
    elfVec3f positionMax;
    elfColor colorMin;
    elfColor colorMax;

    elfVec3f cullAabbMin;
    elfVec3f cullAabbMax;
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
