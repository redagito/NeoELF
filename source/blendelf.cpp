
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

elfContext* ctx = NULL;
elfRenderStation* rnd = NULL;
