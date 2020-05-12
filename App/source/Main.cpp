#include <nelf/Config.h>
#include <nelf/Engine.h>
#include <nelf/Log.h>
#include <nelf/Object.h>
#include <nelf/Scripting.h>
#include <nelf/errorCode.h>
#include <nelf/resource/Script.h>

int main(int argc, char** argv)
{
    // TODO Command line processing
    // Try load from file
    elfConfig* config = elfReadConfig("config.txt");
    if (config == nullptr)
    {
        // Use defaults
        config = elfCreateConfig();
    }

    // Runtime init, creates globals and engine
    if (!elfInit(config))
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: can't initialize engine\n");
        elfDestroyConfig(config);
        return -1;
    }

    // Load startup script
    // TODO Should probably be changeable in config
    elfScript* script = elfCreateScriptFromFile("Init", "init.lua");

    // Either execute script or just start engine
    // TODO Its a very BAD idea to have the script run the main loop
    //      This will impact performance severely, better have callbacks on frame etc..
    //      Needs to be changed!
    if (script != nullptr)
    {
        // TODO Why manually manage references?
        //      Should these not be set by elfCreateScriptFromFile?
        elfIncRef((elfObject*)script);
        elfRunScript(script);
        elfDecRef((elfObject*)script);
    }
    else
    {
        // Engine execution
        bool run = true;
        while (run) run = elfRun();
    }

    // Cleanup
    elfDeinit();

    return 0;
}
