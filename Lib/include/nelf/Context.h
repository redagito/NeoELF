#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfObject;
struct elfList;
struct GLFWwindow;

struct elfVideoMode
{
    ELF_OBJECT_HEADER;
    elfVec2i reso;
};

elfVideoMode* elfCreateVideoMode();
void elfDestroyVideoMode(void* data);

struct elfKeyEvent
{
    ELF_OBJECT_HEADER;
    int key = 0;
    bool state = false;
};

elfKeyEvent* elfCreateKeyEvent();
void elfDestroyKeyEvent(void* data);

struct elfCharEvent
{
    ELF_OBJECT_HEADER;
    int code = 0;
    bool state = false;
};

elfCharEvent* elfCreateCharEvent();
void elfDestroyCharEvent(void* data);

struct elfJoystick
{
    bool present = false;
    float axisPos[2];
    unsigned char curButs[16];
    unsigned char prvButs[16];
};

struct elfContext
{
    ELF_OBJECT_HEADER;
    int width = 0;
    int height = 0;
    bool fullscreen = false;
    char* title = nullptr;
    int multisamples = 0;
    elfList* videoModes = nullptr;

    bool curKeys[256];
    unsigned char prvKeys[256];

    // TODO Why no vec2?
    int mousePosition[2];
    int prvMousePosition[2];
    bool hideMouse = false;
    int mouseWheel = 0;

    bool curMbuts[3];
    unsigned char prvMbuts[3];

    elfJoystick joysticks[16];
    elfList* events = nullptr;

    // App window
    GLFWwindow* window = nullptr;
};

// Global context
extern elfContext* ctx;

elfContext* elfCreateContext();
void elfDestroyContext(void* data);

// From global context

bool elfInitContext(int width, int height, const char* title, int multisamples, bool fullscreen);
void elfDeinitContext();

unsigned char elfResizeContext(int width, int height);

void elfSetTitle(const char* title);

int elfGetWindowWidth();
int elfGetWindowHeight();
int elfGetVideoModeCount();

// From GLFW

elfVec2i elfGetVideoMode(int idx);

// From global context

int elfGetMultisamples();
unsigned char elfIsFullscreen();
const char* elfGetTitle();

// GLFW Util, replace with std functions

double elfGetTime();

void elfSleep(float time);
bool elfIsWindowOpened();
void elfSwapBuffers();

// Callbacks

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mods);
void mousePositionCallback(GLFWwindow* window, double x, double y);
void mouseWheelCallback(GLFWwindow* window, double xOffset, double yOffset);

// Called on keypress
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Char event
void charCallback(GLFWwindow* window, unsigned int code);

// Mouse

elfVec2i elfGetMousePosition();
elfVec2i elfGetMouseForce();

void elfSetMousePosition(int x, int y);

void elfHideMouse(bool hide);
bool elfIsMouseHidden();

int elfGetMouseWheel();
int elfGetMouseButtonState(int button);

// All curren key states
int elfGetKeyState(int key);

// Joystick

bool elfGetJoystickPresent(int joy);
elfVec2f elfGetJoystickAxis(int joy);
int elfGetJoystickButtonState(int joy, int but);

// Events

int elfGetEventCount();
elfObject* elfGetEvent(int idx);
int elfGetKeyEventKey(elfKeyEvent* keyEvent);
int elfGetKeyEventState(elfKeyEvent* keyEvent);