#pragma once

#include <SDL_GL.h>
#include "data_types.h"

#include "texture.h"
#include "VertexArray.h"
#include "mesh.h"
#include "model.h"

#include "input.h"
#include "transform.h"
#include "modulus_time.h"
#include "physics.h"

#include "TextureShader.h"
#include "SpriteShader.h"
#include "PolygonShader2D.h"
#include "PolygonShader.h"
#include "LampShader.h"
#include "FBOShader.h"
#include "TextShader.h"


namespace Modulus{

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
}
