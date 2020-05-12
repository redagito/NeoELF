#include "nelf/FramePlayer.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Engine.h"
#include "nelf/General.h"
#include "nelf/objectType.h"

elfFramePlayer* elfCreateFramePlayer()
{
    elfFramePlayer* player;

    player = (elfFramePlayer*)malloc(sizeof(elfFramePlayer));
    memset(player, 0x0, sizeof(elfFramePlayer));
    player->objType = ELF_FRAME_PLAYER;
    player->objDestr = elfDestroyFramePlayer;

    player->curFrame = 1.0f;

    elfIncObj(ELF_FRAME_PLAYER);

    return player;
}

void elfDestroyFramePlayer(void* data)
{
    elfFramePlayer* player = (elfFramePlayer*)data;

    free(player);

    elfDecObj(ELF_FRAME_PLAYER);
}

void elfUpdateFramePlayer(elfFramePlayer* player)
{
    if (player->end > 1.0f && !player->pause)
    {
        player->curFrame += player->speed * elfGetSync();
        if (player->loop)
        {
            if (player->curFrame > player->end + 1)
                elfSetFramePlayerFrame(player, player->start);
            else
                elfSetFramePlayerFrame(player, player->curFrame);
        }
        else
        {
            if (player->curFrame > player->end)
            {
                elfSetFramePlayerFrame(player, player->curFrame);
                elfStopFramePlayer(player);
            }
            else
                elfSetFramePlayerFrame(player, player->curFrame);
        }
    }
}

void elfSetFramePlayerFrame(elfFramePlayer* player, float frame)
{
    player->curFrame = frame;
    if (player->curFrame < 1.0f)
        player->curFrame = 1.0f;

    if (elfIsFramePlayerPlaying(player) || elfIsFramePlayerPaused(player))
    {
        if (player->curFrame < player->start)
            player->curFrame = player->start;
        if (player->loop)
        {
            if (player->curFrame > player->end + 1)
                player->curFrame = player->start;
        }
        else
        {
            if (player->curFrame > player->end)
                player->curFrame = player->end;
        }
    }

    if (player->callback)
        player->callback(player);
}

void elfPlayFramePlayer(elfFramePlayer* player, float start, float end, float speed)
{
    player->start = start;
    player->end = end;
    if (player->start < 1.0f)
        player->start = 1.0f;
    if (player->end < player->start)
        player->end = player->start;
    player->curFrame = player->start;
    player->speed = speed;
    player->loop = false;
    player->pause = false;

    elfSetFramePlayerFrame(player, player->curFrame);
}

void elfLoopFramePlayer(elfFramePlayer* player, float start, float end, float speed)
{
    player->start = start;
    player->end = end;
    player->curFrame = start;
    player->speed = speed;
    player->pause = false;
    player->loop = true;

    elfSetFramePlayerFrame(player, player->curFrame);
}

void elfStopFramePlayer(elfFramePlayer* player)
{
    player->start = 0.0f;
    player->end = 0.0f;
    player->speed = 0.0f;
    player->pause = false;
    player->loop = false;
}

void elfPauseFramePlayer(elfFramePlayer* player)
{
    if (player->end > 0.99999f)
        player->pause = true;
}

void elfResumeFramePlayer(elfFramePlayer* player)
{
    if (player->end > 0.99999f)
        player->pause = false;
}

float elfGetFramePlayerStart(elfFramePlayer* player) { return player->start; }

float elfGetFramePlayerEnd(elfFramePlayer* player) { return player->end; }

float elfGetFramePlayerSpeed(elfFramePlayer* player) { return player->speed; }

float elfGetFramePlayerFrame(elfFramePlayer* player) { return player->curFrame; }

bool elfIsFramePlayerPlaying(elfFramePlayer* player)
{
    if (player->end > 0.99999f && !player->pause)
        return true;
    return false;
}

bool elfIsFramePlayerPaused(elfFramePlayer* player)
{
    if (player->pause)
        return true;
    return false;
}

void elfSetFramePlayerUserData(elfFramePlayer* player, void* userData) { player->userData = userData; }

void* elfGetFramePlayerUserData(elfFramePlayer* player) { return player->userData; }

void elfSetFramePlayerCallback(elfFramePlayer* player, void (*callback)(elfFramePlayer*))
{
    player->callback = callback;
}
