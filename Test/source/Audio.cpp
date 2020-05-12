#include <nelf/audio/AudioDevice.h>

#include <catch2/catch.hpp>

TEST_CASE("Play sound", "[audio]") { REQUIRE(elfInitAudio()); }