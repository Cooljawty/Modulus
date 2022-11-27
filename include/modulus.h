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
#include <tuple>

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
			
			//Text input strings
			std::string getInputText(){ return mInputText; }
			std::string getCompositionText(){ return mCompositionText; }

			bool getRunning(){return isRunning;}
			void toggleRunning(){isRunning = !isRunning;}
			
		public:
			//Mouse cursor
			Cursor mouseCursor;
		
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
		
		//Rendering queue
		public:
			void drawMesh(FrameBuffer&, Shader&, Mesh&);
			
			void drawQueue();
			//TODO:	
			//Adds mesh to mMeshes, and to MxS
			void addMesh(Mesh&);
			template <typename T>
			void addMeshes(T& meshes){
				for( auto m: meshes ){
					addMesh(*m);
				}
			}
			//Adds mesh to mShaders, and to MxS and FxS
			void addShader(Shader&); //Warning: Time Expensive
			//Adds mesh to mFrameBuffers, and FxS
			void addFrameBuffer(FrameBuffer&);
			
			void bindMeshtoShader(Mesh& m, Shader& s){
				MxS[&m][&s] = true;
			}
			void unbindMeshtoShader(Mesh& m, Shader& s){
				MxS[&m][&s] = false;
			}
			void bindFrameBuffertoShader(FrameBuffer& f, Shader& s){
				FxS[&f][&s] = true;
			}
			void unbindFrameBuffertoShader(FrameBuffer& f, Shader& s){
				FxS[&f][&s] = false;
			}
		private:
			//Queue of meshes to render with it target framebuffer, shader
			std::vector< std::tuple< FrameBuffer*, Shader*, Mesh*> > mRenderQueue;
			
			std::vector<Shader*> mShaders;
			std::vector<FrameBuffer*> mFrameBuffers;
			std::vector<Mesh*> mMeshes;
		public:	
			//TODO: Rename
			std::map<FrameBuffer*, std::map<Shader*, bool>> FxS;
			std::map<Mesh*, std::map<Shader*, bool>> MxS;

		public: //DEBUG	
			//Console input	
			std::string mInputText;
			std::string mCompositionText;
			unsigned int mTextCursor;
			unsigned int mSelectionLength;	
	};
}
