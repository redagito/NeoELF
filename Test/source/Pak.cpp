#include <nelf/Object.h>
#include <nelf/pak/Pak.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Load demo 1 pak", "[pak]")
{
    auto pak = elfCreatePakFromFile("data/balls.pak", true);
    elfIncRef((elfObject*)pak);
    REQUIRE(pak != nullptr);
    elfDecRef((elfObject*)pak);
}