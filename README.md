![Screen 3](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine3b.jpg)
![Screen 1](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine1b.jpg)
![Screen 2](https://github.com/redagito/NeoELF/blob/master/doc/images/blendelf_engine2b.jpg)

# NeoELF

A BlendELF based 3d game engine.

Current project goals are:
* Cleaner project structure
* Refactoring (mostly separating definition and implenetation into separate files)
* Compile as C++ code
* CMake / Conan build system
* Unit tests
* Separate project into lib, app and tests
* Performance benchmarks
* Move to OOP based design
* Replace custom implementations of common functionality with std C++ counterparts (file system, string)
* Changes must show same or better performance
* Demos / games

# BlendELF

The version of the BlendELF source code used as basis for development was taken from https://github.com/cartuchogl/BlendELF (capi branch).
The original version of the engine can be found at https://github.com/jesterKing/BlendELF.

From the original Readme (slightly edited the formatting):

BlendELF is a work in progress, OpenGL graphics powered, compact open source 3d game engine aimed towards independent game developers for quick prototyping and game/virtual environment creation. 
It supports a standard set of features, like skeletal animation, post processing, windows and linux support, LUA/C programming, physics powered by Bullet Physics, GUI and 2D/3D Audio. 
BlendELF binaries and code are provided free of charge without restrictions.

## Authors

Samuel Anjam, aka centralnoise - Lead coder

## Current Contributors

Michael Gale, aka mbg - Networking

## Contributors

Michael Gale, aka mbg - .NET wraps for versions beta 1 - beta 3
Carlos Bolaños, aka cartuchogl - Porting to Mac OSX, bug fixing