#pragma once

<<<<<<< Updated upstream
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#define NO_SDL_GLEXT //SDL_opengl conflicts w/ glew.h without definition
#include <SDL/SDL_opengl.h>
=======
#include <SDL_GL.h>
>>>>>>> Stashed changes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "TextureShader.h"
#include "TextShader.h"
#include "SpriteShader.h"
#include "PolygonShader2D.h"
#include "PolygonShader.h"
#include "LampShader.h"
#include "FBOShader.h"

#include "VertexArray.h"
#include "texture.h"
#include "input.h"
#include "transform.h"
#include "modulus_time.h"
#include "physics.h"

class GameManager{
	public:
		GameManager();
		~GameManager();

	public:
		//Initilize SDL window and OpenGL context
		bool init();

		//Initilizes Opengl matrixies and clear color
		bool initOGL();

		//Handle events in queue
		void pollEvents();

		//Frees OGL context and SDL window
		void close();

	public:
		SDL_Window* getWindow(){ return mWindow; }
		SDL_GLContext getContext(){ return mContext; }

		unsigned int getScreenWidth(){ return mScreenWidth; }
		unsigned int getScreenHeight(){ return mScreenHeight; }

		bool getRunning(){return isRunning;}
		void toggleRunning(){isRunning = !isRunning;}

	private:
		//Main Window
		SDL_Window* mWindow;

		//Opengl context
		SDL_GLContext mContext;

		//Handles SDL events
		SDL_Event mEvents;

	private:
		//Screen dimesions
		unsigned int mScreenWidth;
		unsigned int mScreenHeight;

		//Program running flag
		bool isRunning;

	public:
		//Mouse cursor
		Cursor mouseCursor;
};
