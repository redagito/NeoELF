
#include <GL/glfw.h>
#include <dirent.h>
#include <malloc.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifdef ELF_MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef ELF_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <FreeImage.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef NO_ASSIMP
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/assimp.h>
#endif

#ifdef USE_SERF
#include <apr.h>
#include <apr_atomic.h>
#include <apr_base64.h>
#include <apr_general.h>
#include <apr_getopt.h>
#include <apr_strings.h>
#include <apr_thread_proc.h>
#include <apr_uri.h>
#include <apr_version.h>

#include "serf.h"
#endif

#include "blendelf.h"
#include "gfx.h"
#include "types.h"

elfContext* ctx = NULL;
elfEngine* eng = NULL;
elfRenderStation* rnd = NULL;
elfResources* res = NULL;

#include "nelf/actor.h"
#include "nelf/armature.h"
#include "nelf/camera.h"
#include "nelf/config.h"
#include "nelf/context.h"
#include "nelf/engine.h"
#include "nelf/entity.h"
#include "nelf/font.h"
#include "nelf/frameplayer.h"
#include "nelf/general.h"
#include "nelf/gui.h"
#include "nelf/image.h"
#include "nelf/ipo.h"
#include "nelf/light.h"
#include "nelf/list.h"
#include "nelf/log.h"
#include "nelf/material.h"
#include "nelf/meshdata.h"
#include "nelf/model.h"
#include "nelf/network.h"
#include "nelf/pak.h"
#include "nelf/particles.h"
#include "nelf/postprocess.h"
#include "nelf/property.h"
#include "nelf/renderstation.h"
#include "nelf/resource.h"
#include "nelf/resources.h"
#include "nelf/scene.h"
#include "nelf/script.h"
#include "nelf/sprite.h"
#include "nelf/sst.h"
#include "nelf/str.h"
#include "nelf/texture.h"
#include "nelf/timer.h"

int main()
{
    elfConfig* config;
    elfScript* script;

// On bundle OSX need init glfw first for path are correct
#ifdef ELF_MACOSX
    glfwInit();
#endif

    if (!(config = elfReadConfig("config.txt")))
        config = elfCreateConfig();

    if (!elfInit(config))
    {
        elfSetError(ELF_CANT_INITIALIZE, "error: can't initialize engine\n");
        elfDestroyConfig(config);
        return -1;
    }

    script = elfCreateScriptFromFile("Init", "init.lua");
    if (script)
    {
        elfIncRef((elfObject*)script);
        elfRunScript(script);
        elfDecRef((elfObject*)script);
    }
    else
    {
        while (elfRun())
            ;
    }

    elfDeinit();

    return 0;
}
