## Dungeon Crawler Engine  
A simple 3D game engine base written in C++. This project mainly aims to help with
the overly complicated tasks of setting up and using OpenGL, along with providing
many other useful tools.  

This engine will be the base for an upcoming multiplayer puzzle-platformer game.
However, that project is somewhat blocked until this engine gets to a
somewhat more workable state.  

### Features

#### Current
- GLFW-based window creation
- Event bus system for routing game and window events
- Resource loading system
  - Supports synchronous and (hopefully thread-safe) asynchronous file loading
  with a callback event
- Somewhat flexible logging system
- C++ OpenGL wrapper
  - Shader objects (WIP; cannot set uniforms)

#### Future
- OpenAL 3D audio
- OpenGL features
  - Full-featured camera object
  - Mesh rendering
  - Textures
  - Lighting engine
  - Split-screen multiplayer
- Voxel mesh optimization
- Physics/collision engine?
- Player input
  - Wii controller input (`libwiiuse`)
- **Cross-platform compatibility**

### Compiling
This project uses `cmake`, so compilation is fairly straightforward.
First, you need some dependencies:
- OpenGL
- GLFW 3
- Boost (filesystem) 1.48  or higher
- libdl (POSIX dynamic library loading; need cross-platform solution)

**NOTE**: This project will most likely NOT compile on Windows.
If you have a Windows machine and know how to fix this, it would be greatly
appreciated if you could provide some input.  

Once you have the required libraries, simply use the standard `cmake` incantation:
```
mkdir build
cd build
cmake ..
make
```  
Of course, if you have more than one CPU core, run `make` with more jobs, i.e.:
`make -j4` for four cores.

`make` will produce a binary called `dcengine`. If you run this and all goes well,
you should get a plain white window.
