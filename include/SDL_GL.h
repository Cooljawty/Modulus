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

//GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

//Path variables set by CMake
#ifndef ASSET_PATH
#define ASSET_PATH ""
#endif

#ifndef SHADER_PATH
#define SHADER_PATH ""
#endif

