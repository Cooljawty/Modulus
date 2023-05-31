#pragma once

#include "SDL_GL.h"
#include "data_types.h"

#include "texture.h"
#include "VertexArray.h"
#include "mesh.h"
#include "model.h"

#include "framebuffer.h"
#include "input.h"
#include "transform.h"
#include "modulus_time.h"
#include "physics.h"

#include "TextShader.h"
#include "text.h"
#include "TextureShader.h"
#include "SpriteShader.h"
#include "PolygonShader2D.h"
#include "PolygonShader.h"
#include "LampShader.h"
#include "FBOShader.h"
#include "LineShader.h"

#include <string>
#include <vector>

namespace Modulus{

	class GameManager{
		public:
			GameManager( bool fullscreen = false, unsigned int screenW = 100, unsigned int screenH = 100 );

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

			SDL_Window* getWindow(){ return mWindow; }

			unsigned int getScreenWidth(){ return mScreenWidth; }
			unsigned int getScreenHeight(){ return mScreenHeight; }
			
			SDL_GLContext getContext(){ return mContext; }

			//Rendering functions
			void draw(Shader&, VertArray&, std::vector<Material>, FrameBuffer&, GLenum drawMode = GL_TRIANGLES);
			void draw(Shader&, Mesh&, FrameBuffer&);
			//Copy from framebuffer to frame buffer
			void draw(Shader& s, FrameBuffer& srcFB, FrameBuffer& destFB);

			bool getRunning(){return isRunning;}
			void toggleRunning(){isRunning = !isRunning;}
			
			//Mouse cursor
			Cursor mouseCursor;
		
		private:
			//Main Window
			SDL_Window* mWindow;

			//Opengl context
			SDL_GLContext mContext;

			//Handles SDL events
			SDL_Event mEvents;
	
			//Screen dimesions
			unsigned int mScreenWidth;
			unsigned int mScreenHeight;

			bool isFullscreen;

			//Program running flag
			bool isRunning;
		

		public: //DEBUG	
			//Text input strings
			std::string getInputText(){ return mInputText; }
			std::string getCompositionText(){ return mCompositionText; }
			
			void clearInputText(){
				mInputText.clear();
				mCompositionText.clear();
				mTextCursor = 0;
				mSelectionLength = 0;
			}

			//Console input	
			std::string mInputText;
			std::string mCompositionText;
			unsigned int mTextCursor;
			unsigned int mSelectionLength;	
	};
}
