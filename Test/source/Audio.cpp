#include <nelf/Object.h>
#include <nelf/audio/AudioDevice.h>
#include <nelf/audio/AudioSource.h>
#include <nelf/audio/Sound.h>

#include <catch2/catch.hpp>
#include <thread>

TEST_CASE("Play sound", "[audio]")
{
    REQUIRE(elfInitAudio());
    auto sound = elfLoadSound("data/soliloquy.ogg");
    elfIncRef((elfObject*)sound);
    REQUIRE(sound != nullptr);

    auto src = elfPlaySound(sound, 1.f);
    elfIncRef((elfObject*)src);
    REQUIRE(src != nullptr);
    REQUIRE(elfIsSoundPlaying(src));

    // Play for 5 seconds = 5000 ms
    // 500 iterations, 10 ms wait
    for (int i = 0; i < 500; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        elfUpdateAudio();
    }

    elfStopSound(src);
    REQUIRE(!elfIsSoundPlaying(src));
    elfDecRef((elfObject*)src);
    elfDecRef((elfObject*)sound);

    elfDeinitAudio();
}