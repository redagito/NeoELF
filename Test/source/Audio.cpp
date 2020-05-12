#include <nelf/audio/AudioDevice.h>
#include <nelf/audio/AudioSource.h>
#include <nelf/audio/Sound.h>

#include <catch2/catch.hpp>
#include <thread>

TEST_CASE("Play sound", "[audio]")
{
    REQUIRE(elfInitAudio());
    auto sound = elfLoadSound("data/soliloquy.ogg");
    REQUIRE(sound != nullptr);
    auto src = elfPlaySound(sound, 1.f);
    REQUIRE(src != nullptr);
    while (elfIsSoundPlaying(src))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        elfUpdateAudio();
    }
    elfDeinitAudio();
}