
void mouseButtonCallback(int button, int state);
void mousePositionCallback(int x, int y);
void mouseWheelCallback(int wheel);
void keyCallback(int key, int state);
void charCallback(int code, int state);

elfVideoMode* elfCreateVideoMode()
{
    elfVideoMode* videoMode;

    videoMode = (elfVideoMode*)malloc(sizeof(elfVideoMode));
    memset(videoMode, 0x0, sizeof(elfVideoMode));
    videoMode->objType = ELF_VIDEO_MODE;
    videoMode->objDestr = elfDestroyVideoMode;

    elfIncObj(ELF_VIDEO_MODE);

    return videoMode;
}

void elfDestroyVideoMode(void* data)
{
    elfVideoMode* videoMode = (elfVideoMode*)data;

    free(videoMode);

    elfDecObj(ELF_VIDEO_MODE);
}

elfKeyEvent* elfCreateKeyEvent()
{
    elfKeyEvent* keyEvent;

    keyEvent = (elfKeyEvent*)malloc(sizeof(elfKeyEvent));
    memset(keyEvent, 0x0, sizeof(elfKeyEvent));
    keyEvent->objType = ELF_KEY_EVENT;
    keyEvent->objDestr = elfDestroyKeyEvent;

    elfIncObj(ELF_KEY_EVENT);

    return keyEvent;
}

void elfDestroyKeyEvent(void* data)
{
    elfKeyEvent* keyEvent = (elfKeyEvent*)data;

    free(keyEvent);

    elfDecObj(ELF_KEY_EVENT);
}

elfCharEvent* elfCreateCharEvent()
{
    elfCharEvent* charEvent;

    charEvent = (elfCharEvent*)malloc(sizeof(elfCharEvent));
    memset(charEvent, 0x0, sizeof(elfCharEvent));
    charEvent->objType = ELF_CHAR_EVENT;
    charEvent->objDestr = elfDestroyCharEvent;

    elfIncObj(ELF_CHAR_EVENT);

    return charEvent;
}

void elfDestroyCharEvent(void* data)
{
    elfCharEvent* charEvent = (elfCharEvent*)data;

    free(charEvent);

    elfDecObj(ELF_CHAR_EVENT);
}

elfContext* elfCreateContext()
{
    elfContext* context;

    context = (elfContext*)malloc(sizeof(elfContext));
    memset(context, 0x0, sizeof(elfContext));
    context->objType = ELF_CONTEXT;
    context->objDestr = elfDestroyContext;

    context->videoModes = elfCreateList();
    context->events = elfCreateList();

    elfIncRef((elfObject*)context->videoModes);
    elfIncRef((elfObject*)context->events);

    elfIncObj(ELF_CONTEXT);

    return context;
}

void elfDestroyContext(void* data)
{
    elfContext* context = (elfContext*)data;

    if (context->title)
        elfDestroyString(context->title);

    elfDecRef((elfObject*)context->videoModes);
    elfDecRef((elfObject*)context->events);

    free(context);

    elfDecObj(ELF_CONTEXT);
}

unsigned char elfInitContext(int width, int height, const char* title, int multisamples, unsigned char fullscreen)
{
    int i;
    int videoModeCount;
    GLFWvidmode* vidmodes;
    elfVideoMode* videoMode;

    if (ctx)
    {
        elfLogWrite("warning: can not open window twice\n");
        return ELF_FALSE;
    }

    if (width < 1 || height < 1)
    {
        elfSetError(ELF_INVALID_SIZE, "error: invalid window size (%d, %d)\n", width, height);
        return ELF_FALSE;
    }

    ctx = elfCreateContext();
    elfIncRef((elfObject*)ctx);

    ctx->width = width;
    ctx->height = height;
    ctx->multisamples = multisamples;
    ctx->fullscreen = (fullscreen == ELF_FALSE) ? ELF_FALSE : ELF_TRUE;
    ctx->title = elfCreateString(title);

#ifndef ELF_MACOSX
    glfwInit();
#endif

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, multisamples);

    if (!glfwOpenWindow(width, height, 8, 8, 8, 0, 24, 0, (fullscreen == ELF_FALSE) ? GLFW_WINDOW : GLFW_FULLSCREEN))
    {
        elfDecRef((elfObject*)ctx);
        return ELF_FALSE;
    }

    glfwGetWindowSize(&ctx->width, &ctx->height);

    glfwEnable(GLFW_MOUSE_CURSOR);

    glfwSetWindowTitle(title);
    glfwPollEvents();
    glfwSwapInterval(0);

    glfwSetMouseButtonCallback(mouseButtonCallback);
    glfwSetMousePosCallback(mousePositionCallback);
    glfwSetMouseWheelCallback(mouseWheelCallback);
    glfwSetKeyCallback(keyCallback);
    glfwSetCharCallback(charCallback);

    vidmodes = (GLFWvidmode*)malloc(sizeof(GLFWvidmode) * 256);
    videoModeCount = glfwGetVideoModes(vidmodes, 256);

    for (i = 0; i < videoModeCount; i++)
    {
        if (vidmodes[i].RedBits == 8 && vidmodes[i].GreenBits == 8 && vidmodes[i].BlueBits == 8)
        {
            videoMode = elfCreateVideoMode();
            videoMode->reso.x = vidmodes[i].Width;
            videoMode->reso.y = vidmodes[i].Height;
            elfAppendListObject(ctx->videoModes, (elfObject*)videoMode);
        }
    }

    // dunno what I'm doing here, just wondering if the bit counts aren't
    // 8 for some reason...
    if (videoModeCount > 0 && elfGetListLength(ctx->videoModes) == 0)
    {
        for (i = 0; i < videoModeCount; i++)
        {
            videoMode = elfCreateVideoMode();
            videoMode->reso.x = vidmodes[i].Width;
            videoMode->reso.y = vidmodes[i].Height;
            elfAppendListObject(ctx->videoModes, (elfObject*)videoMode);
        }
    }

    free(vidmodes);

    return ELF_TRUE;
}

void elfDeinitContext()
{
    if (!ctx)
        return;

    glfwTerminate();

    elfDecRef((elfObject*)ctx);
    ctx = NULL;
}

unsigned char elfResizeContext(int width, int height)
{
    if (width <= 0 || height <= 0 || (width == ctx->width && height == ctx->height))
        return ELF_FALSE;

    glfwSetWindowSize(width, height);
    ctx->width = width;
    ctx->height = height;

    return ELF_FALSE;
}

void elfSetTitle(const char* title)
{
    if (!title || !strlen(title))
        return;

    if (ctx->title)
        elfDestroyString(ctx->title);

    ctx->title = elfCreateString(title);

    glfwSetWindowTitle(title);
    glfwPollEvents();
}

int elfGetWindowWidth() { return ctx->width; }

int elfGetWindowHeight() { return ctx->height; }

int elfGetVideoModeCount() { return elfGetListLength(ctx->videoModes); }

elfVec2i elfGetVideoMode(int idx)
{
    elfVideoMode* mode;
    elfVec2i reso;

    memset(&reso.x, 0x0, sizeof(elfVec2i));

    mode = (elfVideoMode*)elfGetListObject(ctx->videoModes, idx);

    if (mode)
    {
        return mode->reso;
    }

    return reso;
}

int elfGetMultisamples() { return ctx->multisamples; }

unsigned char elfIsFullscreen() { return ctx->fullscreen; }

const char* elfGetTitle() { return ctx->title; }

double elfGetTime() { return glfwGetTime(); }

void elfSleep(float time) { glfwSleep(time); }

unsigned char elfIsWindowOpened() { return glfwGetWindowParam(GLFW_OPENED); }

void elfSwapBuffers()
{
    int i;

    elfDestroyList(ctx->events);
    ctx->events = elfCreateList();

    memcpy(ctx->prvMbuts, ctx->curMbuts, sizeof(unsigned char) * 3);
    memcpy(ctx->prvKeys, ctx->curKeys, sizeof(unsigned char) * 256);
    memcpy(ctx->prvMousePosition, ctx->mousePosition, sizeof(int) * 2);

    for (i = 0; i < 16; i++)
    {
        if (ctx->joysticks[i].present)
        {
            memcpy(ctx->joysticks[i].prvButs, ctx->joysticks[i].curButs, sizeof(unsigned char) * 16);
        }
    }

    glfwSwapBuffers();

    for (i = 0; i < 16; i++)
    {
        ctx->joysticks[i].present = glfwGetJoystickParam(GLFW_JOYSTICK_1 + i, GLFW_PRESENT) == GL_TRUE;
        if (ctx->joysticks[i].present)
        {
            glfwGetJoystickPos(GLFW_JOYSTICK_1 + i, ctx->joysticks[i].axisPos, 2);
            glfwGetJoystickButtons(GLFW_JOYSTICK_1 + i, ctx->joysticks[i].curButs, 16);
        }
    }
}

void mouseButtonCallback(int button, int state)
{
    unsigned int elfButton;

    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        elfButton = ELF_BUTTON_LEFT;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        elfButton = ELF_BUTTON_MIDDLE;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        elfButton = ELF_BUTTON_RIGHT;
        break;
    default:
        return;
    }

    ctx->curMbuts[elfButton] = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;
}

void mousePositionCallback(int x, int y)
{
    ctx->mousePosition[0] = x;
    ctx->mousePosition[1] = y;
}

void mouseWheelCallback(int wheel) { ctx->mouseWheel = wheel; }

void keyCallback(int key, int state)
{
    unsigned int elfKey;
    elfKeyEvent* keyEvent;

    if (key >= 32 && key < 127)
    {
        elfKey = key;
    }
    else
    {
        switch (key)
        {
        case GLFW_KEY_ESC:
            elfKey = ELF_KEY_ESC;
            break;
        case GLFW_KEY_F1:
            elfKey = ELF_KEY_F1;
            break;
        case GLFW_KEY_F2:
            elfKey = ELF_KEY_F2;
            break;
        case GLFW_KEY_F3:
            elfKey = ELF_KEY_F3;
            break;
        case GLFW_KEY_F4:
            elfKey = ELF_KEY_F4;
            break;
        case GLFW_KEY_F5:
            elfKey = ELF_KEY_F5;
            break;
        case GLFW_KEY_F6:
            elfKey = ELF_KEY_F6;
            break;
        case GLFW_KEY_F7:
            elfKey = ELF_KEY_F7;
            break;
        case GLFW_KEY_F8:
            elfKey = ELF_KEY_F8;
            break;
        case GLFW_KEY_F9:
            elfKey = ELF_KEY_F9;
            break;
        case GLFW_KEY_F10:
            elfKey = ELF_KEY_F10;
            break;
        case GLFW_KEY_F11:
            elfKey = ELF_KEY_F11;
            break;
        case GLFW_KEY_F12:
            elfKey = ELF_KEY_F12;
            break;
        case GLFW_KEY_UP:
            elfKey = ELF_KEY_UP;
            break;
        case GLFW_KEY_DOWN:
            elfKey = ELF_KEY_DOWN;
            break;
        case GLFW_KEY_LEFT:
            elfKey = ELF_KEY_LEFT;
            break;
        case GLFW_KEY_RIGHT:
            elfKey = ELF_KEY_RIGHT;
            break;
        case GLFW_KEY_LSHIFT:
            elfKey = ELF_KEY_LSHIFT;
            break;
        case GLFW_KEY_RSHIFT:
            elfKey = ELF_KEY_RSHIFT;
            break;
        case GLFW_KEY_LCTRL:
            elfKey = ELF_KEY_LCTRL;
            break;
        case GLFW_KEY_RCTRL:
            elfKey = ELF_KEY_RCTRL;
            break;
        case GLFW_KEY_LALT:
            elfKey = ELF_KEY_LALT;
            break;
        case GLFW_KEY_RALT:
            elfKey = ELF_KEY_RALT;
            break;
        case GLFW_KEY_TAB:
            elfKey = ELF_KEY_TAB;
            break;
        case GLFW_KEY_ENTER:
            elfKey = ELF_KEY_ENTER;
            break;
        case GLFW_KEY_BACKSPACE:
            elfKey = ELF_KEY_BACKSPACE;
            break;
        case GLFW_KEY_INSERT:
            elfKey = ELF_KEY_INSERT;
            break;
        case GLFW_KEY_DEL:
            elfKey = ELF_KEY_DEL;
            break;
        case GLFW_KEY_PAGEUP:
            elfKey = ELF_KEY_PAGEUP;
            break;
        case GLFW_KEY_PAGEDOWN:
            elfKey = ELF_KEY_PAGEDOWN;
            break;
        case GLFW_KEY_HOME:
            elfKey = ELF_KEY_HOME;
            break;
        case GLFW_KEY_END:
            elfKey = ELF_KEY_END;
            break;
        case GLFW_KEY_KP_0:
            elfKey = ELF_KEY_KP_0;
            break;
        case GLFW_KEY_KP_1:
            elfKey = ELF_KEY_KP_1;
            break;
        case GLFW_KEY_KP_2:
            elfKey = ELF_KEY_KP_2;
            break;
        case GLFW_KEY_KP_3:
            elfKey = ELF_KEY_KP_3;
            break;
        case GLFW_KEY_KP_4:
            elfKey = ELF_KEY_KP_4;
            break;
        case GLFW_KEY_KP_5:
            elfKey = ELF_KEY_KP_5;
            break;
        case GLFW_KEY_KP_6:
            elfKey = ELF_KEY_KP_6;
            break;
        case GLFW_KEY_KP_7:
            elfKey = ELF_KEY_KP_7;
            break;
        case GLFW_KEY_KP_8:
            elfKey = ELF_KEY_KP_8;
            break;
        case GLFW_KEY_KP_9:
            elfKey = ELF_KEY_KP_9;
            break;
        case GLFW_KEY_KP_DIVIDE:
            elfKey = ELF_KEY_KP_DIVIDE;
            break;
        case GLFW_KEY_KP_MULTIPLY:
            elfKey = ELF_KEY_KP_MULTIPLY;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            elfKey = ELF_KEY_KP_SUBTRACT;
            break;
        case GLFW_KEY_KP_ADD:
            elfKey = ELF_KEY_KP_ADD;
            break;
        case GLFW_KEY_KP_DECIMAL:
            elfKey = ELF_KEY_KP_DECIMAL;
            break;
        case GLFW_KEY_KP_EQUAL:
            elfKey = ELF_KEY_KP_EQUAL;
            break;
        case GLFW_KEY_KP_ENTER:
            elfKey = ELF_KEY_KP_ENTER;
            break;
        case GLFW_KEY_SPACE:
            elfKey = ELF_KEY_SPACE;
            break;
        default:
            return;
        }
    }

    ctx->curKeys[elfKey] = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

    keyEvent = elfCreateKeyEvent();
    keyEvent->key = elfKey;
    keyEvent->state = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

    elfAppendListObject(ctx->events, (elfObject*)keyEvent);
}

void charCallback(int code, int state)
{
    elfCharEvent* charEvent;

    charEvent = elfCreateCharEvent();
    charEvent->code = code;
    charEvent->state = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

    elfAppendListObject(ctx->events, (elfObject*)charEvent);
}

elfVec2i elfGetMousePosition()
{
    elfVec2i pos;

    pos.x = ctx->mousePosition[0];
    pos.y = ctx->mousePosition[1];

    return pos;
}

elfVec2i elfGetMouseForce()
{
    elfVec2i force;

    force.x = ctx->mousePosition[0] - ctx->prvMousePosition[0];
    force.y = ctx->mousePosition[1] - ctx->prvMousePosition[1];

    return force;
}

void elfSetMousePosition(int x, int y)
{
    glfwSetMousePos(x, y);

    ctx->mousePosition[0] = x;
    ctx->mousePosition[1] = y;

    if (ctx->mousePosition[0] < 0)
        ctx->mousePosition[0] = 0;
    else if (ctx->mousePosition[0] > elfGetWindowWidth())
        ctx->mousePosition[0] = elfGetWindowWidth();
    if (ctx->mousePosition[1] < 0)
        ctx->mousePosition[1] = 0;
    else if (ctx->mousePosition[1] > elfGetWindowWidth())
        ctx->mousePosition[1] = elfGetWindowHeight();
}

void elfHideMouse(unsigned char hide)
{
    if (hide)
    {
        glfwDisable(GLFW_MOUSE_CURSOR);
        ctx->hideMouse = ELF_TRUE;
    }
    else
    {
        glfwEnable(GLFW_MOUSE_CURSOR);
        ctx->hideMouse = ELF_FALSE;
    }
}

unsigned char elfIsMouseHidden() { return ctx->hideMouse; }

int elfGetMouseWheel() { return ctx->mouseWheel; }

int elfGetMouseButtonState(int button)
{
    if (button < 0 || button > 2)
        return 0;
    if (ctx->prvMbuts[button])
    {
        if (ctx->curMbuts[button])
            return ELF_DOWN;
        else
            return ELF_RELEASED;
    }
    else
    {
        if (ctx->curMbuts[button])
            return ELF_PRESSED;
        else
            return ELF_UP;
    }
}

int elfGetKeyState(int key)
{
    if (key < 0 || key > 256)
        return 0;
    if (ctx->prvKeys[key])
    {
        if (ctx->curKeys[key])
            return ELF_DOWN;
        else
            return ELF_RELEASED;
    }
    else
    {
        if (ctx->curKeys[key])
            return ELF_PRESSED;
        else
            return ELF_UP;
    }
}

unsigned char elfGetJoystickPresent(int joy)
{
    if (joy < 0 || joy > 15)
        return ELF_FALSE;
    return ctx->joysticks[joy].present;
}

elfVec2f elfGetJoystickAxis(int joy)
{
    elfVec2f pos;
    memset(&pos, 0x0, sizeof(elfVec2f));
    if (joy < 0 || joy > 15)
        return pos;
    pos.x = ctx->joysticks[joy].axisPos[0];
    pos.y = ctx->joysticks[joy].axisPos[1];
    return pos;
}

int elfGetJoystickButtonState(int joy, int but)
{
    if (joy < 0 || joy > 15)
        return ELF_UP;
    if (but < 0 || but > 15)
        return ELF_UP;
    if (ctx->joysticks[joy].prvButs[but])
    {
        if (ctx->joysticks[joy].curButs[but])
            return ELF_DOWN;
        else
            return ELF_RELEASED;
    }
    else
    {
        if (ctx->joysticks[joy].curButs[but])
            return ELF_PRESSED;
        else
            return ELF_UP;
    }
}

int elfGetEventCount() { return elfGetListLength(ctx->events); }

elfObject* elfGetEvent(int idx)
{
    elfObject* obj;
    int i;

    if (idx < 0 || idx > elfGetListLength(ctx->events) - 1)
        return NULL;

    for (i = 0, obj = elfBeginList(ctx->events); obj; obj = elfGetListNext(ctx->events), i++)
    {
        if (i == idx)
            return obj;
    }

    return NULL;
}

int elfGetKeyEventKey(elfKeyEvent* keyEvent) { return keyEvent->key; }

int elfGetKeyEventState(elfKeyEvent* keyEvent) { return keyEvent->state; }
