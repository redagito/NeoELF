#pragma once

// <!!

#ifdef __cplusplus
extern "C"
{
#endif
    // !!>

#define ELF_FALSE \
    0x0000  // <mdoc> TRUTH VALUES <mdocc> The thruth value flags. Usually you can just use normal Lua true and false
            // but there are some special cases.
#define ELF_TRUE 0x0001

#define ELF_KEY_ESC 135  // <mdoc> KEY CODES <mdocc> The key codes used by elf.GetKeyState
#define ELF_KEY_F1 136
#define ELF_KEY_F2 137
#define ELF_KEY_F3 138
#define ELF_KEY_F4 139
#define ELF_KEY_F5 140
#define ELF_KEY_F6 141
#define ELF_KEY_F7 142
#define ELF_KEY_F8 143
#define ELF_KEY_F9 144
#define ELF_KEY_F10 145
#define ELF_KEY_F11 146
#define ELF_KEY_F12 147
#define ELF_KEY_UP 148
#define ELF_KEY_DOWN 149
#define ELF_KEY_LEFT 150
#define ELF_KEY_RIGHT 151
#define ELF_KEY_LSHIFT 152
#define ELF_KEY_RSHIFT 153
#define ELF_KEY_LCTRL 154
#define ELF_KEY_RCTRL 155
#define ELF_KEY_LALT 156
#define ELF_KEY_RALT 157
#define ELF_KEY_TAB 158
#define ELF_KEY_ENTER 159
#define ELF_KEY_BACKSPACE 160
#define ELF_KEY_INSERT 161
#define ELF_KEY_DEL 162
#define ELF_KEY_PAGEUP 163
#define ELF_KEY_PAGEDOWN 164
#define ELF_KEY_HOME 165
#define ELF_KEY_END 166
#define ELF_KEY_KP_0 167
#define ELF_KEY_KP_1 168
#define ELF_KEY_KP_2 169
#define ELF_KEY_KP_3 170
#define ELF_KEY_KP_4 171
#define ELF_KEY_KP_5 172
#define ELF_KEY_KP_6 173
#define ELF_KEY_KP_7 174
#define ELF_KEY_KP_8 175
#define ELF_KEY_KP_9 176
#define ELF_KEY_KP_DIVIDE 177
#define ELF_KEY_KP_MULTIPLY 178
#define ELF_KEY_KP_SUBTRACT 179
#define ELF_KEY_KP_ADD 180
#define ELF_KEY_KP_DECIMAL 181
#define ELF_KEY_KP_EQUAL 182
#define ELF_KEY_KP_ENTER 183
#define ELF_KEY_SPACE 32
#define ELF_KEY_0 48
#define ELF_KEY_1 49
#define ELF_KEY_2 50
#define ELF_KEY_3 51
#define ELF_KEY_4 52
#define ELF_KEY_5 53
#define ELF_KEY_6 54
#define ELF_KEY_7 55
#define ELF_KEY_8 56
#define ELF_KEY_9 57
#define ELF_KEY_A 65
#define ELF_KEY_B 66
#define ELF_KEY_C 67
#define ELF_KEY_D 68
#define ELF_KEY_E 69
#define ELF_KEY_F 70
#define ELF_KEY_G 71
#define ELF_KEY_H 72
#define ELF_KEY_I 73
#define ELF_KEY_J 74
#define ELF_KEY_K 75
#define ELF_KEY_L 76
#define ELF_KEY_M 77
#define ELF_KEY_N 78
#define ELF_KEY_O 79
#define ELF_KEY_P 80
#define ELF_KEY_Q 81
#define ELF_KEY_R 82
#define ELF_KEY_S 83
#define ELF_KEY_T 84
#define ELF_KEY_U 85
#define ELF_KEY_V 86
#define ELF_KEY_W 87
#define ELF_KEY_X 88
#define ELF_KEY_Y 89
#define ELF_KEY_Z 90

#define ELF_UP \
    0x0000  // <mdoc> KEY AND MOUSE STATES <mdocc> The state codes returned by elf.GetKeyState and
            // elf.GetMouseButtonState
#define ELF_PRESSED 0x0001
#define ELF_DOWN 0x0002
#define ELF_RELEASED 0x0003

#define ELF_OFF 0x0000  // <mdoc> GUI BUTTON STATE <mdocc> The gui button states returned by elf.GetButtonState
#define ELF_ON 0x0001
#define ELF_OVER 0x0002

#define ELF_BUTTON_LEFT \
    0x0000  // <mdoc> MOUSE BUTTON CODES <mdocc> The mouse button codes used by elf.GetMouseButtonState
#define ELF_BUTTON_MIDDLE 0x0001
#define ELF_BUTTON_RIGHT 0x0002

#define ELF_HINGE 0x0001  // <mdoc> JOINT TYPES <mdocc> The joint types returned by elf.GetJointType
#define ELF_BALL 0x0002
#define ELF_CONE_TWIST 0x0003

#define ELF_POINT_LIGHT 0x0001  // <mdoc> LIGHT TYPES <mdocc> The light types used by elf.SetLightType
#define ELF_SUN_LIGHT 0x0002
#define ELF_SPOT_LIGHT 0x0003

#define ELF_LUMINANCE 0x0000  // <mdoc> TEXTURE FORMATS <mdocc> The texture formats returned by elf.GetTextureFormat
#define ELF_LUMINANCE_ALPHA 0x0001
#define ELF_RGB 0x0002
#define ELF_RGBA 0x0003
#define ELF_BGR 0x0004
#define ELF_BGRA 0x0005
#define ELF_RGB16F 0x0006
#define ELF_RGB32F 0x0007
#define ELF_RGBA16F 0x0008
#define ELF_RGBA32F 0x0009
#define ELF_ALPHA32F 0x000A
#define ELF_DEPTH_COMPONENT 0x000B
#define ELF_COMPRESSED_RGB 0x000C
#define ELF_COMPRESSED_RGBA 0x000D

#define ELF_FLOAT \
    0x0000  // <mdoc> TEXTURE DATA FORMATS <mdocc> The texture formats returned by elf.GetTextureDataFormat
#define ELF_INT 0x0001
#define ELF_UINT 0x0002
#define ELF_SHORT 0x0003
#define ELF_USHORT 0x0004
#define ELF_BYTE 0x0005
#define ELF_UBYTE 0x0006

#define ELF_COLOR_MAP 0x0001  // <mdoc> TEXTURE TYPES <mdocc> The textures types
#define ELF_NORMAL_MAP 0x0002
#define ELF_HEIGHT_MAP 0x0004
#define ELF_SPECULAR_MAP 0x0008
#define ELF_DETAIL_MAP 0x0010
#define ELF_SHADOW_MAP 0x0020
#define ELF_COLOR_RAMP_MAP 0x0040
#define ELF_LIGHT_MAP 0x0080
#define ELF_CUBE_MAP 0x0100

#define ELF_TRANSPARENT 0x0001  // <mdoc> PARTICLE DRAW MODES <mdocc> The draw modes used by elf.SetParticlesDrawMode
#define ELF_ADD 0x0002

#define ELF_DIR 0x0001  // <mdoc> DIRECTORY ITEM TYPES <mdocc> The directory item types used by elf.GetDirectoryItemType
#define ELF_FILE 0x0002

#define ELF_CLICKED \
    0x0001  // <mdoc> GUI EVENTS <mdocc> The gui events triggered by gui objects. The gui events are automatically set
            // to the "event" variable when in a gui action.
#define ELF_VALUE_CHANGED 0x0002
#define ELF_GAIN_FOCUS 0x0003
#define ELF_LOSE_FOCUS 0x0004
#define ELF_CHAR_INPUT 0x0005
#define ELF_SELECTION_CHANGED 0x0006
#define ELF_STATE_CHANGED 0x0007
#define ELF_DROP 0x0008

#define ELF_JOYSTICK_BUTTON_1 \
    0x0000  // <mdoc> JOYSTICK BUTTONS <mdocc> The joystick buttons used by elf.GetJoystickButtonState
#define ELF_JOYSTICK_BUTTON_2 0x0001
#define ELF_JOYSTICK_BUTTON_3 0x0002
#define ELF_JOYSTICK_BUTTON_4 0x0003
#define ELF_JOYSTICK_BUTTON_5 0x0004
#define ELF_JOYSTICK_BUTTON_6 0x0005
#define ELF_JOYSTICK_BUTTON_7 0x0006
#define ELF_JOYSTICK_BUTTON_8 0x0007
#define ELF_JOYSTICK_BUTTON_9 0x0008
#define ELF_JOYSTICK_BUTTON_10 0x0009
#define ELF_JOYSTICK_BUTTON_11 0x000A
#define ELF_JOYSTICK_BUTTON_12 0x000B
#define ELF_JOYSTICK_BUTTON_13 0x000C
#define ELF_JOYSTICK_BUTTON_14 0x000D
#define ELF_JOYSTICK_BUTTON_15 0x000E
#define ELF_JOYSTICK_BUTTON_16 0x000F

// <!!
#define ELF_ARMATURE_MAGIC 179532122
#define ELF_CAMERA_MAGIC 179532111
#define ELF_ENTITY_MAGIC 179532112
#define ELF_LIGHT_MAGIC 179532113
#define ELF_MATERIAL_MAGIC 179532109
#define ELF_MODEL_MAGIC 179532110
#define ELF_PARTICLES_MAGIC 179532141
#define ELF_SCENE_MAGIC 179532120
#define ELF_SCRIPT_MAGIC 179532121
#define ELF_SPRITE_MAGIC 179532140
#define ELF_TEXTURE_MAGIC 179532108

#define ELF_NAME_LENGTH 128

#define ELF_PAK_VERSION 104
    // !!>

    typedef struct elfVec2i elfVec2i;
    typedef struct elfVec2f elfVec2f;
    typedef struct elfVec3f elfVec3f;
    typedef struct elfVec4f elfVec4f;
    typedef struct elfColor elfColor;

    typedef struct elfGeneral elfGeneral;
    typedef struct elfConfig elfConfig;
    typedef struct elfObject elfObject;
    typedef struct elfResource elfResource;
    typedef struct elfGuiObject elfGuiObject;
    typedef struct elfList elfList;
    typedef struct elfKeyEvent elfKeyEvent;
    typedef struct elfCharEvent elfCharEvent;
    typedef struct elfContext elfContext;
    typedef struct elfEngine elfEngine;
    typedef struct elfTimer elfTimer;

    typedef struct elfImage elfImage;
    typedef struct elfTexture elfTexture;
    typedef struct elfMaterial elfMaterial;
    typedef struct elfBezierPoint elfBezierPoint;
    typedef struct elfBezierCurve elfBezierCurve;
    typedef struct elfIpo elfIpo;
    typedef struct elfActor elfActor;
    typedef struct elfCamera elfCamera;
    typedef struct elfModel elfModel;
    typedef struct elfEntity elfEntity;
    typedef struct elfLight elfLight;
    typedef struct elfScene elfScene;
    typedef struct elfPakIndex elfPakIndex;
    typedef struct elfPak elfPak;
    typedef struct elfPostProcess elfPostProcess;
    typedef struct elfScript elfScript;
    typedef struct elfAudioDevice elfAudioDevice;
    typedef struct elfAudioSource elfAudioSource;
    typedef struct elfSound elfSound;
    typedef struct elfBone elfBone;
    typedef struct elfArmature elfArmature;
    typedef struct elfString elfString;
    typedef struct elfFont elfFont;
    typedef struct elfArea elfArea;
    typedef struct elfLabel elfLabel;
    typedef struct elfButton elfButton;
    typedef struct elfPicture elfPicture;
    typedef struct elfTextField elfTextField;
    typedef struct elfSlider elfSlider;
    typedef struct elfScreen elfScreen;
    typedef struct elfTextList elfTextList;
    typedef struct elfCheckBox elfCheckBox;
    typedef struct elfGui elfGui;
    typedef struct elfDirectoryItem elfDirectoryItem;
    typedef struct elfDirectory elfDirectory;
    typedef struct elfCollision elfCollision;
    typedef struct elfPhysicsTriMesh elfPhysicsTriMesh;
    typedef struct elfPhysicsObject elfPhysicsObject;
    typedef struct elfPhysicsWorld elfPhysicsWorld;
    typedef struct elfJoint elfJoint;
    typedef struct elfResources elfResources;
    typedef struct elfParticle elfParticle;
    typedef struct elfParticles elfParticles;
    typedef struct elfFramePlayer elfFramePlayer;
    typedef struct elfProperty elfProperty;
    typedef struct elfScripting elfScripting;
    typedef struct elfSprite elfSprite;
    typedef struct elfVideoMode elfVideoMode;
    typedef struct elfVertex elfVertex;
    typedef struct elfFace elfFace;
    typedef struct elfMeshData elfMeshData;
    typedef struct elfRenderStation elfRenderStation;
    typedef struct elfRequest elfRequest;

    // <!!
    // !!>

    //////////////////////////////// GENERAL ////////////////////////////////

    // <!!
    elfGeneral* elfCreateGeneral();
    void elfDestroyGeneral(elfGeneral* general);

    void elfInitGeneral();
    void elfDeinitGeneral();

    void elfSetLogFilePath(const char* filePath);

    void elfIncObj(int type);
    void elfDecObj(int type);

    void elfDumpRefTable();
    void elfDumpObjTable();
    // !!>

    void elfIncRef(elfObject* obj);  // <mdoc> OBJECT FUNCTIONS <mdocc> The object functions can be performed on any
                                     // generic ELF objects.
    void elfDecRef(elfObject* obj);
    int elfGetObjectType(elfObject* obj);
    int elfGetObjectRefCount(elfObject* obj);
    int elfGetGlobalRefCount();
    int elfGetGlobalObjCount();
    unsigned char elfIsActor(elfObject* obj);
    unsigned char elfIsGuiObject(elfObject* obj);

    //////////////////////////////// RESOURCE ////////////////////////////////

    // <!!
    elfResource* elfGetResource(elfList* resources, const char* name);
    elfResource* elfGetResourceById(elfList* resources, int id);
    void elfSetResourceUniqueName(elfList* namedObjects, elfResource* object);
    // !!>

    //////////////////////////////// STRING ////////////////////////////////

    // <!!
    elfString* elfCreateStringObject();
    void elfDestroyStringObject(void* data);
    char* elfCreateString(const char* str);
    void elfDestroyString(char* str);
    char* elfRemoveCharFromString(char* str, int idx);
    char* elfAppendCharToString(char* str, char c);
    char* elfInsertCharToString(char* str, int idx, char c);
    char* elfMergeStrings(const char* str1, const char* str2);
    char* elfMergeStrings3(const char* str1, const char* str2, const char* str3);
    char* elfSubString(char* str, int start, int len);
    unsigned char elfIsCharNumber(char c);
    unsigned char elfIsStringNumber(const char* str);
    unsigned char elfIsStringPositiveInt(const char* str);
    int elfRFindCharFromString(char chr, const char* str);
    int elfRFindCharsFromString(char* chrs, const char* str);
    // !!>

    //////////////////////////////// LIST ////////////////////////////////

    elfList* elfCreateList();  // <mdoc> LIST FUNCTIONS
    /* <!> */ void elfDestroyList(void* data);
    int elfGetListLength(elfList* list);
    void elfInsertListObject(elfList* list, int idx, elfObject* obj);
    void elfAppendListObject(elfList* list, elfObject* obj);
    unsigned char elfRemoveListObject(elfList* list, elfObject* obj);
    elfObject* elfGetListObject(elfList* list, int idx);
    elfObject* elfBeginList(elfList* list);
    elfObject* elfGetListNext(elfList* list);
    elfObject* elfRBeginList(elfList* list);
    elfObject* elfGetListRNext(elfList* list);
    /* <!> */ void elfSetListCurPtr(elfList* list, elfObject* ptr);
    void elfSeekList(elfList* list, elfObject* ptr);
    void elfRSeekList(elfList* list, elfObject* ptr);

    /////////////////////////////// CONFIG ///////////////////////////////

    // <!!
    elfConfig* elfCreateConfig();
    void elfDestroyConfig(void* data);
    // !!>

    elfConfig* elfReadConfig(const char* filePath);  // <mdoc> CONFIG FUNCTIONS

    void elfSetConfigWindowSize(elfConfig* config, int width, int height);
    void elfSetConfigMultisamples(elfConfig* config, int multisamples);
    void elfSetConfigFullscreen(elfConfig* config, unsigned char fullscreen);
    void elfSetConfigTextureCompress(elfConfig* config, unsigned char textureCompress);
    void elfSetConfigTextureAnisotropy(elfConfig* config, float textureAnisotropy);
    void elfSetConfigShadowMapSize(elfConfig* config, int shadowMapSize);
    void elfSetConfigStart(elfConfig* config, const char* start);
    void elfSetConfigLogPath(elfConfig* config, const char* logPath);

    elfVec2i elfGetConfigWindowSize(elfConfig* config);
    int elfGetConfigMultisamples(elfConfig* config);
    unsigned char elfGetConfigFullscreen(elfConfig* config);
    unsigned char elfGetConfigTextureCompress(elfConfig* config);
    float elfGetConfigTextureAnisotropy(elfConfig* config);
    int elfGetConfigShadowMapSize(elfConfig* config);
    const char* elfGetConfigStart(elfConfig* config);
    const char* elfGetConfigLogPath(elfConfig* config);

    ///////////////////////////////// LOG /////////////////////////////////

    // <!!
    void elfStartLog();
    void elfLogWrite(const char* fmt, ...);
    void elfSetError(int code, const char* fmt, ...);
    // !!>

    void elfWriteLogLine(const char* str);  // <mdoc> LOG FUNCTIONS

    /////////////////////////////// CONTEXT ///////////////////////////////

    // <!!
    elfVideoMode* elfCreateVideoMode();
    void elfDestroyVideoMode(void* data);

    elfKeyEvent* elfCreateKeyEvent();
    void elfDestroyKeyEvent(void* data);

    elfCharEvent* elfCreateCharEvent();
    void elfDestroyCharEvent(void* data);

    elfContext* elfCreateContext();
    void elfDestroyContext(void* data);

    unsigned char elfInitContext(int width, int height, const char* title, int multisamples, unsigned char fullscreen);
    void elfCloseWindow();

    unsigned char elfResizeContext(int width, int height);
    // !!>

    void elfSetTitle(const char* title);  // <mdoc> CONTEXT FUNCTIONS

    int elfGetWindowWidth();
    int elfGetWindowHeight();
    int elfGetVideoModeCount();
    elfVec2i elfGetVideoMode(int idx);
    int elfGetMultisamples();
    unsigned char elfIsFullscreen();
    const char* elfGetTitle();
    double elfGetTime();
    void elfSleep(float time);
    unsigned char elfIsWindowOpened();
    /* <!> */ void elfSwapBuffers();
    elfVec2i elfGetMousePosition();
    elfVec2i elfGetMouseForce();
    void elfSetMousePosition(int x, int y);
    void elfHideMouse(unsigned char hide);
    unsigned char elfIsMouseHidden();
    int elfGetMouseWheel();
    int elfGetMouseButtonState(int button);
    int elfGetKeyState(int key);
    unsigned char elfGetJoystickPresent(int joy);
    elfVec2f elfGetJoystickAxis(int joy);
    int elfGetJoystickButtonState(int joy, int but);
    int elfGetEventCount();
    elfObject* elfGetEvent(int idx);
    int elfGetKeyEventKey(elfKeyEvent* keyEvent);
    int elfGetKeyEventState(elfKeyEvent* keyEvent);

    //////////////////////////////// ENGINE ///////////////////////////////

    // <!!
    void elfWriteToLog(const char* fmt, ...);
    void elfSetError(int code, const char* fmt, ...);
    void elfSetErrorNoSave(int code, const char* fmt, ...);

    elfEngine* elfCreateEngine();
    void elfDestroyEngine(void* data);

    unsigned char elfInitEngine();
    void elfDeinitEngine();
    // !!>

    unsigned char elfInit(elfConfig* config);  // <mdoc> ENGINE FUNCTIONS
    void elfDeinit();

    void elfResizeWindow(int width, int height);

    const char* elfGetPlatform();

    int elfGetVersionMajor();
    int elfGetVersionMinor();
    const char* elfGetVersionRelease();
    const char* elfGetVersion();

    char* elfGetFileFromPath(const char* filePath);
    char* elfGetDirectoryFromPath(const char* filePath);
    const char* elfGetCurrentDirectory();

    const char* elfGetErrorString();
    int elfGetError();

    unsigned char elfRun();
    void elfQuit();

    void elfSetF10Exit(unsigned char exit);
    unsigned char elfGetF10Exit();

    elfScene* elfLoadScene(const char* filePath);
    void elfSetScene(elfScene* scene);
    elfScene* elfGetScene();

    void elfSetGui(elfGui* gui);
    elfGui* elfGetGui();

    float elfGetSync();
    int elfGetFps();

    unsigned char elfSaveScreenShot(const char* filePath);

    void elfSetFpsLimit(float fpsLimit);
    float elfGetFpsLimit();

    void elfSetTickRate(float tickRate);
    float elfGetTickRate();

    void elfSetSpeed(float speed);
    float elfGetSpeed();

    void elfSetTextureCompress(unsigned char compress);
    unsigned char elfGetTextureCompress();
    void elfSetTextureAnisotropy(float anisotropy);
    float elfGetTextureAnisotropy();

    void elfSetShadowMapSize(int size);
    int elfGetShadowMapSize();

    int elfGetPolygonsRendered();

    void elfSetBloom(float threshold);
    void elfDisableBloom();
    float elfGetBloomThreshold();

    void elfSetDof(float focalRange, float focalDistance);
    void elfDisableDof();
    float elfGetDofFocalRange();
    float elfGetDofFocalDistance();

    void elfSetSsao(float amount);
    void elfDisableSsao();
    float elfGetSsaoAmount();

    void elfSetLightShafts(float intensity);
    void elfDisableLightShafts();
    float elfGetLightShaftsIntensity();

    unsigned char elfIsBloom();
    unsigned char elfIsSsao();
    unsigned char elfIsDof();
    unsigned char elfIsLightShafts();

    elfObject* elfGetActor();

    // <!!
    elfDirectoryItem* elfCreateDirectoryItem();
    void elfDestroyDirectoryItem(void* data);
    elfDirectory* elfCreateDirectory();
    void elfDestroyDirectory(void* data);
    // !!>

    elfDirectory* elfReadDirectory(const char* path);

    const char* elfGetDirectoryPath(elfDirectory* directory);
    int elfGetDirectoryItemCount(elfDirectory* directory);
    elfDirectoryItem* elfGetDirectoryItem(elfDirectory* directory, int idx);
    const char* elfGetDirectoryItemName(elfDirectoryItem* dirItem);
    int elfGetDirectoryItemType(elfDirectoryItem* dirItem);

    elfColor elfCreateColor(float r, float g, float b, float a);
    elfVec2f elfCreateVec2f(float x, float y);
    elfVec3f elfCreateVec3f(float x, float y, float z);
    elfVec4f elfCreateQua(float x, float y, float z, float w);
    elfVec4f elfCreateQuaFromEuler(float x, float y, float z);
    elfVec4f elfCreateQuaFromAngleAxis(float angle, float x, float y, float z);
    elfVec2f elfAddVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
    elfVec2f elfSubVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
    elfVec2f elfMulVec2fVec2f(elfVec2f vec1, elfVec2f vec2);
    float elfGetVec2fLength(elfVec2f vec);
    elfVec3f elfAddVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
    elfVec3f elfSubVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
    elfVec3f elfMulVec3fVec3f(elfVec3f vec1, elfVec3f vec2);
    float elfGetVec3fLength(elfVec3f vec);
    elfVec3f elfRotateVec3f(elfVec3f vec, float x, float y, float z);
    elfVec3f elfMulQuaVec3f(elfVec4f qua, elfVec3f vec);
    elfVec4f elfMulQuaQua(elfVec4f qua1, elfVec4f qua2);
    elfVec4f elfGetQuaInverted(elfVec4f qua);

    unsigned char elfAboutSame(float v1, float v2);
    unsigned char elfAboutZero(float val);
    float elfFloatAbs(float val);
    float elfFloatMin(float a, float b);
    float elfFloatMax(float a, float b);
    float elfRandomFloat();
    float elfRandomFloatRange(float min, float max);
    int elfRandomInt();
    int elfRandomIntRange(int min, int max);

    /////////////////////////// RENDER STATION ////////////////////////////

    // <!!
    elfRenderStation* elfCreateRenderStation();
    void elfDestroyRenderStation(void* data);
    unsigned char elfInitRenderStation();
    void elfDeinitRenderStation();

    void elfDraw2dQuad(float x, float y, float width, float height);
    void elfDrawTextured2dQuad(float x, float y, float width, float height);
    void elfDrawTextured2dQuadRegion(float x, float y, float width, float height, float tx, float ty, float twidth,
                                     float theight);
    float elfGetAbsoluteValue(float val);
    void elfDrawCircle(float x, float y, int vertices, float size);
    void elfDrawBoundingBox(float min[3], float max[3]);
    void elfDrawLines(int count, gfxVertexData* vertices);
    void elfDrawLineLoop(int count, gfxVertexData* vertices);
    void elfDrawHorGradient(int x, int y, int width, int height, elfColor col1, elfColor col2);
    void elfDrawHorGradientBorder(int x, int y, int width, int height, elfColor col1, elfColor col2);
    // !!>

    ////////////////////////////// RESOURCES //////////////////////////////

    // <!!
    elfResources* elfCreateResources();
    void elfDestroyResources(void* data);
    unsigned char elfInitResources();
    void elfDeinitResources();
    // !!>

    elfTexture* elfGetResourcesTexture(const char* filePath, const char* name);

    elfTexture* elfGetOrLoadResourcesTexture(const char* filePath, const char* name);

    ///////////////////////////// FRAME PLAYER ////////////////////////////

    /* <!> */ void elfDestroyFramePlayer(void* data);

    elfFramePlayer* elfCreateFramePlayer();  // <mdoc> FRAME PLAYER FUNCTIONS
    void elfUpdateFramePlayer(elfFramePlayer* player);
    void elfSetFramePlayerFrame(elfFramePlayer* player, float frame);
    void elfPlayFramePlayer(elfFramePlayer* player, float start, float end, float speed);
    void elfLoopFramePlayer(elfFramePlayer* player, float start, float end, float speed);
    void elfStopFramePlayer(elfFramePlayer* player);
    void elfPauseFramePlayer(elfFramePlayer* player);
    void elfResumeFramePlayer(elfFramePlayer* player);
    float elfGetFramePlayerStart(elfFramePlayer* player);
    float elfGetFramePlayerEnd(elfFramePlayer* player);
    float elfGetFramePlayerSpeed(elfFramePlayer* player);
    float elfGetFramePlayerFrame(elfFramePlayer* player);
    unsigned char elfIsFramePlayerPlaying(elfFramePlayer* player);
    unsigned char elfIsFramePlayerPaused(elfFramePlayer* player);
    // <!!
    void elfSetFramePlayerUserData(elfFramePlayer* player, void* userData);
    void* elfGetFramePlayerUserData(elfFramePlayer* player);
    void elfSetFramePlayerCallback(elfFramePlayer* player, void (*callback)(elfFramePlayer*));
    // !!>

    //////////////////////////////// TIMER ////////////////////////////////

    elfTimer* elfCreateTimer();  // <mdoc> TIMER FUNCTIONS
    /* <!> */ void elfDestroyTimer(void* data);
    void elfStartTimer(elfTimer* timer);
    double elfGetElapsedTime(elfTimer* timer);

    //////////////////////////////// IMAGE ////////////////////////////////

    /* <!> */ elfImage* elfCreateImage();
    elfImage* elfCreateEmptyImage(int width, int height, int bpp);  // IMAGE FUNCTIONS
    elfImage* elfCreateImageFromFile(const char* filePath);
    /* <!> */ void elfDestroyImage(void* data);

    void elfSetImagePixel(elfImage* image, int x, int y, float r, float g, float b, float a);

    int elfGetImageWidth(elfImage* image);
    int elfGetImageHeight(elfImage* image);
    int elfGetImageBitsPerPixel(elfImage* image);
    elfColor elfGetImagePixel(elfImage* image, int x, int y);

    // <!!
    void* elfGetImageData(elfImage* image);
    unsigned char elfSaveImageData(const char* filePath, int width, int height, unsigned char bpp, void* data);
    // !!>

    /////////////////////////////// TEXTURE ///////////////////////////////

    // <!!
    elfTexture* elfCreateTexture();
    void elfDestroyTexture(void* data);
    // !!>

    elfTexture* elfCreateTextureFromFile(const char* name, const char* filePath);  // <mdoc> TEXTURE FUNCTIONS
    elfTexture* elfCreateTextureFromImage(const char* name, elfImage* image);
    elfTexture* elfCreateCubeMapFromFiles(const char* name, const char* xpos, const char* xneg, const char* ypos,
                                          const char* yneg, const char* zpos, const char* zneg);

    void elfSetTextureName(elfTexture* texture, const char* name);

    const char* elfGetTextureName(elfTexture* texture);
    const char* elfGetTextureFilePath(elfTexture* texture);
    int elfGetTextureWidth(elfTexture* texture);
    int elfGetTextureHeight(elfTexture* texture);
    int elfGetTextureFormat(elfTexture* texture);
    int elfGetTextureDataFormat(elfTexture* texture);
    // <!!
    gfxTexture* elfGetGfxTexture(elfTexture* texture);
    void elfSetTexture(int slot, elfTexture* texture, gfxShaderParams* shaderParams);
    // !!>

    //////////////////////////////// MATERIAL ////////////////////////////////

    /* <!> */ void elfDestroyMaterial(void* data);

    elfMaterial* elfCreateMaterial(const char* name);  // <mdoc> MATERIAL FUNCTIONS

    void elfSetMaterialDiffuseColor(elfMaterial* material, float r, float g, float b, float a);
    void elfSetMaterialSpecularColor(elfMaterial* material, float r, float g, float b, float a);
    void elfSetMaterialAmbientColor(elfMaterial* material, float r, float g, float b, float a);
    void elfSetMaterialSpecularPower(elfMaterial* material, float power);
    void elfSetMaterialLighting(elfMaterial* material, unsigned char lighting);

    void elfSetMaterialName(elfMaterial* material, const char* name);
    const char* elfGetMaterialName(elfMaterial* material);
    const char* elfGetMaterialFilePath(elfMaterial* material);
    elfColor elfGetMaterialDiffuseColor(elfMaterial* material);
    elfColor elfGetMaterialSpecularColor(elfMaterial* material);
    elfColor elfGetMaterialAmbientColor(elfMaterial* material);
    float elfGetMaterialSpecularPower(elfMaterial* material);
    unsigned char elfGetMaterialLighting(elfMaterial* material);

    void elfSetMaterialDiffuseMap(elfMaterial* material, elfTexture* texture);
    void elfSetMaterialNormalMap(elfMaterial* material, elfTexture* texture);
    void elfSetMaterialHeightMap(elfMaterial* material, elfTexture* texture);
    void elfSetMaterialSpecularMap(elfMaterial* material, elfTexture* texture);
    void elfSetMaterialLightMap(elfMaterial* material, elfTexture* texture);
    void elfSetMaterialCubeMap(elfMaterial* material, elfTexture* texture);

    void elfClearMaterialDiffuseMap(elfMaterial* material);
    void elfClearMaterialNormalMap(elfMaterial* material);
    void elfClearMaterialHeightMap(elfMaterial* material);
    void elfClearMaterialSpecularMap(elfMaterial* material);
    void elfClearMaterialLightMap(elfMaterial* material);
    void elfClearMaterialCubeMap(elfMaterial* material);

    elfTexture* elfGetMaterialDiffuseMap(elfMaterial* material);
    elfTexture* elfGetMaterialNormalMap(elfMaterial* material);
    elfTexture* elfGetMaterialHeightMap(elfMaterial* material);
    elfTexture* elfGetMaterialSpecularMap(elfMaterial* material);
    elfTexture* elfGetMaterialLightMap(elfMaterial* material);
    elfTexture* elfGetMaterialCubeMap(elfMaterial* material);

    void elfSetMaterialParallaxScale(elfMaterial* material, float scale);

    void elfSetMaterialAlphaTest(elfMaterial* material, unsigned char alphaTest);
    void elfSetMaterialAlphaThreshold(elfMaterial* material, float threshold);

    float elfGetMaterialParallaxScale(elfMaterial* material);

    unsigned char elfGetMaterialAlphaTest(elfMaterial* material);
    float elfGetMaterialAlphaThreshold(elfMaterial* material);

    // <!!
    void elfSetMaterial(elfMaterial* material, int mode, gfxShaderParams* shaderParams);
    // !!>

    //////////////////////////////// IPOS ////////////////////////////////

    // <!!
    void elfDestroyBezierPoint(void* data);
    void elfDestroyBezierCurve(void* data);
    void elfDestroyIpo(void* data);
    // !!>

    elfBezierPoint* elfCreateBezierPoint();  // <mdoc> IPO FUNCTIONS
    void elfSetBezierPointPosition(elfBezierPoint* point, float x, float y);
    void elfSetBezierPointControl1(elfBezierPoint* point, float x, float y);
    void elfSetBezierPointControl2(elfBezierPoint* point, float x, float y);
    elfVec2f elfGetBezierPointPosition(elfBezierPoint* point);
    elfVec2f elfGetBezierPointControl1(elfBezierPoint* point);
    elfVec2f elfGetBezierPointControl2(elfBezierPoint* point);

    elfBezierCurve* elfCreateBezierCurve();
    void elfSetBezierCurveType(elfBezierCurve* curve, int type);
    int elfGetBezierCurveType(elfBezierCurve* curve);
    void elfAddBezierCurvePoint(elfBezierCurve* curve, elfBezierPoint* point);
    elfBezierPoint* elfGetPointFromBezierCurve(elfBezierCurve* curve, int idx);
    float elfGetBezierCurveValue(elfBezierCurve* curve, float x);

    elfIpo* elfCreateIpo();
    unsigned char elfAddIpoCurve(elfIpo* ipo, elfBezierCurve* curve);
    int elfGetIpoCurveCount(elfIpo* ipo);
    elfBezierCurve* elfGetCurveFromIpo(elfIpo* ipo, int idx);
    elfVec3f elfGetIpoLoc(elfIpo* ipo, float x);
    elfVec3f elfGetIpoRot(elfIpo* ipo, float x);
    elfVec3f elfGetIpoScale(elfIpo* ipo, float x);
    elfVec4f elfGetIpoQua(elfIpo* ipo, float x);

    //////////////////////////////// PROPERTY ////////////////////////////////

    elfProperty* elfCreateProperty(const char* name);  // <mdoc> PROPERTY FUNCTIONS
    /* <!> */ void elfDestroyProperty(void* data);

    int elfGetPropertyType(elfProperty* property);

    int elfGetPropertyInt(elfProperty* property);
    float elfGetPropertyFloat(elfProperty* property);
    const char* elfGetPropertyString(elfProperty* property);
    unsigned char elfGetPropertyBool(elfProperty* property);

    void elfSetPropertyInt(elfProperty* property, int ival);
    void elfSetPropertyFloat(elfProperty* property, float fval);
    void elfSetPropertyString(elfProperty* property, const char* sval);
    void elfSetPropertyBool(elfProperty* property, unsigned char bval);

    //////////////////////////////// ACTOR ////////////////////////////////

    // <!!
    void elfInitActor(elfActor* actor, unsigned char camera);
    void elfUpdateActor(elfActor* actor);
    void elfActorPreDraw(elfActor* actor);
    void elfActorPostDraw(elfActor* actor);
    void elfCleanActor(elfActor* actor);
    // !!>

    const char* elfGetActorName(elfActor* actor);  // <mdoc> ACTOR FUNCTIONS
    const char* elfGetActorFilePath(elfActor* actor);
    elfScript* elfGetActorScript(elfActor* actor);

    void elfSetActorName(elfActor* actor, const char* name);
    void elfSetActorScript(elfActor* actor, elfScript* script);
    void elfClearActorScript(elfActor* actor);

    void elfSetActorPosition(elfActor* actor, float x, float y, float z);
    void elfSetActorRotation(elfActor* actor, float x, float y, float z);
    void elfSetActorOrientation(elfActor* actor, float x, float y, float z, float w);
    void elfRotateActor(elfActor* actor, float x, float y, float z);
    void elfRotateActorLocal(elfActor* actor, float x, float y, float z);
    void elfMoveActor(elfActor* actor, float x, float y, float z);
    void elfMoveActorLocal(elfActor* actor, float x, float y, float z);
    void elfSetActorPositionRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z);
    void elfSetActorRotationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z);
    void elfSetActorOrientationRelativeTo(elfActor* actor, elfActor* to, float x, float y, float z, float w);
    elfVec3f elfGetActorPosition(elfActor* actor);
    elfVec3f elfGetActorRotation(elfActor* actor);
    elfVec4f elfGetActorOrientation(elfActor* actor);

    // <!!
    void elfGetActorPosition_(elfActor* actor, float* params);
    void elfGetActorRotation_(elfActor* actor, float* params);
    void elfGetActorOrientation_(elfActor* actor, float* params);
    // !!>

    void elfSetActorPhysics(elfActor* actor, unsigned char physics);
    void elfSetActorShape(elfActor* actor, int shape);
    void elfSetActorBoundingLengths(elfActor* actor, float x, float y, float z);
    void elfSetActorBoundingOffset(elfActor* actor, float x, float y, float z);
    /* <!> */ void elfResetActorBoundingOffsetSetFlag(elfActor* actor);
    void elfSetActorMass(elfActor* actor, float mass);

    void elfSetActorDamping(elfActor* actor, float linDamp, float angDamp);
    void elfSetActorSleep(elfActor* actor, float linThrs, float angThrs);
    void elfSetActorRestitution(elfActor* actor, float restitution);
    void elfSetActorAnisotropicFriction(elfActor* actor, float x, float y, float z);
    void elfSetActorLinearFactor(elfActor* actor, float x, float y, float z);
    void elfSetActorAngularFactor(elfActor* actor, float x, float y, float z);

    void elfAddActorForce(elfActor* actor, float x, float y, float z);
    void elfAddActorForceLocal(elfActor* actor, float x, float y, float z);
    void elfAddActorTorque(elfActor* actor, float x, float y, float z);
    void elfSetActorLinearVelocity(elfActor* actor, float x, float y, float z);
    void elfSetActorLinearVelocityLocal(elfActor* actor, float x, float y, float z);
    void elfSetActorAngularVelocity(elfActor* actor, float x, float y, float z);

    unsigned char elfGetActorPhysics(elfActor* actor);
    elfVec3f elfGetActorBoundingLengths(elfActor* actor);
    elfVec3f elfGetActorBoundingOffset(elfActor* actor);
    int elfGetActorShape(elfActor* actor);
    float elfGetActorMass(elfActor* actor);

    float elfGetActorLinearDamping(elfActor* actor);
    float elfGetActorAngularDamping(elfActor* actor);
    float elfGetActorLinearSleep(elfActor* actor);
    float elfGetActorAngularSleep(elfActor* actor);
    float elfGetActorRestitution(elfActor* actor);
    elfVec3f elfGetActorAnisotropicFriction(elfActor* actor);
    elfVec3f elfGetActorLinearFactor(elfActor* actor);
    elfVec3f elfGetActorAngularFactor(elfActor* actor);
    elfVec3f elfGetActorLinearVelocity(elfActor* actor);
    elfVec3f elfGetActorAngularVelocity(elfActor* actor);

    elfJoint* elfAddActorHingeJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz,
                                    float ax, float ay, float az);
    elfJoint* elfAddActorBallJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py, float pz);
    elfJoint* elfAddActorConeTwistJoint(elfActor* actor, elfActor* actor2, const char* name, float px, float py,
                                        float pz, float ax, float ay, float az);
    elfJoint* elfGetActorJoint(elfActor* actor, const char* name);
    elfJoint* elfGetActorJointByIndex(elfActor* actor, int idx);
    unsigned char elfRemoveActorJoint(elfActor* actor, const char* name);
    unsigned char elfRemoveActorJointByIndex(elfActor* actor, int idx);
    unsigned char elfRemoveActorJointByObject(elfActor* actor, elfJoint* joint);

    void elfSetActorIpo(elfActor* actor, elfIpo* ipo);
    elfIpo* elfGetActorIpo(elfActor* actor);

    void elfSetActorIpoFrame(elfActor* actor, float frame);
    void elfPlayActorIpo(elfActor* actor, float start, float end, float speed);
    void elfLoopActorIpo(elfActor* actor, float start, float end, float speed);
    void elfStopActorIpo(elfActor* actor);
    void elfPauseActorIpo(elfActor* actor);
    void elfResumeActorIpo(elfActor* actor);
    float elfGetActorIpoStart(elfActor* actor);
    float elfGetActorIpoEnd(elfActor* actor);
    float elfGetActorIpoSpeed(elfActor* actor);
    float elfGetActorIpoFrame(elfActor* actor);
    unsigned char elfIsActorIpoPlaying(elfActor* actor);
    unsigned char elfIsActorIpoPaused(elfActor* actor);

    int elfGetActorCollisionCount(elfActor* actor);
    elfCollision* elfGetActorCollision(elfActor* actor, int idx);

    int elfGetActorPropertyCount(elfActor* actor);
    void elfAddPropertyToActor(elfActor* actor, elfProperty* property);
    elfProperty* elfGetActorPropertyByName(elfActor* actor, const char* name);
    elfProperty* elfGetActorPropertyByIndex(elfActor* actor, int idx);
    unsigned char elfRemoveActorPropertyByName(elfActor* actor, const char* name);
    unsigned char elfRemoveActorPropertyByIndex(elfActor* actor, int idx);
    unsigned char elfRemoveActorPropertyByObject(elfActor* actor, elfProperty* property);
    void elfRemoveActorProperties(elfActor* actor);

    void elfSetActorSelected(elfActor* actor, unsigned char selected);
    unsigned char elfGetActorSelected(elfActor* actor);

    /* <!> */ void elfDrawActorDebug(elfActor* actor, gfxShaderParams* shaderParams);

    //////////////////////////////// CAMERA ////////////////////////////////

    // <!!
    void elfUpdateCamera(elfCamera* camera);
    void elfCameraPreDraw(elfCamera* camera);
    void elfCamearPostDraw(elfCamera* camera);
    void elfDestroyCamera(void* data);
    // !!>

    elfCamera* elfCreateCamera(const char* name);  // <mdoc> CAMERA FUNCTIONS

    void elfSetCameraViewport(elfCamera* camera, int x, int y, int width, int height);
    void elfSetCameraOrthoViewport(elfCamera* camera, int x, int y, int width, int height);
    void elfSetCameraMode(elfCamera* camera, int mode);
    void elfSetCameraFov(elfCamera* camera, float fov);
    void elfSetCameraAspect(elfCamera* camera, float aspect);
    void elfSetCameraClip(elfCamera* camera, float clipNear, float clipFar);

    elfVec2i elfGetCameraViewportSize(elfCamera* camera);
    elfVec2i elfGetCameraViewportOffset(elfCamera* camera);
    elfVec2i elfGetCameraOrthoViewportSize(elfCamera* camera);
    elfVec2i elfGetCameraOrthoViewportOffset(elfCamera* camera);
    int elfGetCameraMode(elfCamera* camera);
    float elfGetCameraFov(elfCamera* camera);
    float elfGetCameraAspect(elfCamera* camera);
    elfVec2f elfGetCameraClip(elfCamera* camera);
    elfVec2f elfGetCameraFarPlaneSize(elfCamera* camera);

    // <!!
    float* elfGetCameraProjectionMatrix(elfCamera* camera);
    float* elfGetCameraModelviewMatrix(elfCamera* camera);
    void elfSetCamera(elfCamera* camera, gfxShaderParams* shaderParams);
    unsigned char elfAabbInsideFrustum(elfCamera* camera, float* min, float* max);
    unsigned char elfSphereInsideFrustum(elfCamera* camera, float* pos, float radius);
    unsigned char elfCameraInsideAabb(elfCamera* camera, float* min, float* max);
    unsigned char elfCameraInsideSphere(elfCamera* camera, float* pos, float radius);
    void elfDrawCameraDebug(elfCamera* camera, gfxShaderParams* shaderParams);
    // !!>

    elfVec3f elfProjectCameraPoint(elfCamera* camera, float x, float y, float z);
    elfVec3f elfUnProjectCameraPoint(elfCamera* camera, float x, float y, float z);

    //////////////////////////////// MESH DATA ////////////////////////////////

    elfVertex* elfCreateVertex();  // <mdoc> MESH DATA FUNCTIONS
    /* <!> */ void elfDestroyVertex(void* data);

    void elfSetVertexPosition(elfVertex* vertice, float x, float y, float z);
    void elfSetVertexNormal(elfVertex* vertice, float x, float y, float z);
    void elfSetVertexTexCoord(elfVertex* vertice, float u, float v);

    elfVec3f elfGetVertexPosition(elfVertex* vertice);
    elfVec3f elfGetVertexNormal(elfVertex* vertice);
    elfVec2f elfGetVertexTexCoord(elfVertex* vertice);

    // <!!
    elfFace* elfCreateFace();
    void elfDestroyFace(void* data);
    // !!>

    int elfGetFaceV1(elfFace* face);
    int elfGetFaceV2(elfFace* face);
    int elfGetFaceV3(elfFace* face);

    elfMeshData* elfCreateMeshData();
    /* <!> */ void elfDestroyMeshData(void* data);

    int elfGetMeshDataVertexCount(elfMeshData* meshData);
    int elfGetMeshDataFaceCount(elfMeshData* meshData);

    void elfAddMeshDataVertex(elfMeshData* meshData, elfVertex* vertice);
    void elfAddMeshDataFace(elfMeshData* meshData, int v1, int v2, int v3);

    elfVertex* elfGetVertexFromMeshData(elfMeshData* meshData, int idx);
    elfFace* elfGetFaceFromMeshData(elfMeshData* meshData, int idx);

    //////////////////////////////// MODEL ////////////////////////////////

    // <!!
    elfModel* elfCreateModel(const char* name);
    void elfDestroyModel(void* data);
    void elfGenerateModelTangents(elfModel* model);
    // !!>

    elfModel* elfCreateModelFromMeshData(elfMeshData* data);  // <mdoc> MODEL FUNCTIONS

    void elfSetModelName(elfModel* model, const char* name);
    const char* elfGetModelName(elfModel* model);
    const char* elfGetModelFilePath(elfModel* model);
    int elfGetModelVertexCount(elfModel* model);
    int elfGetModelIndiceCount(elfModel* model);
    elfVec3f elfGetModelBoundingBoxMin(elfModel* model);
    elfVec3f elfGetModelBoundingBoxMax(elfModel* model);

    // <!!
    float* elfGetModelVertexs(elfModel* model);
    float* elfGetModelNormals(elfModel* model);
    float* elfGetModelTexCoords(elfModel* model);
    float* elfGetModelTangents(elfModel* model);
    unsigned int* elfGetModelIndices(elfModel* model);

    void elfDrawModel(elfList* material, elfModel* model, int mode, gfxShaderParams* shaderParams);
    void elfDrawModelBoudingBox(elfModel* model, gfxShaderParams* shaderParams);
    // !!>

    //////////////////////////////// ENTITY ////////////////////////////////

    // <!!
    void elfUpdateEntity(elfEntity* entity);
    void elfEntityPreDraw(elfEntity* entity);
    void elfEntityPostDraw(elfEntity* entity);
    void elfDestroyEntity(void* data);
    void elfCalcEntityAabb(elfEntity* entity);
    void elfCalcEntityBoundingVolumes(elfEntity* entity, unsigned char newModel);
    // !!>

    elfEntity* elfCreateEntity(const char* name);  // <mdoc> ENTITY FUNCTIONS
    void elfGenerateEntityTangents(elfEntity* entity);

    void elfSetEntityScale(elfEntity* entity, float x, float y, float z);
    elfVec3f elfGetEntityScale(elfEntity* entity);
    /* <!> */ void elfGetEntityScale_(elfEntity* entity, float* scale);

    void elfSetEntityModel(elfEntity* entity, elfModel* model);
    void elfClearEntityModel(elfEntity* entity);
    elfModel* elfGetEntityModel(elfEntity* entity);
    int elfGetEntityMaterialCount(elfEntity* entity);
    void elfAddEntityMaterial(elfEntity* entity, elfMaterial* material);
    void elfSetEntityMaterial(elfEntity* entity, int idx, elfMaterial* material);
    void elfRemoveEntityMaterial(elfEntity* entity, int idx);
    elfMaterial* elfGetEntityMaterial(elfEntity* entity, int idx);

    void elfSetEntityVisible(elfEntity* entity, unsigned char visible);
    unsigned char elfGetEntityVisible(elfEntity* entity);

    void elfSetEntityOccluder(elfEntity* entity, unsigned char occluder);
    unsigned char elfGetEntityOccluder(elfEntity* entity);

    void elfSetEntityArmature(elfEntity* entity, elfArmature* armature);
    void elfSetEntityArmatureFrame(elfEntity* entity, float frame);
    void elfPlayEntityArmature(elfEntity* entity, float start, float end, float speed);
    void elfLoopEntityArmature(elfEntity* entity, float start, float end, float speed);
    void elfStopEntityArmature(elfEntity* entity);
    void elfPauseEntityArmature(elfEntity* entity);
    void elfResumeEntityArmature(elfEntity* entity);
    float elfGetEntityArmatureStart(elfEntity* entity);
    float elfGetEntityArmatureEnd(elfEntity* entity);
    float elfGetEntityArmatureSpeed(elfEntity* entity);
    float elfGetEntityArmatureFrame(elfEntity* entity);
    unsigned char elfIsEntityArmaturePlaying(elfEntity* entity);
    unsigned char elfIsEntityArmaturePaused(elfEntity* entity);

    elfArmature* elfGetEntityArmature(elfEntity* entity);

    // <!!
    void elfResetEntityDebugPhysicsObject(elfEntity* entity);
    void elfDrawEntity(elfEntity* entity, int mode, gfxShaderParams* shaderParams);
    void elfDrawEntityBoundingBox(elfEntity* entity, gfxShaderParams* shaderParams);
    void elfDrawEntityDebug(elfEntity* entity, gfxShaderParams* shaderParams);
    unsigned char elfCullEntity(elfEntity* entity, elfCamera* camera);
    // !!>

    unsigned char elfGetEntityChanged(elfEntity* entity);

    //////////////////////////////// LIGHT ////////////////////////////////

    // <!!
    void elfUpdateLight(elfLight* light);
    void elfLightPreDraw(elfLight* light);
    void elfLightPostDraw(elfLight* light);
    void elfDestroyLight(void* data);
    // !!>

    elfLight* elfCreateLight(const char* name);  // <mdoc> LIGHT FUNCTIONS

    void elfSetLightType(elfLight* light, int type);
    void elfSetLightColor(elfLight* light, float r, float g, float b, float a);
    void elfSetLightRange(elfLight* light, float range, float fadeRange);
    void elfSetLightShadows(elfLight* light, unsigned char shadows);
    void elfSetLightVisible(elfLight* light, unsigned char visible);
    void elfSetLightCone(elfLight* light, float innerCone, float outerCone);
    void elfSetLightShaft(elfLight* light, unsigned char shaft);
    void elfSetLightShaftSize(elfLight* light, float size);
    void elfSetLightShaftIntensity(elfLight* light, float intensity);
    void elfSetLightShaftFadeOff(elfLight* light, float fadeOff);

    int elfGetLightType(elfLight* light);
    elfColor elfGetLightColor(elfLight* light);
    float elfGetLightRange(elfLight* light);
    float elfGetLightFadeRange(elfLight* light);
    unsigned char elfGetLightShadows(elfLight* light);
    unsigned char elfGetLightVisible(elfLight* light);
    float elfGetLightInnerCone(elfLight* light);
    float elfGetLightOuterCone(elfLight* light);
    unsigned char elfGetLightShaft(elfLight* light);
    float elfGetLightShaftSize(elfLight* light);
    float elfGetLightShaftIntensity(elfLight* light);
    float elfGetLightShaftFadeOff(elfLight* light);

    // <!!
    void elfSetLight(elfLight* light, elfCamera* camera, gfxShaderParams* shaderParams);
    void elfDrawLightDebug(elfLight* light, gfxShaderParams* shaderParams);
    unsigned char elfGetLightChanged(elfLight* light);
    // !!>

    //////////////////////////////// ARMATURE ////////////////////////////////

    // <!!
    elfBone* elfCreateBone(const char* name);
    void elfDestroyBone(void* data);
    // !!>

    elfArmature* elfGetBoneArmature(elfBone* bone);  // <mdoc> ARMATURE FUNCTIONS
    elfBone* elfGetBoneParent(elfBone* bone);
    elfBone* elfGetBoneChildByName(elfBone* bone, const char* name);
    elfBone* elfGetBoneChildById(elfBone* bone, int id);
    elfBone* elfGetBoneChildByIndex(elfBone* bone, int idx);
    elfVec3f elfGetBonePosition(elfBone* bone);
    elfVec3f elfGetBoneRotation(elfBone* bone);
    elfVec4f elfGetBoneOrientation(elfBone* bone);

    // <!!
    void elfUpdateArmature(elfArmature* armature);
    void elfDestroyArmature(void* data);
    // !!>

    elfArmature* elfCreateArmature(const char* name);

    elfBone* elfGetBoneFromArmatureByName(const char* name, elfArmature* armature);
    elfBone* elfGetBoneFromArmatureById(int id, elfArmature* armature);

    // <!!
    void elfAddRootBoneToArmature(elfArmature* armature, elfBone* bone);

    void elfDeformEntityWithArmature(elfArmature* armature, elfEntity* entity, float frame);
    void elfDrawArmatureDebug(elfArmature* armature, gfxShaderParams* shaderParams);
    // !!>

    //////////////////////////////// PARTICLES ////////////////////////////////

    // <!!
    elfParticle* elfCreateParticle();
    void elfDestroyParticle(void* data);
    void elfParticlesPreDraw(elfParticles* particles);
    void elfParticlesPostDraw(elfParticles* particles);
    void elfUpdateParticles(elfParticles* particles, float sync);
    void elfDestroyParticles(void* data);
    // !!>

    elfParticles* elfCreateParticles(const char* name, int maxCount);  // <mdoc> PARTICLE FUNCTIONS

    const char* elfGetParticlesName(elfParticles* particles);
    const char* elfGetParticlesFilePath(elfParticles* particles);

    void elfSetParticlesMaxCount(elfParticles* particles, int maxCount);
    void elfSetParticlesDrawMode(elfParticles* particles, int mode);
    void elfSetParticlesTexture(elfParticles* particles, elfTexture* texture);
    void elfClearParticlesTexture(elfParticles* particles);
    void elfSetParticlesModel(elfParticles* particles, elfModel* model);
    void elfClearParticlesModel(elfParticles* particles);
    void elfSetParticlesEntity(elfParticles* particles, elfEntity* entity);
    void elfClearParticlesEntity(elfParticles* particles);
    void elfSetParticlesGravity(elfParticles* particles, float x, float y, float z);
    void elfSetParticlesSpawnCount(elfParticles* particles, int perSecond);
    void elfSetParticlesSpawn(elfParticles* particles, unsigned char spawn);

    void elfSetParticlesSize(elfParticles* particles, float min, float max);
    void elfSetParticlesSizeGrowth(elfParticles* particles, float min, float max);
    void elfSetParticlesRotation(elfParticles* particles, float min, float max);
    void elfSetParticlesRotationGrowth(elfParticles* particles, float min, float max);
    void elfSetParticlesLifeSpan(elfParticles* particles, float min, float max);
    void elfSetParticlesFadeSpeed(elfParticles* particles, float min, float max);
    void elfSetParticlesPositionMin(elfParticles* particles, float x, float y, float z);
    void elfSetParticlesPositionMax(elfParticles* particles, float x, float y, float z);
    void elfSetParticlesVelocityMin(elfParticles* particles, float x, float y, float z);
    void elfSetParticlesVelocityMax(elfParticles* particles, float x, float y, float z);
    void elfSetParticlesColorMin(elfParticles* particles, float r, float g, float b, float a);
    void elfSetParticlesColorMax(elfParticles* particles, float r, float g, float b, float a);

    int elfGetParticlesMaxCount(elfParticles* particles);
    int elfGetParticlesCount(elfParticles* particles);
    int elfGetParticlesDrawMode(elfParticles* particles);
    elfTexture* elfGetParticlesTexture(elfParticles* particles);
    elfModel* elfGetParticlesModel(elfParticles* particles);
    elfEntity* elfGetParticlesEntity(elfParticles* particles);
    elfVec3f elfGetParticlesGravity(elfParticles* particles);
    int elfGetParticlesSpawnCount(elfParticles* particles);
    unsigned char elfGetParticlesSpawn(elfParticles* particles);

    float elfGetParticlesSizeMin(elfParticles* particles);
    float elfGetParticlesSizeMax(elfParticles* particles);
    float elfGetParticlesSizeGrowthMin(elfParticles* particles);
    float elfGetParticlesSizeGrowthMax(elfParticles* particles);
    float elfGetParticlesRotationMin(elfParticles* particles);
    float elfGetParticlesRotationMax(elfParticles* particles);
    float elfGetParticlesRotationGrowthMin(elfParticles* particles);
    float elfGetParticlesRotationGrowthMax(elfParticles* particles);
    float elfGetParticlesLifeSpanMin(elfParticles* particles);
    float elfGetParticlesLifeSpanMax(elfParticles* particles);
    float elfGetParticlesFadeSpeedMin(elfParticles* particles);
    float elfGetParticlesFadeSpeedMax(elfParticles* particles);
    elfVec3f elfGetParticlesPositionMin(elfParticles* particles);
    elfVec3f elfGetParticlesPositionMax(elfParticles* particles);
    elfVec3f elfGetParticlesVelocityMin(elfParticles* particles);
    elfVec3f elfGetParticlesVelocityMax(elfParticles* particles);
    elfColor elfGetParticlesColorMin(elfParticles* particles);
    elfColor elfGetParticlesColorMax(elfParticles* particles);

    // <!!
    void elfDrawParticlesDebug(elfParticles* particles, gfxShaderParams* shaderParams);
    void elfDrawParticles(elfParticles* particles, elfCamera* camera, gfxShaderParams* shaderParams);
    unsigned char elfCullParticles(elfParticles* particles, elfCamera* camera);
    // !!>

    //////////////////////////////// SPRITE ////////////////////////////////

    // <!!
    void elfUpdateSprite(elfSprite* sprite);
    void elfSpritePreDraw(elfSprite* sprite, elfCamera* camera);
    void elfSpritePostDraw(elfSprite* sprite);
    void elfDestroySprite(void* data);
    // !!>

    elfSprite* elfCreateSprite(const char* name);  // <mdoc> SPRITE FUNCTIONS

    void elfSetSpriteMaterial(elfSprite* sprite, elfMaterial* material);
    void elfSetSpriteScale(elfSprite* sprite, float x, float y);
    void elfSetSpriteFaceCamera(elfSprite* sprite, unsigned char faceCamera);

    elfMaterial* elfGetSpriteMaterial(elfSprite* sprite);
    elfVec2f elfGetSpriteScale(elfSprite* sprite);
    unsigned char elfGetSpriteFaceCamera(elfSprite* sprite);

    void elfSetSpriteVisible(elfSprite* sprite, unsigned char visible);
    unsigned char elfGetSpriteVisible(elfSprite* sprite);

    void elfSetSpriteOccluder(elfSprite* sprite, unsigned char occluder);
    unsigned char elfGetSpriteOccluder(elfSprite* sprite);

    // <!!
    unsigned char elfCullSprite(elfSprite* sprite, elfCamera* camera);
    void elfDrawSprite(elfSprite* sprite, int mode, gfxShaderParams* shaderParams);
    void elfDrawSpriteDebug(elfSprite* sprite, gfxShaderParams* shaderParams);
    // !!>

    //////////////////////////////// SCENE ////////////////////////////////

    // <!!
    void elfUpdateScene(elfScene* scene, float sync);
    void elfScenePreDraw(elfScene* scene);
    void elfScenePostDraw(elfScene* scene);
    void elfDestroyScene(void* data);
    // !!>

    const char* elfGetSceneName(elfScene* scene);
    const char* elfGetSceneFilePath(elfScene* scene);

    elfScene* elfCreateScene(const char* name);  // <mdoc> SCENE FUNCTIONS
    elfScene* elfCreateSceneFromFile(const char* name, const char* filePath);
    unsigned char elfSaveScene(elfScene* scene, const char* filePath);

    void elfSetScenePhysics(elfScene* scene, unsigned char physics);
    unsigned char elfGetScenePhysics(elfScene* scene);

    void elfSetSceneRunScripts(elfScene* scene, unsigned char runScripts);
    unsigned char elfGetSceneRunScripts(elfScene* scene);

    void elfSetSceneDebugDraw(elfScene* scene, unsigned char debugDraw);
    unsigned char elfGetSceneDebugDraw(elfScene* scene);

    void elfSetSceneOcclusionCulling(elfScene* scene, unsigned char occlusionCulling);
    unsigned char elfGetSceneOcclusionCulling(elfScene* scene);

    void elfSetSceneGravity(elfScene* scene, float x, float y, float z);
    elfVec3f elfGetSceneGravity(elfScene* scene);

    void elfSetSceneAmbientColor(elfScene* scene, float r, float g, float b, float a);
    elfColor elfGetSceneAmbientColor(elfScene* scene);

    void elfSetSceneFog(elfScene* scene, unsigned char fog);
    void elfSetSceneFogStart(elfScene* scene, float start);
    void elfSetSceneFogEnd(elfScene* scene, float end);
    void elfSetSceneFogColor(elfScene* scene, float r, float g, float b, float a);

    unsigned char elfGetSceneFog(elfScene* scene);
    float elfGetSceneFogStart(elfScene* scene);
    float elfGetFogEnd(elfScene* scene);
    elfColor elfGetFogColor(elfScene* scene);

    int elfGetSceneCameraCount(elfScene* scene);
    int elfGetSceneEntityCount(elfScene* scene);
    int elfGetSceneLightCount(elfScene* scene);
    int elfGetSceneArmatureCount(elfScene* scene);
    int elfGetSceneParticlesCount(elfScene* scene);
    int elfGetSceneSpriteCount(elfScene* scene);

    void elfAddSceneCamera(elfScene* scene, elfCamera* camera);
    void elfAddSceneEntity(elfScene* scene, elfEntity* entity);
    void elfAddSceneLight(elfScene* scene, elfLight* light);
    void elfAddSceneParticles(elfScene* scene, elfParticles* particles);
    void elfAddSceneSprite(elfScene* scene, elfSprite* sprite);

    void elfSetSceneActiveCamera(elfScene* scene, elfCamera* camera);
    elfCamera* elfGetSceneActiveCamera(elfScene* scene);
    elfCollision* elfGetSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);
    elfList* elfGetSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);
    elfCollision* elfGetDebugSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy,
                                                float dz);
    elfList* elfGetDebugSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz);

    elfCamera* elfGetSceneCameraByIndex(elfScene* scene, int idx);
    elfEntity* elfGetSceneEntityByIndex(elfScene* scene, int idx);
    elfLight* elfGetSceneLightByIndex(elfScene* scene, int idx);
    elfArmature* elfGetSceneArmatureByIndex(elfScene* scene, int idx);
    elfParticles* elfGetSceneParticlesByIndex(elfScene* scene, int idx);
    elfSprite* elfGetSceneSpriteByIndex(elfScene* scene, int idx);

    elfTexture* elfGetSceneTexture(elfScene* scene, const char* name);
    elfMaterial* elfGetSceneMaterial(elfScene* scene, const char* name);
    elfModel* elfGetSceneModel(elfScene* scene, const char* name);
    elfScript* elfGetSceneScript(elfScene* scene, const char* name);
    elfCamera* elfGetSceneCamera(elfScene* scene, const char* name);
    elfEntity* elfGetSceneEntity(elfScene* scene, const char* name);
    elfLight* elfGetSceneLight(elfScene* scene, const char* name);
    elfArmature* elfGetSceneArmature(elfScene* scene, const char* name);
    elfParticles* elfGetSceneParticles(elfScene* scene, const char* name);
    elfSprite* elfGetSceneSprite(elfScene* scene, const char* name);
    elfActor* elfGetSceneActor(elfScene* scene, const char* name);

    // <!!
    elfTexture* elfGetOrLoadTextureByName(elfScene* scene, const char* name);
    elfModel* elfGetOrLoadModelByName(elfScene* scene, const char* name);
    elfScript* elfGetOrLoadScriptByName(elfScene* scene, const char* name);
    elfCamera* elfGetOrLoadCameraByName(elfScene* scene, const char* name);
    elfEntity* elfGetOrLoadEntityByName(elfScene* scene, const char* name);
    elfParticles* elfGetOrLoadParticlesByName(elfScene* scene, const char* name);
    elfSprite* elfGetOrLoadSpriteByName(elfScene* scene, const char* name);
    elfLight* elfGetOrLoadLightByName(elfScene* scene, const char* name);
    elfArmature* elfGetOrLoadArmatureByName(elfScene* scene, const char* name);
    elfMaterial* elfGetOrLoadMaterialByName(elfScene* scene, const char* name);
    elfActor* elfGetOrLoadActorByName(elfScene* scene, const char* name);

    void elfRemoveActor(elfActor* actor);
    // !!>
    unsigned char elfRemoveSceneCamera(elfScene* scene, const char* name);
    unsigned char elfRemoveSceneEntity(elfScene* scene, const char* name);
    unsigned char elfRemoveSceneLight(elfScene* scene, const char* name);
    unsigned char elfRemoveSceneParticles(elfScene* scene, const char* name);
    unsigned char elfRemoveSceneSprite(elfScene* scene, const char* name);

    unsigned char elfRemoveSceneCameraByIndex(elfScene* scene, int idx);
    unsigned char elfRemoveSceneEntityByIndex(elfScene* scene, int idx);
    unsigned char elfRemoveSceneLightByIndex(elfScene* scene, int idx);
    unsigned char elfRemoveSceneParticlesByIndex(elfScene* scene, int idx);
    unsigned char elfRemoveSceneSpriteByIndex(elfScene* scene, int idx);

    unsigned char elfRemoveSceneCameraByObject(elfScene* scene, elfCamera* camera);
    unsigned char elfRemoveSceneEntityByObject(elfScene* scene, elfEntity* entity);
    unsigned char elfRemoveSceneLightByObject(elfScene* scene, elfLight* light);
    unsigned char elfRemoveSceneParticlesByObject(elfScene* scene, elfParticles* particles);
    unsigned char elfRemoveSceneSpriteByObject(elfScene* scene, elfSprite* sprite);

    unsigned char elfRemoveSceneActorByObject(elfScene* scene, elfActor* actor);

    // <!!
    void elfDrawScene(elfScene* scene);
    void elfDrawSceneDebug(elfScene* scene);
    // !!>

    elfList* elfGetSceneScripts(elfScene* scene);
    elfList* elfGetSceneTextures(elfScene* scene);
    elfList* elfGetSceneMaterials(elfScene* scene);
    elfList* elfGetSceneModels(elfScene* scene);

    //////////////////////////////// PAK ////////////////////////////////

    // <!!
    elfPakIndex* elfCreatePakIndex();
    void elfDestroyPakIndex(void* data);
    elfPak* elfCreatePakFromFile(const char* filePath);
    void elfDestroyPak(void* data);

    const char* elfGetPakFilePath(elfPak* pak);
    int elfGetPakIndexCount(elfPak* pak);

    elfPakIndex* elfGetPakIndexByName(elfPak* pak, const char* name, unsigned char type);
    elfPakIndex* elfGetPakIndexByIndex(elfPak* pak, int idx);
    unsigned char elfGetPakIndexType(elfPakIndex* index);
    const char* elfGetPakIndexName(elfPakIndex* index);
    int elfGetPakIndexOffset(elfPakIndex* index);

    int elfGetActorHeaderSizeBytes(elfActor* actor);
    int elfGetArmatureSizeBytes(elfArmature* armature);
    int elfGetCameraSizeBytes(elfCamera* camera);
    int elfGetEntitySizeBytes(elfEntity* entity);
    int elfGetLightSizeBytes(elfLight* light);
    int elfGetMaterialSizeBytes(elfMaterial* material);
    int elfGetModelSizeBytes(elfModel* model);
    int elfGetParticlesSizeBytes(elfParticles* particles);
    int elfGetSceneSizeBytes(elfScene* scene);
    int elfGetScriptSizeBytes(elfScript* script);
    int elfGetSpriteSizeBytes(elfSprite* sprite);
    int elfGetTextureSizeBytes(elfTexture* texture);

    void elfReadActorHeader(elfActor* actor, FILE* file, elfScene* scene);
    elfArmature* elfCreateArmatureFromPak(FILE* file, const char* name, elfScene* scene);
    elfCamera* elfCreateCameraFromPak(FILE* file, const char* name, elfScene* scene);
    elfEntity* elfCreateEntityFromPak(FILE* file, const char* name, elfScene* scene);
    elfLight* elfCreateLightFromPak(FILE* file, const char* name, elfScene* scene);
    elfMaterial* elfCreateMaterialFromPak(FILE* file, const char* name, elfScene* scene);
    elfModel* elfCreateModelFromPak(FILE* file, const char* name, elfScene* scene);
    elfParticles* elfCreateParticlesFromPak(FILE* file, const char* name, elfScene* scene);
    elfScript* elfCreateScriptFromPak(FILE* file, const char* name, elfScene* scene);
    elfSprite* elfCreateSpriteFromPak(FILE* file, const char* name, elfScene* scene);
    elfTexture* elfCreateTextureFromPak(FILE* file, const char* name, elfScene* scene);
    unsigned char elfLoadTextureDataFromPak(elfTexture* texture);

    elfScene* elfCreateSceneFromPak(const char* name, elfPak* pak);

    void elfWriteActorHeader(elfActor* actor, FILE* file);
    void elfWriteArmatureToFile(elfArmature* armature, FILE* file);
    void elfWriteCameraToFile(elfCamera* camera, FILE* file);
    void elfWriteEntityToFile(elfEntity* entity, FILE* file);
    void elfWriteLightToFile(elfLight* light, FILE* file);
    void elfWriteMaterialToFile(elfMaterial* material, FILE* file);
    void elfWriteModelToFile(elfModel* model, FILE* file);
    void elfWriteParticlesToFile(elfParticles* particles, FILE* file);
    void elfWriteScriptToFile(elfScript* script, FILE* file);
    void elfWriteSpriteToFile(elfSprite* sprite, FILE* file);
    void elfWriteTextureToFile(elfTexture* texture, FILE* file);

    unsigned char elfSaveSceneToPak(elfScene* scene, const char* filePath);
    // !!>

    //////////////////////////////// POST PROCESS ////////////////////////////////

    // <!!
    elfPostProcess* elfCreatePostProcess();
    void elfDestroyPostProcess(void* data);

    void elfInitPostProcessBuffers(elfPostProcess* postProcess);

    void elfRunPostProcess(elfPostProcess* postProcess, elfScene* scene);

    void elfSetPostProcessBloom(elfPostProcess* postProcess, float threshold);
    void elfDisablePostProcessBloom(elfPostProcess* postProcess);
    float elfGetPostProcessBloomThreshold(elfPostProcess* postProcess);

    void elfSetPostProcessDof(elfPostProcess* postProcess, float focalRange, float focalDistance);
    void elfDisablePostProcessDof(elfPostProcess* postProcess);
    float elfGetPostProcessDofFocalRange(elfPostProcess* postProcess);
    float elfGetPostProcessDofFocalDistance(elfPostProcess* postProcess);

    void elfSetPostProcessSsao(elfPostProcess* postProcess, float amount);
    void elfDisablePostProcessSsao(elfPostProcess* postProcess);
    float elfGetPostProcessSsaoAmount(elfPostProcess* postProcess);

    void elfSetPostProcessLightShafts(elfPostProcess* postProcess, float intensity);
    void elfDisablePostProcessLightShafts(elfPostProcess* postProcess);
    float elfGetPostProcessLightShaftsIntensity(elfPostProcess* postProcess);

    unsigned char elfIsPostProcessBloom(elfPostProcess* postProcess);
    unsigned char elfIsPostProcessSsao(elfPostProcess* postProcess);
    unsigned char elfIsPostProcessDof(elfPostProcess* postProcess);
    unsigned char elfIsPostProcessLightShafts(elfPostProcess* postProcess);
    // !!>

    //////////////////////////////// SCRIPT ////////////////////////////////

    // <!!
    void elfDestroyScript(void* data);
    // !!>

    elfScript* elfCreateScript(const char* name);  // <mdoc> SCRIPT FUNCTIONS
    elfScript* elfCreateScriptFromFile(const char* name, const char* filePath);

    void elfSetScriptName(elfScript* script, const char* name);

    const char* elfGetScriptName(elfScript* script);
    const char* elfGetScriptFilePath(elfScript* script);

    void elfSetScriptText(elfScript* script, const char* text);
    unsigned char elfIsScriptError(elfScript* script);

    //////////////////////////////// SCRIPTING ////////////////////////////////

    // <!!
    elfScripting* elfCreateScripting();
    void elfDestroyScripting(void* data);

    unsigned char elfInitScripting();
    void elfUpdateScripting();
    void elfDeinitScripting();

    void elfSetScriptError(int err, const char* msg);
    // !!>

    unsigned char elfRunString(const char* str);  // <mdoc> SCRIPTING FUNCTIONS
    unsigned char elfRunScript(elfScript* script);

    //////////////////////////////// AUDIO ////////////////////////////////

    // <!!
    elfAudioDevice* elfCreateAudioDevice();
    void elfDestroyAudioDevice(void* data);
    unsigned char elfInitAudio();
    void elfUpdateAudio();
    void elfDeinitAudio();
    // !!>
    void elfSetAudioVolume(float volume);  // <mdoc> AUDIO FUNCTIONS
    float elfGetAudioVolume();
    void elfSetAudioRolloff(float rolloff);
    float elfGetAudioRolloff();
    // <!!
    void elfSetAudioListenerPosition(float x, float y, float z);
    void elfSetAudioListenerOrientation(float* params);
    // !!>

    // <!!
    elfSound* elfCreateSound();
    void elfDestroySound(void* data);
    // !!>

    elfSound* elfLoadSound(const char* filePath);
    elfSound* elfLoadStreamedSound(const char* filePath);
    int elfGetSoundFileType(elfSound* sound);

    elfAudioSource* elfPlaySound(elfSound* sound, float volume);
    elfAudioSource* elfPlayEntitySound(elfEntity* entity, elfSound* sound, float volume);
    elfAudioSource* elfLoopSound(elfSound* sound, float volume);
    elfAudioSource* elfLoopEntitySound(elfEntity* entity, elfSound* sound, float volume);
    // <!!
    elfAudioSource* elfCreateAudioSource();
    void elfStreamAudioSource(elfAudioSource* source);
    void elfDestroyAudioSource(void* data);
    // !!>
    void elfSetSoundVolume(elfAudioSource* source, float volume);
    float elfGetSoundVolume(elfAudioSource* source);
    void elfPauseSound(elfAudioSource* source);
    void elfResumeSound(elfAudioSource* source);
    void elfStopSound(elfAudioSource* source);

    unsigned char elfIsSoundPlaying(elfAudioSource* source);
    unsigned char elfIsSoundPaused(elfAudioSource* source);
    // <!!
    void elfSetSoundPosition(elfAudioSource* source, float x, float y, float z);
    // !!>

    //////////////////////////////// PHYSICS ////////////////////////////////

    // <!!
    elfCollision* elfCreateCollision();
    void elfDestroyCollision(void* data);

    elfPhysicsWorld* elfCreatePhysicsWorld();
    void elfDestroyPhysicsWorld(void* data);
    void elfUpdatePhysicsWorld(elfPhysicsWorld* world, float time);

    void elfSetPhysicsWorldGravity(elfPhysicsWorld* world, float x, float y, float z);
    elfVec3f elfGetPhysicsWorldGravity(elfPhysicsWorld* world);

    elfCollision* elfGetRayCastResult(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz);
    elfList* elfGetRayCastResults(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz);
    // !!>

    elfActor* elfGetCollisionActor(elfCollision* collision);  // <mdoc> COLLISION FUNCTIONS
    elfVec3f elfGetCollisionPosition(elfCollision* collision);
    elfVec3f elfGetCollisionNormal(elfCollision* collision);
    float elfGetCollisionDepth(elfCollision* collision);

    // <!!
    elfJoint* elfCreateJoint();
    void elfSetJointWorld(elfJoint* joint, elfPhysicsWorld* world);
    elfJoint* elfCreateHingeJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                                  float ax, float ay, float az);
    elfJoint* elfCreateBallJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz);
    elfJoint* elfCreateConeTwistJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py,
                                      float pz, float ax, float ay, float az);
    void elfClearJoint(elfJoint* joint);
    void elfDestroyJoint(void* data);
    // !!>

    const char* elfGetJointName(elfJoint* joint);  // <mdoc> JOINT FUNCTIONS
    int elfGetJointType(elfJoint* joint);
    elfActor* elfGetJointActorA(elfJoint* joint);
    elfActor* elfGetJointActorB(elfJoint* joint);
    elfVec3f elfGetJointPivot(elfJoint* joint);
    elfVec3f elfGetJointAxis(elfJoint* joint);

    // <!!
    elfPhysicsTriMesh* elfCreatePhysicsTriMesh(float* verts, unsigned int* idx, int indiceCount);
    void elfDestroyPhysicsTriMesh(void* data);

    elfPhysicsObject* elfCreatePhysicsObject();
    elfPhysicsObject* elfCreatePhysicsObjectMesh(elfPhysicsTriMesh* triMesh, float mass);
    elfPhysicsObject* elfCreatePhysicsObjectSphere(float radius, float mass, float ox, float oy, float oz);
    elfPhysicsObject* elfCreatePhysicsObjectBox(float hx, float hy, float hz, float mass, float ox, float oy, float oz);
    elfPhysicsObject* elfCreatePhysicsObjectCapsule(unsigned char type, float length, float radius, float mass,
                                                    float ox, float oy, float oz);
    elfPhysicsObject* elfCreatePhysicsObjectCone(unsigned char type, float length, float radius, float mass, float ox,
                                                 float oy, float oz);
    void elfSetPhysicsObjectWorld(elfPhysicsObject* object, elfPhysicsWorld* world);
    void elfDestroyPhysicsObject(void* data);

    void elfSetPhysicsObjectActor(elfPhysicsObject* object, elfActor* actor);
    void elfRemovePhysicsObjectCollisions(elfPhysicsObject* objects);
    void elfClearPhysicsObjectCollisions(elfPhysicsObject* objects);

    int elfGetPhysicsObjectCollisionCount(elfPhysicsObject* object);
    elfCollision* elfGetPhysicsObjectCollision(elfPhysicsObject* object, int idx);

    void elfSetPhysicsObjectPosition(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectOrientation(elfPhysicsObject* object, float x, float y, float z, float w);
    void elfSetPhysicsObjectScale(elfPhysicsObject* object, float x, float y, float z);

    int elfGetPhysicsObjectShape(elfPhysicsObject* object);
    float elfGetPhysicsObjectMass(elfPhysicsObject* object);
    void elfGetPhysicsObjectPosition(elfPhysicsObject* object, float* params);
    void elfGetPhysicsObjectOrientation(elfPhysicsObject* object, float* params);
    void elfGetPhysicsObjectScale(elfPhysicsObject* object, float* params);
    unsigned char elfIsPhysicsObjectStatic(elfPhysicsObject* object);

    void elfSetPhysicsObjectDamping(elfPhysicsObject* object, float linDamp, float angDamp);
    void elfSetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectSleepThresholds(elfPhysicsObject* object, float linThrs, float angThrs);
    void elfSetPhysicsObjectRestitution(elfPhysicsObject* object, float restitution);
    void elfAddPhysicsObjectForce(elfPhysicsObject* object, float x, float y, float z);
    void elfAddPhysicsObjectTorque(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectLinearFactor(elfPhysicsObject* object, float x, float y, float z);
    void elfSetPhysicsObjectAngularFactor(elfPhysicsObject* object, float x, float y, float z);

    void elfGetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float* params);
    void elfGetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float* params);
    void elfGetPhysicsObjectLinearFactor(elfPhysicsObject* object, float* params);
    void elfGetPhysicsObjectAngularFactor(elfPhysicsObject* object, float* params);
    float elfGetPhysicsObjectLinearDamping(elfPhysicsObject* object);
    float elfGetPhysicsObjectAngularDamping(elfPhysicsObject* object);
    void elfGetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float* params);
    float elfGetPhysicsObjectLinearSleepThreshold(elfPhysicsObject* object);
    float elfGetPhysicsObjectAngularSleepThreshold(elfPhysicsObject* object);
    float elfGetPhysicsObjectRestitution(elfPhysicsObject* object);
    // !!>

    //////////////////////////////// FONT ////////////////////////////////

    // <!!
    elfFont* elfCreateFont();
    void elfDestroyFont(void* data);
    // !!>

    elfFont* elfCreateFontFromFile(const char* filePath, int size);  // <mdoc> FONT FUNCTIONS

    elfFont* elfGetDefaultFont();
    const char* elfGetFontName(elfFont* font);
    const char* elfGetFontFilePath(elfFont* font);
    int elfGetFontSize(elfFont* font);
    int elfGetStringWidth(elfFont* font, const char* str);
    int elfGetStringHeight(elfFont* font, const char* str);

    /* <!> */ void elfDrawString(elfFont* font, const char* str, int x, int y, gfxShaderParams* shaderParams);

    //////////////////////////////// GUI ////////////////////////////////

    const char* elfGetGuiObjectName(elfGuiObject* object);  // <mdoc> GUI OBJECT FUNCTIONS <mdocc> Gui object functions
                                                            // can be performed on any gui object.
    elfVec2i elfGetGuiObjectPosition(elfGuiObject* object);
    elfVec2i elfGetGuiObjectSize(elfGuiObject* object);
    elfColor elfGetGuiObjectColor(elfGuiObject* object);
    unsigned char elfGetGuiObjectVisible(elfGuiObject* object);
    unsigned char elfGetGuiObjectActive(elfGuiObject* object);
    elfScript* elfGetGuiObjectScript(elfGuiObject* object);
    int elfGetGuiObjectEvent(elfGuiObject* object);

    void elfSetGuiObjectPosition(elfGuiObject* object, float x, float y);
    void elfSetGuiObjectColor(elfGuiObject* object, float r, float g, float b, float a);
    void elfSetGuiObjectVisible(elfGuiObject* object, unsigned char visible);
    void elfSetGuiObjectActive(elfGuiObject* object, unsigned char active);
    void elfSetGuiObjectScript(elfGuiObject* object, elfScript* script);

    // <!!
    void elfDestroyLabel(void* data);
    void elfDrawLabel(elfLabel* label, gfxShaderParams* shaderParams);
    void elfRecalcLabel(elfLabel* label);
    // !!>

    elfLabel* elfCreateLabel(const char* name);  // <mdoc> LABEL FUNCTIONS

    elfFont* elfGetLabelFont(elfLabel* label);
    const char* elfGetLabelText(elfLabel* label);

    void elfSetLabelFont(elfLabel* label, elfFont* font);
    void elfSetLabelText(elfLabel* label, const char* text);

    // <!!
    void elfDestroyButton(void* data);
    void elfDrawButton(elfButton* button, gfxShaderParams* shaderParams);
    void elfRecalcButton(elfButton* button);
    // !!>

    elfButton* elfCreateButton(const char* name);  // <mdoc> BUTTON FUNCTION

    unsigned char elfGetButtonState(elfButton* button);
    const char* elfGetButtonText(elfButton* button);
    elfFont* elfGetButtonFont(elfButton* button);
    elfTexture* elfGetButtonOffTexture(elfButton* button);
    elfTexture* elfGetButtonOverTexture(elfButton* button);
    elfTexture* elfGetButtonOnTexture(elfButton* button);
    elfColor elfGetButtonTextColor(elfTextField* textField);

    void elfSetButtonTextColor(elfButton* button, float r, float g, float b, float a);
    void elfSetButtonText(elfButton* button, const char* text);
    void elfSetButtonFont(elfButton* button, elfFont* font);
    void elfSetButtonSize(elfButton* button, int width, int height);
    void elfSetButtonOffTexture(elfButton* button, elfTexture* off);
    void elfSetButtonOverTexture(elfButton* button, elfTexture* over);
    void elfSetButtonOnTexture(elfButton* button, elfTexture* on);

    // <!!
    void elfDestroyPicture(void* data);
    void elfDrawPicture(elfPicture* picture, gfxShaderParams* shaderParams);
    void elfRecalcPicture(elfPicture* picture);
    // !!>

    elfPicture* elfCreatePicture(const char* name);  // <mdoc> PICTURE FUNCTIONS

    elfTexture* elfGetPictureTexture(elfPicture* picture);
    elfVec2f elfGetPictureScale(elfPicture* picture);

    void elfSetPictureTexture(elfPicture* picture, elfTexture* texture);
    void elfSetPictureScale(elfPicture* picture, float x, float y);

    // <!!
    void elfDestroyTextField(void* data);
    void elfDrawTextField(elfTextField* textField, elfArea* area, gfxShaderParams* shaderParams);
    void elfRecalcTextField(elfTextField* textField);
    // !!>

    elfTextField* elfCreateTextField(const char* name);  // <mdoc> TEXT FIELD FUNCTIONS

    elfTexture* elfGetTextFieldTexture(elfTextField* textField);
    elfFont* elfGetTextFieldFont(elfTextField* textField);
    elfColor elfGetTextFieldTextColor(elfTextField* textField);
    elfVec2i elfGetTextFieldOffset(elfTextField* textField);
    const char* elfGetTextFieldText(elfTextField* textField);

    void elfSetTextFieldFont(elfTextField* textField, elfFont* font);
    void elfSetTextFieldWidth(elfTextField* textField, int width);
    void elfSetTextFieldTexture(elfTextField* textField, elfTexture* texture);
    void elfSetTextFieldTextColor(elfTextField* textField, float r, float g, float b, float a);
    void elfSetTextFieldOffset(elfTextField* textField, int offsetX, int offsetY);
    void elfSetTextFieldCursorPosition(elfTextField* textField, int idx);
    void elfSetTextFieldText(elfTextField* textField, const char* text);

    // <!!
    void elfDestroySlider(void* data);
    void elfDrawSlider(elfSlider* slider, gfxShaderParams* shaderParams);
    void elfRecalcSlider(elfSlider* slider);
    // !!>

    elfSlider* elfCreateSlider(const char* name);  // <mdoc> SLIDER FUNCTIONS

    elfTexture* elfGetSliderBackgroundTexture(elfSlider* slider);
    elfTexture* elfGetSliderSliderTexture(elfSlider* slider);
    float elfGetSliderValue(elfSlider* slider);

    void elfSetSliderSize(elfSlider* slider, int width, int height);
    void elfSetSliderBackgroundTexture(elfSlider* slider, elfTexture* background);
    void elfSetSliderSliderTexture(elfSlider* slider, elfTexture* sliderTexture);
    void elfSetSliderValue(elfSlider* slider, float value);

    // <!!
    void elfDestroyScreen(void* data);
    void elfDrawScreen(elfScreen* screen, elfArea* area, gfxShaderParams* shaderParams);
    void elfRecalcScreen(elfScreen* screen);
    // !!>

    elfScreen* elfCreateScreen(const char* name);  // <mdoc> SCREEN FUCNTIONS

    void elfSetScreenSize(elfScreen* screen, int width, int height);
    elfTexture* elfGetScreenTexture(elfScreen* screen);
    void elfSetScreenTexture(elfScreen* screen, elfTexture* texture);
    void elfSetScreenToTop(elfScreen* screen);
    void elfForceScreenFocus(elfScreen* screen);
    void elfReleaseScreenFocus(elfScreen* screen);

    // <!!
    void elfDestroyTextList(void* data);
    void elfDrawTextList(elfTextList* textList, elfArea* area, gfxShaderParams* shaderParams);
    void elfRecalcTextList(elfTextList* textList);
    // !!>

    elfTextList* elfCreateTextList(const char* name);  // <mdoc> TEXT LIST FUNCTIONS

    elfFont* elfGetTextListFont(elfTextList* textList);
    int elfGetTextListRowCount(elfTextList* textList);
    int elfGetTextListItemCount(elfTextList* textList);
    int elfGetTextListSelectionIndex(elfTextList* textList);
    int elfGetTextListOffset(elfTextList* textList);
    const char* elfGetTextListItem(elfTextList* textList, int idx);
    const char* elfGetTextListSelectedItem(elfTextList* textList);
    unsigned char elfGetTextListItemDrag(elfTextList* textList);

    void elfSetTextListFont(elfTextList* textList, elfFont* font);
    void elfSetTextListSize(elfTextList* textList, int rows, int width);
    void elfAddTextListItem(elfTextList* textList, const char* text);
    void elfSetTextListItem(elfTextList* textList, int idx, const char* text);
    unsigned char elfRemoveTextListItem(elfTextList* textList, int idx);
    void elfRemoveTextListItems(elfTextList* textList);
    void elfSetTextListOffset(elfTextList* textList, int offset);
    void elfSetTextListSelection(elfTextList* textList, int selection);
    void elfSetTextListItemDrag(elfTextList* textList, unsigned char itemDrag);

    // <!!
    void elfDestroyCheckBox(void* data);
    void elfDrawCheckBox(elfCheckBox* checkBox, gfxShaderParams* shaderParams);
    void elfRecalcCheckBox(elfCheckBox* checkBox);
    // !!>

    elfCheckBox* elfCreateCheckBox(const char* name);  // <mdoc> CHECK BOX FUNCTIONS

    unsigned char elfGetCheckBoxState(elfCheckBox* checkBox);
    elfTexture* elfGetCheckBoxOffTexture(elfCheckBox* checkBox);
    elfTexture* elfGetCheckBoxOnTexture(elfCheckBox* checkBox);

    void elfSetCheckBoxOffTexture(elfCheckBox* checkBox, elfTexture* off);
    void elfSetCheckBoxOnTexture(elfCheckBox* checkBox, elfTexture* on);
    void elfSetCheckBoxState(elfCheckBox* checkBox, unsigned char state);

    /* <!> */ void elfRecalcGuiObject(elfGuiObject* object);

    // <!!
    void elfDestroyGui(void* data);
    void elfUpdateGui(elfGui* gui, float step);
    void elfDrawGui(elfGui* gui);
    // !!>

    elfGui* elfCreateGui();  // <mdoc> GUI FUNCTIONS

    unsigned char elfAddGuiObject(elfGuiObject* parent, elfGuiObject* object);
    elfGuiObject* elfGetGuiObjectByName(elfGuiObject* parent, const char* name);
    elfGuiObject* elfGetGuiObjectByIndex(elfGuiObject* parent, int idx);
    unsigned char elfRemoveGuiObjectByName(elfGuiObject* parent, const char* name);
    unsigned char elfRemoveGuiObjectByIndex(elfGuiObject* parent, int idx);
    unsigned char elfRemoveGuiObjectByObject(elfGuiObject* parent, elfGuiObject* object);

    elfGuiObject* elfGetGuiTrace(elfGui* gui);
    elfGuiObject* elfGetGuiFocus(elfGui* gui);
    elfGuiObject* elfGetGuiActiveTextField(elfGui* gui);
    unsigned char elfGetGuiDragging(elfGui* gui);
    elfGuiObject* elfGetGuiDragObject(elfGui* gui);
    const char* elfGetGuiDragContent(elfGui* gui);

    void elfEmptyGui(elfGui* gui);

    // <!!
    void elfDestroyRequest(void* data);
    // !!>

    elfRequest* elfCreateRequest(const char* name);  // <mdoc> NETWORKING FUNCTIONS
    void elfSendRequest(elfRequest* req);
    const char* elfGetRequestUrl(elfRequest* request);
    void elfSetRequestUrl(elfRequest* request, const char* url);
    const char* elfGetRequestMethod(elfRequest* request);
    void elfSetRequestMethod(elfRequest* request, const char* method);
    int elfGetRequestState();

    //////////////////////////////// SST ////////////////////////////////

    // <!!
    char* elfReadSstNext(const char* text, int* pos, const char* sep);
    char* elfReadSstText(const char* text, int* pos);
    char* elfReadSstString(const char* text, int* pos);
    float elfReadSstFloat(const char* text, int* pos);
    void elfReadSstFloats(const char* text, int* pos, int n, float* params);
    int elfReadSstInt(const char* text, int* pos);
    void elfReadSstInts(const char* text, int* pos, int n, int* params);
    unsigned char elfReadSstBool(const char* text, int* pos);
// !!>

// <!!
#ifdef __cplusplus
}
#endif

#endif  // BLENDELF_H
// !!>
