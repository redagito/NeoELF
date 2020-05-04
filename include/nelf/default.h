/*
        BlendELF
        ------------------------------------------
        File: 		default.h
        Author:		mbg
        Purpose:	Includes commonly used headers.
*/

#include <malloc.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef ELF_WINDOWS
// mbg: prevents windows.h from including winsock.h, this is later handled by enet
#define _WINSOCKAPI_
#include <windows.h>
#include <strsafe.h>
#endif

#include <GL/glfw.h>
#include <dirent.h>
#ifdef ELF_MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef ELF_MACOSX
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#else
#include <AL/alc.h>
#include <AL/al.h>
#endif

#include <FreeImage.h>
#include <ft2build.h>
#include <vorbis/vorbisfile.h>
#include FT_FREETYPE_H

#include <enet/enet.h>

#ifndef NO_ASSIMP
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/assimp.h>
#endif

/*
        End of File
*/
