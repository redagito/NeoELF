#pragma once

#include "nelf/Vector.h"
#include "nelf/objectHeader.h"

struct elfObject;
struct elfList;

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
    int key;
    unsigned char state;
};

elfKeyEvent* elfCreateKeyEvent();
void elfDestroyKeyEvent(void* data);

struct elfCharEvent
{
    ELF_OBJECT_HEADER;
    int code;
    unsigned char state;
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

    unsigned char curKeys[256];
    unsigned char prvKeys[256];

    // TODO Why no vec2?
    int mousePosition[2];
    int prvMousePosition[2];
    bool hideMouse = false;
    int mouseWheel = 0;

    unsigned char curMbuts[3];
    unsigned char prvMbuts[3];

    elfJoystick joysticks[16];
    elfList* events = nullptr;
};

// Global context
extern elfContext* ctx;

elfContext* elfCreateContext();
void elfDestroyContext(void* data);

// From global context

unsigned char elfInitContext(int width, int height, const char* title, int multisamples, unsigned char fullscreen);

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

unsigned char elfIsWindowOpened();

void elfSwapBuffers();

// Callbacks

void mouseButtonCallback(int button, int state);

void mousePositionCallback(int x, int y);

void mouseWheelCallback(int wheel);

// Called on keypress
void keyCallback(int key, int state);

// Char event
void charCallback(int code, int state);

// Mouse

elfVec2i elfGetMousePosition();

elfVec2i elfGetMouseForce();

void elfSetMousePosition(int x, int y);

void elfHideMouse(unsigned char hide);

unsigned char elfIsMouseHidden();

int elfGetMouseWheel();

int elfGetMouseButtonState(int button);

// All curren key states
int elfGetKeyState(int key);

// Joystick

unsigned char elfGetJoystickPresent(int joy);

elfVec2f elfGetJoystickAxis(int joy);

int elfGetJoystickButtonState(int joy, int but);

// Events

int elfGetEventCount();

elfObject* elfGetEvent(int idx);

int elfGetKeyEventKey(elfKeyEvent* keyEvent);

int elfGetKeyEventState(elfKeyEvent* keyEvent);