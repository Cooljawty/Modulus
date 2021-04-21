#include <iostream>
#include <string>
#include <vector>

#include "modulus.h"

#define FULLSCREEN false

using namespace Modulus;

GameManager::GameManager(){

	mWindow = nullptr;

	mContext = nullptr;

	mScreenWidth = 1366 * 0.8;
	mScreenHeight = 768 * 0.8;

	isRunning = true;

}

GameManager::~GameManager(){
	close();
}

bool GameManager::init(){

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
		std::cout << "SDL cannot initilize. SDL_Error: "
		     << SDL_GetError() << std::endl;

		success = false;
 	}
	 else{
		//Set OpenGL version to
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		//Create window
		mWindow = SDL_CreateWindow("Modulus",
									SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									mScreenWidth, mScreenHeight,
									SDL_WINDOW_OPENGL | (FULLSCREEN ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN));
		if(mWindow == NULL){
			std::cout << "Window could not be created. SDL_Error: "
				 << SDL_GetError() << std::endl;

			success = false;
		}
 		else{

			//Get window size
			SDL_DisplayMode display;
			SDL_GetDesktopDisplayMode(0, &display);
			if(FULLSCREEN){
				mScreenWidth = display.w;
				mScreenHeight = display.h;
			}
			SDL_SetRelativeMouseMode(SDL_TRUE);

			//Create opengl context
			mContext = SDL_GL_CreateContext(mWindow);
			if(mContext == NULL){
				std::cout << "OpenGL context could not be created. SDL_Error: "
					 << SDL_GetError() << std::endl;

				success = false;
			}
			else{

				//Set context as current
				SDL_GL_MakeCurrent(mWindow, mContext);

				//Initilizing GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if(glewError != GLEW_OK){
					std::cout << "GLEW could not be initilized. "
						 << glewGetErrorString(glewError) << std::endl;
					success = false;
				}
				else{
					//Use VSync
					if(SDL_GL_SetSwapInterval(1) < 0){
						std::cout << "Unable to set VSync. SDL_Error: "
							 << SDL_GetError() << std::endl;

					}
 				}
 			}
		}
  	}

	isRunning = success;

	return success;
 }

bool GameManager::initOGL(){
	
	#ifndef NDEBUG
	glEnable(GL_DEBUG_OUTPUT);
	#endif

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_MULTISAMPLE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Check for errors
	GLenum error = glGetError();
	 if(error !=  GL_NO_ERROR){
		std::cout << "OpenGL can not initilize. " << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

void GameManager::pollEvents(){

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	while(SDL_PollEvent(&mEvents) != 0){
		switch(mEvents.type){
			//Quit event
			case SDL_QUIT:
				isRunning = false;
				break;
			//Update inputs
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mouseCursor.update(mEvents.button.x, mEvents.button.y, mEvents.button.state, mEvents.button.state);
				break;
			case SDL_MOUSEMOTION:
				mouseCursor.update(mEvents.motion.xrel, mEvents.motion.yrel, mEvents.motion.state);
				break;
			case SDL_TEXTINPUT:
				std::cout << mEvents.text.text << std::flush;
				//testText.append(e.text.text);
				break;
			case SDL_TEXTEDITING:
				//compositionText = e.edit.text;
				//gTextCursor = e.edit.start;
				//selection_len = e.edit.length;
				break;
			default:
				break;
		}
	}

	for(auto b : Button::gButtons){
		b->update(keystate);
	}
}

void GameManager::close(){

	//Delete Opengl context
	SDL_GL_DeleteContext(mContext);

	//Destroy window
	SDL_DestroyWindow(mWindow);

	mWindow = nullptr;
	mContext = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
