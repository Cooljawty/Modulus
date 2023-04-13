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
#include "parser/VertexArray.h"
#include "parser/material.h"

using namespace Modulus;

GameManager gGameContext;

FrameBuffer gFrameBuffer;
FrameBuffer gmsFrameBuffer;

VertArray gFrameBufferVAO;

//PI to 5 digits
const double PI = 3.141590;

Font gFont;
std::string testText = "Hello World!";
int textSize = 24;

//Background transform and texture
Material backgroundMaterial;
VertArray backgroundVAO;
Mesh* backgroundMesh;
Transform backgroundTrans(0.0,0.0,1.0);

//Texture and transform for player
Texture playerTex;
VertArray playerVAO;
Transform playerTrans(0.0,0.0,0.7);
Vector2D playerForce(0.0, 0.0);

//Testing model loading
Model testModel;

//Testing line shader
VertArray testLine;

//Times jump duration
Timer gJumpTimer;
//The maximum height of the jump
const double gJumpHeight = 7.0;
//The total length of the jump
const double gJumpTime = 0.75;
//Total number of jumps
unsigned int gJumpCount = 0;
unsigned int gJumpTotal = 3;
//Check if on ground
bool grounded = true;
Vector2D gravity((8.0*gJumpHeight)/(gJumpTime*gJumpTime), -PI/2.0);

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
PolygonShader gPolygonShader;
TextureShader gTexShader;
TextShader gTextShader;
SpriteShader gSpriteShader;
LampShader gLampShader;
FBOShader gFBOShader;
LineShader gLineShader;

unsigned int gSpriteRow = 0;
unsigned int gSpriteCollumn = 0;

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

	bUp = new Button (SDL_SCANCODE_UP);
	bUp->addInput(SDL_SCANCODE_W);
	
	bDown = new Button (SDL_SCANCODE_DOWN);
	bDown->addInput(SDL_SCANCODE_S);
	
	bLeft = new Button (SDL_SCANCODE_LEFT);
	bLeft->addInput(SDL_SCANCODE_A);
	
	bRight = new Button (SDL_SCANCODE_RIGHT);
	bRight->addInput(SDL_SCANCODE_D);
	
	bRotateL = new Button (SDL_SCANCODE_Q);
	bRotateR = new Button (SDL_SCANCODE_E);
	bJump = new Button (SDL_SCANCODE_SPACE);
	bDebug = new Button (SDL_SCANCODE_GRAVE);
	bReturn = new Button (SDL_SCANCODE_RETURN);
	bExit = new Button (SDL_SCANCODE_ESCAPE);

	if(gGameContext.init()){

		//Initilize OpenGL
		if(!gGameContext.initOGL()){
			gGameContext.close();
			return 1;
		}

		//Initilize Graphics Pipeline
		if(!initGP()){
			gGameContext.close();
			return 1;
		}

		//Load media
		if(!loadMedia()){
			std::cout << "Failed to load media" << std::endl;
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

	//MSAA FBO
	if(!gmsFrameBuffer.init(gGameContext.getScreenWidth(), gGameContext.getScreenHeight(), true)){
		std::cout << "Initilizing Graphics Pipeline: Could not initilized multisample Framebuffer." << std::endl;
		return false;
	}
	gGameContext.addFrameBuffer(gmsFrameBuffer);	
	//FBO for post-processing
	if(!gFrameBuffer.init(gGameContext.getScreenWidth(), gGameContext.getScreenHeight())){
		std::cout << "Initilizing Graphics Pipeline: Could not initilized global Framebuffer." << std::endl;
		return false;
	}
	gGameContext.addFrameBuffer(gFrameBuffer);	

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
	
	if(!gPolygonShader.loadProgram()){
			std::cout << "Unable to load polygon shader." << std::endl;
			return false;
	}
	gPolygonShader.bind();
		gPolygonShader.setPVMatrix(gProjectionMatrix * gCamera.viewMatrix);
		gPolygonShader.updatePVMatrix();
		gPolygonShader.setModelMatrix(testModel.getModelMatrix());
		gPolygonShader.updateModelMatrix();

		gPolygonShader.setViewPosition(glm::vec3(0.f, 0.f, 1000.f));
		gPolygonShader.updateViewPosition();

		gPolygonShader.setLightPosition(glm::vec3(-75.f, 100.f, 1000.f));
		gPolygonShader.updateLightPosition();

		gPolygonShader.setShininess(32.0f);

		gPolygonShader.setLightAmbiant(0.2f, 0.2f, 0.2f);
		gPolygonShader.setLightDiffuse(0.5f, 0.5f, 0.5f);
		gPolygonShader.setLightSpecular(0.7f, 0.7f, 0.7f);
		gPolygonShader.resetParameters();
	gPolygonShader.unbind();
	gGameContext.addShader(gPolygonShader);
	gGameContext.bindFrameBuffertoShader(gmsFrameBuffer, gPolygonShader);
	
	//Load texture shader program
	if(!gTexShader.loadProgram()){
		std::cout << "Unable to load texture shader." << std::endl;
		return false;
	}
	gTexShader.bind();
		gTexShader.setProjectionMatrix(gProjectionMatrix);
		gTexShader.updateProjectionMatrix();
		gTexShader.setViewMatrix(gCamera.viewMatrix);
		gTexShader.updateViewMatrix();
		gTexShader.setModelMatrix(glm::mat4(1.f));
		gTexShader.updateModelMatrix();
		gTexShader.setTexUnit(0);
	gTexShader.unbind();
	gGameContext.addShader(gTexShader);
	gGameContext.bindFrameBuffertoShader(gmsFrameBuffer, gTexShader);

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
	gGameContext.addShader(gTextShader);
	gGameContext.bindFrameBuffertoShader(gmsFrameBuffer, gTextShader);

	/*
	if(!gLampShader.loadProgram()){
			std::cout << "Unable to load lamp shader." << std::endl;
			return false;
	}
	gLampShader.bind();
		gLampShader.setProjectionMatrix(gProjectionMatrix);
		gLampShader.updateProjectionMatrix();
		gLampShader.setViewMatrix(gCamera.viewMatrix);
		gLampShader.updateViewMatrix();
		gLampShader.setModelMatrix(lampMat);
		gLampShader.updateModelMatrix();
	gLampShader.unbind();

	//Load texture shader program
	if(!gSpriteShader.loadProgram()){
		std::cout << "Unable to load sprite shader." << std::endl;
		return false;
	}
	gSpriteShader.bind();
		gSpriteShader.setProjectionMatrix(gProjectionMatrix);
		gSpriteShader.updateProjectionMatrix();
		gSpriteShader.setViewMatrix(gCamera.viewMatrix);
		gSpriteShader.updateViewMatrix();
		gSpriteShader.setModelMatrix(glm::mat4(1.f));
		gSpriteShader.updateModelMatrix();
		gSpriteShader.setSpriteTexture(0);
	gSpriteShader.unbind();
	
	if(!gLineShader.loadProgram()){
		std::cout << "Unable to load Line shader." << std::endl;
	}
	gLineShader.bind();
		gLineShader.setProjectionMatrix(gProjectionMatrix);
		gLineShader.updateProjectionMatrix();
		gLineShader.setViewMatrix(gCamera.viewMatrix);
		gLineShader.updateViewMatrix();
		gLineShader.setModelMatrix(glm::mat4(1.f));
		gLineShader.updateModelMatrix();
	gLineShader.unbind();
	*/

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
	if(!gFont.loadFont(ASSET_PATH "Fonts/Open_Sans/OpenSans-Regular.ttf", textSize)){
		std::cout << "Load Media: Failed to load font \'" << ASSET_PATH "Fonts/OpenSans/OpenSans-Regular.ttf" << "\'." << std::endl;
		success = false;
	}
	else{
		gFont.initVAO(gTextShader);
	}
	
	//Load textures
	if(!playerTex.loadFromImage(ASSET_PATH "Debug_Sprite.png")){
		std::cout << "Load Media: Unable to load texture" << std::endl;
		success = false;
	}

	testModel.loadModel(ASSET_PATH "backpack/backpack.obj");
	gGameContext.addMeshes(testModel.mMeshes);
	for(auto m: testModel.mMeshes){
		gGameContext.bindMeshtoShader(*m, gPolygonShader);
	}
	
	std::vector<GLuint> iData{
			0,1,2,3
	};
	
	//vWidth  = (float)backgroundMaterial.getWidth();
	//vHeight = (float)backgroundMaterial.getHeight();
	//std::vector<GLfloat> backgroundData{
	//	0,  		vHeight, 0.0f,  0.0f,
	//	0, 			0, 			 0.0f,  1.0f,
	//	vWidth, 0, 			 1.0f,  1.0f,
	//	vWidth, vHeight, 1.0f,  0.0f,
	//};
	//backgroundVAO.addAttribute(gTexShader.getVertexPosID(), 2, GL_FLOAT);
	//backgroundVAO.addAttribute(gTexShader.getTexCoordID(), 2, GL_FLOAT);
	//backgroundVAO.initVAO<GLfloat>(backgroundData, iData, GL_STATIC_DRAW);
	string backgroundMaterialsrc = "<texture> \"" ASSET_PATH "Solo_Jazz.png\"";
	if(!Parse::parseMaterial(backgroundMaterialsrc, backgroundMaterial)){
		std::cout << "LoadMedia: Error parsing material" << std::endl;
	}
	string strWidth =  std::to_string(backgroundMaterial.texture->getWidth());
	string strHeight = std::to_string(backgroundMaterial.texture->getHeight());
	string backgroundVAOsrc = "\
		<2,float> <2,float> [\
		 " + strWidth + ",  " + strHeight + ", 0.0, 0.0,\
		-" + strWidth + ",  " + strHeight + ", 1.0, 0.0,\
		-" + strWidth + ", -" + strHeight + ", 1.0, 1.0,\
		 " + strWidth + ", -" + strHeight + ", 0.0, 1.0 ]\
		<index> [ 0, 1, 2, 3 ]";
	if(!Parse::parseVA(backgroundVAOsrc, backgroundVAO)){
		std::cout << "LoadMedia: Error parsing vao" << std::endl;
	}
	backgroundMesh = new Mesh(backgroundVAO, {backgroundMaterial}, GL_TRIANGLE_FAN);
	gGameContext.addMesh(*backgroundMesh);
	gGameContext.bindMeshtoShader(*backgroundMesh, gTexShader);

	//if(!backgroundMaterial.loadFromImage(ASSET_PATH "Solo_Jazz.png")){
	//	std::cout << "Load Media: Unable to load texture" << std::endl;
	//	success = false;
	//}
	
	/*
	GLfloat vWidth  = 255;
	GLfloat vHeight = 255;
	GLfloat s = gSpriteRow * 256 +1;
	GLfloat t = gSpriteCollumn *256 +1;
	std::vector<GLfloat> playerData{
		0,  		vHeight, s,					 t,
		vWidth, vHeight, s + vWidth, t,
		vWidth, 0, 			 s + vWidth, t+vHeight,
		0, 			0, 			 s,					 t+vHeight,
	};
	playerVAO.addAttribute(gSpriteShader.getVertexPosID(), 2, GL_FLOAT);
	playerVAO.addAttribute(gSpriteShader.getTexCoordID(), 2, GL_FLOAT);
	playerVAO.initVAO<GLfloat>(playerData, iData, GL_STATIC_DRAW);


	std::vector<GLfloat> lineVAO{
		-10.f,  0.f, 0.f,	1.f, 0.f, 0.f,
	 	  0.f, 10.f, 0.f,	0.f, 1.f, 0.f,
		 10.f,  0.f, 0.f,	0.f, 0.f, 1.f,
	};
	testLine.addAttribute(gLineShader.getVertexPosID(), 3, GL_FLOAT);
	testLine.addAttribute(gLineShader.getVertexColorID(), 3, GL_FLOAT);
	testLine.initVAO(lineVAO, std::vector<unsigned int>{0,1, 1,2}, GL_STATIC_DRAW);
	*/
	
	std::vector<GLfloat> fboVAO{
		-1.f,  1.f, 0.f,	1.f,
		-1.f, -1.f, 0.f,	0.f,
		 1.f, -1.f, 1.f, 0.f,
		 1.f,  1.f, 1.f, 1.f,
	};
	gFrameBufferVAO.addAttribute(gFBOShader.getVertexPosID(), 2, GL_FLOAT);
	gFrameBufferVAO.addAttribute(gFBOShader.getTextureCoordID(), 2, GL_FLOAT);
	gFrameBufferVAO.initVAO(fboVAO, iData, GL_DYNAMIC_DRAW);

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
		if(bJump->getState() == 1 && ((gJumpCount < gJumpTotal) || grounded)){
			gJumpTimer.start();
			gJumpCount++;
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
		std::cout << "Text input " << (SDL_IsTextInputActive() ? "Disabled." : "Enabled.") << std::endl;
		SDL_IsTextInputActive() ? SDL_StopTextInput() : SDL_StartTextInput();
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
	if(bExit->getState()){
		SDL_Event e;
		e.type = SDL_QUIT;
		SDL_PushEvent(&e);
  	}
} 

//Updates on each frame
void update(){

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
	
	//Render to multisampled FBO
	gmsFrameBuffer.bind(GL_FRAMEBUFFER);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	gPolygonShader.bind();
		gPolygonShader.setPVMatrix(gProjectionMatrix * gCamera.viewMatrix);
		gPolygonShader.updatePVMatrix();
	gPolygonShader.unbind();
	
	gTexShader.bind();
		gTexShader.setViewMatrix(gCamera.viewMatrix);
		gTexShader.updateViewMatrix();
	gTexShader.unbind();
	
	gGameContext.drawQueue();	
	
	/*
	 gTexShader.bind();

		//Render texture
		backgroundVAO.bind();
		backgroundMaterial.bind();
			//gTexShader.setModelMatrix(textModel.mModelMatrxix);
			//gTexShader.updateModelMatrix();
			gTexShader.setViewMatrix(gCamera.viewMatrix);
			gTexShader.updateViewMatrix();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		backgroundMaterial.unbind();
		backgroundVAO.unbind();

	gTexShader.unbind();
	
	gLineShader.bind();
	testLine.bind();
	gLineShader.setViewMatrix(gCamera.viewMatrix);
	gLineShader.updateViewMatrix();
	glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, (void*)0);
	testLine.unbind();
	gLineShader.unbind();

	gSpriteShader.bind();

		//Render player
		playerVAO.bind();
		playerTex.bind();
			gSpriteShader.setTextureDimensions(playerTex.getWidth(), playerTex.getHeight());
			gSpriteShader.setModelMatrix(textModel.mModelMatrxix);
			gSpriteShader.updateModelMatrix();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		playerVAO.unbind();
		playerTex.unbind();

	gSpriteShader.unbind();
	*/

	//Print text
	if(textTimer.getTime() > 5.f){
		//testText = "FPS: " + std::to_string(dTime.getTime())/*.substr(0, 5)*/;
		textTimer.start();
	}
	
	//Copy multisampled FBO to post-processing FBO
	gmsFrameBuffer.bind(GL_READ_FRAMEBUFFER);
	gFrameBuffer.bind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(0,0,gGameContext.getScreenWidth(),gGameContext.getScreenHeight(),
					  0,0,gGameContext.getScreenWidth(),gGameContext.getScreenHeight(),
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	//Render frame buffer to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	gFBOShader.bind();
			gFrameBufferVAO.bind();
			gFrameBuffer.bindTexture();
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	gFBOShader.unbind();

	/*
	int x, y = 0;
	gGameContext.mouseCursor.getCoords(x,y);
	if(textTimer.getTime() >= 0.5){
		testText = "FPS: " + std::to_string(1/dTime.getTime());
		textTimer.start();
	}
	gFont.renderText(gTextShader, testText, 0, gGameContext.getScreenHeight() - textSize, 1.f, glm::vec3(1.f,1.f,1.f));
	*/
	gFont.renderText(gTextShader, gGameContext.getInputText(), 0, gGameContext.getScreenHeight() - textSize, 1.f, glm::vec3(1.f,1.f,1.f));
	gFont.renderText(gTextShader, std::to_string(gGameContext.mTextCursor), 0, gGameContext.getScreenHeight() - textSize * 2, 1.f, glm::vec3(1.f,1.f,1.f));

	//Update screen
	SDL_GL_SwapWindow(gGameContext.getWindow());

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
 	if(gJumpTimer.started()){
		if(gJumpTimer.getTime() <= gJumpTime){
			gCamera.position.y += (-1.0*abs(gravity.getYComp())*gJumpTimer.getTime()) + sqrt(2.0 * gJumpHeight * abs(gravity.getYComp())) * (1.0-dTime.getTime());
			grounded = false;
		} 
		else{ 
			gJumpTimer.stop();
			gCamera.position.y -= gravity.getForce() * (1.0-dTime.getTime());
		}
	}
	else{
			gCamera.position.y -= gravity.getForce() * (1.0-dTime.getTime());
	} 
	if(gCamera.position.y <= 0){
		gCamera.position.y = 0.f; 
		gJumpCount = 0;
		grounded = true;
 	}

	gCamera.viewMatrix = glm::lookAt(gCamera.position,
									 gCamera.position + rollDirection,
									 glm::cross(rollDirection, pitchDirection));
}
