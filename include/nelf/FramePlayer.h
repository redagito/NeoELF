#pragma once

#include "nelf/objectHeader.h"

struct elfFramePlayer
{
    ELF_OBJECT_HEADER;
    float start = 0.f;
    float end = 0.f;
    float curFrame = 0.f;
    float speed = 1.f;
    bool pause = true;
    bool loop = false;
    void* userData = nullptr;
    void (*callback)(elfFramePlayer*) = nullptr;
};

elfFramePlayer* elfCreateFramePlayer();

void elfDestroyFramePlayer(void* data);

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

bool elfIsFramePlayerPlaying(elfFramePlayer* player);

bool elfIsFramePlayerPaused(elfFramePlayer* player);

void elfSetFramePlayerUserData(elfFramePlayer* player, void* userData);

void* elfGetFramePlayerUserData(elfFramePlayer* player);

void elfSetFramePlayerCallback(elfFramePlayer* player, void (*callback)(elfFramePlayer*));