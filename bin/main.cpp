/*--------------------------------------------------*
 | Modulus Engine									|
 | A modular 2D game engine made with SDL2 & OpenGL	|
 | Version: 0.0.1									|
 | Author: Jacari Harper							|
 | Created: 02/01/2020 09:02						|
 | Updated: 06/07/2021 11:59						|
 *--------------------------------------------------*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "modulus.h"
#include "parser/lua/lua.h"

using namespace Modulus;

GameManager gGameContext;

Parse::Lua::Context gLuaContext( gGameContext );
using Parse::Lua::gLuaFrameBuffers;

Font gFont;
std::string testText = "Hello\nWorld!";
int textSize = 12;
int textResolution = 300;
float textScale = static_cast<float>(textSize) / textResolution;

//Testing model loading
Model testModel;

struct Jumper {
	//Times jump duration
	Timer timer;
	
	//The maximum height of the 
	double height = 7.0;
	
	//The total length of the 
	double time = 0.75;

	//Total number of s
	unsigned int count = 0;
	unsigned int total = 3;
	
	//Check if on ground
	bool grounded = true;

} gJump;

Vector2D gravity( (8.0 * gJump.height)/(gJump.time * gJump.time), -3.14/2.0);

//Camera transform
struct Camera{
	glm::mat4 viewMatrix;
	glm::vec3 position;
	glm::quat orientation;
	const float speed = 10.f;
} gCamera;

//Default projection matrix
glm::mat4 gProjectionMatrix;

//Shader declarations:
TextShader gTextShader;
FBOShader gFBOShader;

//Delta time
Timer dTime;

//Calculating FPS
Timer fpsTimer;
float gFrameCount = 0.f;
float gFPS = 0.f;

//Timer for repeated text output
Timer textTimer;

Button* bUp;
Button* bDown;
Button* bLeft;
Button* bRight;
Button* bRotateL;
Button* bRotateR;
Button* bJump;
Button* bDebug;
Button* bReturn;
Button* bExit;

//Intilizes the OpenGL Graphics Pipeline
bool initGP();

//Loads media like images, text, soundfiles, ect.
bool loadMedia();

//Update baised on input
void inputs();

//Updates on each frame
void update();

//Renders objects
void render();

//Renders a given string to screen
void RenderText(TextShader &shader, VertArray &mesh, std::string text, float x, float y, float scale, glm::vec3 color);

//Takes a rotation and direction vector and applies translation and rotation of camera
void MoveCamera( glm::vec2 rotation, glm::vec3 direction);

int main(int argc, char* argv[]){
	//std::cout << std::to_string(SHADER_PATH) << std::endl
	//		  << std::to_string(ASSET_PATH) << std::endl;

	bUp      = new Button({"Up", "W"});
	bDown    = new Button({"Down", "S"});
	bLeft    = new Button({"Left", "A"});
	bRight   = new Button({"Right", "D"});
	
	bRotateL = new Button("Q");
	bRotateR = new Button("E");
	bJump    = new Button("Space");
	bDebug   = new Button("`");
	bReturn  = new Button("Return");
	bExit    = new Button("Escape");

	if(gGameContext.init()){

		//Initilize OpenGL
		if(    !gGameContext.initOGL()
			|| !initGP()
			|| !loadMedia()
			|| !gLuaContext.init()
			|| !gLuaContext.loadFile(ASSET_PATH "scripts/init.lua") )
		{
			gGameContext.close();
			return 1;
		}


		//Disable test input by default
		SDL_StopTextInput();

		fpsTimer.start();
		textTimer.start();
		dTime.start();

		//prevCursor = {gGameContext.getScreenWidth()/2, gGameContext.getScreenHeight()/2};
		
		//Program loop
		while(gGameContext.getRunning()){

			gGameContext.pollEvents();

			inputs();

			update();

			render();
			
			dTime.start();
 	 	}

  	}

	gGameContext.close();

	delete bUp;
	delete bDown; 
	delete bLeft; 
	delete bRight;
	delete bRotateL;
	delete bRotateR;
	delete bJump;
	delete bDebug;
	delete bReturn;
	delete bExit; 
	
	return 0;
}

//Intilizes the OpenGL Graphics Pipeline
bool initGP(){

	if(!gFBOShader.loadProgram()){
		std::cout << "Unable to load FBO shader." << std::endl;
		return false;
	}
	gFBOShader.bind();
		gFBOShader.setTexture(0);
	gFBOShader.unbind();

	//Set default projection matrix
	float fov = 45;
	gProjectionMatrix = glm::perspective(
							glm::radians(fov),
							(float)gGameContext.getScreenWidth() / (float)gGameContext.getScreenHeight(),
							0.1f,
							(float)gGameContext.getScreenWidth() * 10.f);
	
	gCamera.position = glm::vec3(0.f, 0.f, 1000.f);

	testModel.setModelMatrix(glm::translate(glm::scale(	glm::mat4(1.f), 
														glm::vec3(1.f, 1.f, 1.f) * 75.f), 
														glm::vec3(0.f,0.f,5.f)));

	//Load texure shader program
	if(!gTextShader.loadProgram()){
		std::cout << "Unable to load text shader." << std::endl;
		return false;
	}
	gTextShader.bind();
		gTextShader.setTextColor(1.f, 1.f, 1.f);
		gTextShader.setProjectionMatrix(glm::ortho(0.f, static_cast<float>(gGameContext.getScreenWidth()), 0.f, static_cast<float>(gGameContext.getScreenHeight())));
		gTextShader.updateProjectionMatrix();
	gTextShader.unbind();

	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "InitGP: Error initilizing texture shader: " << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

//Loads media like images, text, soundfiles, ect.
bool loadMedia(){

	//Success flag
	bool success = true;
	
	//Load font
	if(!gFont.loadFont("/usr/share/fonts/type1/urw-base35/NimbusSans-Regular.t1", textSize, textResolution)){
		std::cout << "Load Media: Failed to load font \'" << ASSET_PATH "Fonts/OpenSans/OpenSans-Regular.ttf" << "\'." << std::endl;
		success = false;
	}
	else{
		gFont.initVAO(gTextShader.getVertexID(), gTextShader.getVertexID() + 1);
	}

	testModel.loadModel(ASSET_PATH "backpack/backpack.obj");

	return success;
}

//Update baised on input
void inputs(){ 

	//Movement
	//double xForce = 0.0;
	//double yForce = 0.0;
	//playerForce.setVector(0.0,0.0);
	if(!SDL_IsTextInputActive()){
		static int xprev, yprev; 
		int x,y;
		gGameContext.mouseCursor.getCoords(x,y);
		glm::vec2 rotation(x - xprev, y - yprev);
		xprev = x, yprev = y;
		glm::vec3 direction(0.f);
		if(bUp->getState()){
			direction.x = 1;
		}
		if(bDown->getState()){
			direction.x = -1;
		}
		if(bLeft->getState()){
			direction.y =  1;
		}
		if(bRight->getState()){
			direction.y = -1;
		}
		if(bJump->getState() == 1 && ((gJump.count < gJump.total) || gJump.grounded)){
			gJump.timer.start();
			gJump.count++;
		}
		
		MoveCamera(rotation, direction);
	}
	//playerForce += Vector2D(sqrt(xForce * xForce + yForce * yForce), (xForce == 0.0 && yForce == 0.0) ? 0.0 : atan2(yForce,xForce));

	/*if(bUp->getState() || bDown->getState() || bLeft->getState() || bRight->getState()){
		if(bUp->getState() == 1){
			gSpriteCollumn++;
		}
		if(bDown->getState() == 1){
			if(gSpriteCollumn > 0) gSpriteCollumn--;
		}
		if(bRight->getState() == 1){
			gSpriteRow++;
		}
		if(bLeft->getState() == 1){
			if(gSpriteRow > 0) gSpriteRow--;
		}

		GLfloat vWidth  = 255;
		GLfloat vHeight = 255;
		GLfloat s = gSpriteRow * 256 +1;
		GLfloat t = gSpriteCollumn *256 +1;
		std::vector<GLfloat> spriteClip{
			s,			t,
			s+ vWidth,	t,
			s+ vWidth,	t+vHeight,
			s,			t+vHeight,
		};
		playerVAO.updateAttribute(gSpriteShader.getTexCoordID(), GL_ARRAY_BUFFER, spriteClip);
	}*/

	/*
	if(bRotateL->getState()){
		gTexShader.bind();
		gTexShader.leftMultiplyViewMatrix(glm::translate(glm::vec3(1.f, 0.f, 0.f)));
		gTexShader.updateViewMatrix();
		gTexShader.unbind();
	}
	if(bRotateR->getState()){
		gTexShader.bind();
		gTexShader.leftMultiplyViewMatrix(glm::translate(glm::vec3(-1.f, 0.f, 0.f)));
		gTexShader.updateViewMatrix();
		gTexShader.unbind();
	}
	*/

	//Debug toggle
	if(bDebug->getState() == 1){
	 	/*
		if(!gFBOShader.loadProgram()){
			std::cout << "Unable to load polygon shader." << std::endl;
		}
		else{
	 		std::cout << "PolygonShader recompiled!" << std::endl;

			gFBOShader.bind();
				//gFBOShader.setProjectionMatrix(gProjectionMatrix);
				//gFBOShader.updateProjectionMatrix();
				//gFBOShader.setViewMatrix(gCamera.viewMatrix);
				//gFBOShader.updateViewMatrix();
				//gFBOShader.setModelMatrix(glm::mat4(1.f));
				//gFBOShader.updateModelMatrix();
			gFBOShader.unbind();
		}
	 	*/
		//std::cout << "Text input " << (SDL_IsTextInputActive() ? "Disabled." : "Enabled.") << std::endl;
		//SDL_IsTextInputActive() ? SDL_StopTextInput() : SDL_StartTextInput();
	} 

	//End text input	
	if(bReturn->getState() == 1 && SDL_IsTextInputActive()){
		std::cout << std::endl << "Text input disabled." << std::endl;
		SDL_StopTextInput();
	}

	/*Mouse Button controls
	if(gMouseButton->state == UNTOUCHED){
		mouseButtonTex.setColor(255,255,255);
	}
	if(gMouseButton->state == HOVERED){
		mouseButtonTex.setColor(255-50,255-50,255);
	}
	if(	   gMouseButton->state == CLICKEDR
		|| gMouseButton->state == CLICKEDM
		|| gMouseButton->state == CLICKEDL){
		mouseButtonTex.setColor(255/10, 255, 255/10);

		//TODO:Click and drag
		//SDL_Rect* mouseRect = mouseButtonTrans.toScreen(0,0,mouseButtonTex.getWidth(), mouseButtonTex.getHeight());
		//mouseButtonTrans.setX(mouseRect->x);
		//mouseButtonTrans.setY(mouseRect->y);
		mouseButtonTrans.setX(-((gGameContext.getScreenWidth();g/2) - gGameContext.mouseCursor.getCoords()->x));
		mouseButtonTrans.setY((gGameContext.getScreenHeight()/2) - gGameContext.mouseCursor.getCoords()->y);

	}*/

	//Exit
	//if(bExit->getState()){
	//	SDL_Event e;
	//	e.type = SDL_QUIT;
	//	SDL_PushEvent(&e);
  	//}
} 

//Updates on each frame
void update(){

	gLuaContext.loadFile(ASSET_PATH "scripts/update.lua");
	//playerTrans.movX(playerForce.getXComp());
	//playerTrans.movY(playerForce.getYComp());
	//textModel.mModelMatrxix = textModel.mModelMatrxix * glm::translate(glm::vec3(playerForce.getXComp(), playerForce.getYComp(), 0.f));
	//textModel.mModelMatrxix = glm::rotate<float>(textModel.mModelMatrxix, 0.01f, glm::vec3(0.f,1.f,0.f));

	//gPolygonShader.setLightPosition( glm::vec3(camx, camz, 1000.f));
	//lampMat = glm::translate(lampMat, glm::vec3(camx, (float)gGameContext.getScreenHeight(), camz));
	
	//compositionText 
}

//Renders objects
void render(){
	
	for( auto fb : gLuaFrameBuffers){
		fb->clear(GL_FRAMEBUFFER);
		glEnable(GL_DEPTH_TEST);
	}

	//gLuaContext.loadFile(ASSET_PATH "scripts/render.lua");

	(*gLuaFrameBuffers.back()).bind(GL_FRAMEBUFFER);
	gFont.renderText( gTextShader, testText, 0, gGameContext.getScreenHeight() / 2.0,      textScale,  glm::vec3(0,0,1) );
	gFont.renderText( gTextShader, "Second text", 0, gGameContext.getScreenHeight() / 2.0, textScale,  glm::vec3(1,1,0.2) );

	gGameContext.drawToScreen( gFBOShader, *gLuaFrameBuffers.front() );
	
	GLenum error = glGetError();
 	 if(error != GL_NO_ERROR){
	 	std::cout << "Render: Error while rendering: " << gluErrorString(error) << std::endl;
	}
}

void MoveCamera( glm::vec2 rotation, glm::vec3 direction){
	const float sensitivity = 0.007f;
	
	auto pitch = glm::quat(glm::vec3(-rotation.y,0.f,0.f) * sensitivity);
	auto yaw = glm::quat(glm::vec3(0.f,-rotation.x, 0.f) * sensitivity);
		
	gCamera.orientation = glm::normalize(yaw * gCamera.orientation * pitch);
	
	auto rollDirection = gCamera.orientation * glm::vec3(0,0,-1);
	auto pitchDirection = gCamera.orientation * glm::vec3(-1,0,0);

	gCamera.position	+= direction[0] * rollDirection * gCamera.speed;
	gCamera.position	+= direction[1] * pitchDirection * gCamera.speed;
	//gCamera.position.y	+= direction[2] * gCamera.speed;
	
	//Calculate jump position
 	if(gJump.timer.started()){
		if(gJump.timer.getTime() <= gJump.time){
			gCamera.position.y += (-1.0*abs(gravity.getYComp())*gJump.timer.getTime()) + sqrt(2.0 * gJump.height * abs(gravity.getYComp())) * (1.0-dTime.getTime());
			gJump.grounded = false;
		} 
		else{ 
			gJump.timer.stop();
			gCamera.position.y -= gravity.getForce() * (1.0-dTime.getTime());
		}
	}
	else{
			gCamera.position.y -= gravity.getForce() * (1.0-dTime.getTime());
	} 
	if(gCamera.position.y <= 0){
		gCamera.position.y = 0.f; 
		gJump.count = 0;
		gJump.grounded = true;
 	}

	gCamera.viewMatrix = glm::lookAt(gCamera.position,
									 gCamera.position + rollDirection,
									 glm::cross(rollDirection, pitchDirection));
}
