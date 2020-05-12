#define CATCH_CONFIG_MAIN
#include <nelf/pak/Pak.h>

#include <catch2/catch.hpp>

TEST_CASE("Load demo 1 pak", "[pak]")
{
    elfPak* pak = elfCreatePakFromFile("data/demo1.pak");

    REQUIRE(pak != nullptr);

    elfDestroyPak(pak);
}