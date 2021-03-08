//Default incudes for sdl and opengl libraries
#pragma once

//Windows build
#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include <GL/glew.h>
#define NO_SDL_GLEXT //SDL_opengl conflicts w/ glew.h without definition
#include <SDL_opengl.h>

//Default build
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#define NO_SDL_GLEXT //SDL_opengl conflicts w/ glew.h without definition
#include <SDL/SDL_opengl.h>
#endif

//Path variables set by CMake
#ifndef ASSET_PATH
#define ASSET_PATH ""
#endif

#ifndef SHADER_PATH
#define SHADER_PATH ""
#endif
