#include <iostream>
#include <string>
#include <vector>

#include "modulus.h"

#define NDEBUG
#define FULLSCREEN false

using namespace Modulus;

GameManager::GameManager(){

	mWindow = nullptr;

	mContext = nullptr;

	mScreenWidth = 100;
	mScreenHeight = 100;

	isRunning = true;
			
	mTextCursor = 0;
	mSelectionLength = 0;

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
			
			//Screen dimensions 
			if(FULLSCREEN){
				mScreenWidth  = display.w;
				mScreenHeight = display.h;
			}
			else{
				mScreenWidth  = display.w * 0.8;
				mScreenHeight = display.h * 0.8;
			}
			SDL_SetWindowSize(mWindow, mScreenWidth, mScreenHeight);
			SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

			//Locks cursor to window
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
				if(SDL_IsTextInputActive() && mInputText.length() > 0){
					if(mEvents.key.keysym.sym == SDLK_BACKSPACE && mTextCursor > 0){
						mInputText.erase(--mTextCursor, 1);
					}
					if((mEvents.key.keysym.sym == SDLK_RIGHT || mEvents.key.keysym.sym == SDLK_PAGEUP) && mTextCursor < mInputText.length()){
						mTextCursor++;
					}
					if((mEvents.key.keysym.sym == SDLK_LEFT  || mEvents.key.keysym.sym == SDLK_PAGEDOWN) && mTextCursor > 0){
						mTextCursor--;
					}
				}
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
				mInputText.insert(mTextCursor++, mEvents.text.text);
				break;
			case SDL_TEXTEDITING:
				mCompositionText = mEvents.edit.text;
				mTextCursor = mEvents.edit.start;
				mSelectionLength = mEvents.edit.length;
				break;
			default:
				break;
		}
	}

	for(auto b : Button::gButtons){
		b->update(keystate);
	}
}

void GameManager::draw(	Shader& shader, Mesh& mesh, FrameBuffer& framebuffer){

	shader.bind();
	for(auto p: mesh.getParameters() ){
		shader.setParameter( p.name, p.type, p.value, false); 
	}

	this->draw(shader, mesh.getVertArray(), mesh.getMaterials(), framebuffer, mesh.getDrawMode());

	shader.resetParameters();
	shader.unbind();
}

void GameManager::draw(	Shader& shader, VertArray& vao, std::vector<Material> materials, FrameBuffer& framebuffer, GLenum drawMode){

	framebuffer.bind(GL_FRAMEBUFFER);

	shader.bind();
	
	for(unsigned int m = 0; m < materials.size(); m++){	
		glActiveTexture(GL_TEXTURE0 + m);		
		materials[m].texture->bind();
		if( !shader.setParameter(("material." + materials[m].type), GL_INT, &m, false) ) return;
	}

	glActiveTexture(GL_TEXTURE0);
	
	auto indicies = vao.getIndexBuffer();

	vao.bind();
	
	glDrawElements(drawMode, indicies.size(), GL_UNSIGNED_INT, 0);

	vao.unbind();
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		cout << "Mesh::Draw: error while rendering: " << gluErrorString(error) << endl;
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
