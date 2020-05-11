![Screen 3](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine3b.jpg)

# NeoELF

[![Build status](https://ci.appveyor.com/api/projects/status/faq9yhhcel6y2067/branch/master?svg=true)](https://ci.appveyor.com/project/redagito/neoelf/branch/master)

A cross-platform 3d game engine based on BlendELF, written in C++ with a modern build system.

## Project Goal

Current goals are:
* Clean project structure
* Better / more documentation
* Organize doc, move content from old website to wiki
* Refactoring (mostly separating definition and implementation into separate files)
* Compile as C++ code
* CMake / Conan build system
* Achieve feature parity with last release of BlendELF (make sure everything still works)
* Setup Appveyor for automatic builds
* Unit tests
* Separate project into lib, app and tests
* Performance benchmarks
* Move to OOP based design
* Replace custom implementations of common functionality with std C++ counterparts (file system, string)
* Changes must show same or better performance
* Demos / games

## Building

Development is currently done with
* CMake, 3.16.0, https://cmake.org
* Conan, 1.25.0, https://conan.io
* Visual Studio 2019 Community Edition, https://visualstudio.microsoft.com/vs/community

Conan remotes used are
* conan-center: https://conan.bintray.com
* bincrafters: https://api.bintray.com/conan/bincrafters/public-conan

Building the project should just require running CMake and the respective build command.

## Project Status

Active development, no official release yet.
The project is currently not in a usable state.

## Screenshots

These are from BlendELF, to be replaced once everything works

![Screen 1](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine1b.jpg)
![Screen 2](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine2b.jpg)
![Screen 4](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine4b.jpg)

# BlendELF

The version of the BlendELF source code used as basis for development was taken from https://github.com/cartuchogl/BlendELF (capi branch).
The original version of the engine can be found at https://github.com/jesterKing/BlendELF.

![Banner](https://github.com/redagito/NeoELF/blob/master/doc/images/topbanner.jpg)

Last valid state of the original webiste can be found at http://web.archive.org/web/20121014085548/http://blendelf.com/

## Original Readme

```
BlendELF is a work in progress, OpenGL graphics powered, compact
open source 3d game engine aimed towards independent game developers
for quick prototyping and game/virtual environment creation. It
supports a standard set of features, like skeletal animation, post
processing, windows and linux support, LUA/C programming, physics
powered by Bullet Physics, GUI and 2D/3D Audio. BlendELF binaries
and code are provided free of charge without restrictions. 

--------------------------------
----------- Authors ------------
--------------------------------

Samuel Anjam, aka centralnoise - Lead coder

--------------------------------
----- Current Contributors -----
--------------------------------

Michael Gale, aka mbg - Networking

--------------------------------
--------- Contributors ---------
--------------------------------

Michael Gale, aka mbg - .NET wraps for versions beta 1 - beta 3
Carlos Bolaños, aka cartuchogl - Porting to Mac OSX, bug fixing
```
