/*------------------------------------------------------------------------------*
 | Modulus Engine																|
 | A modular 2D game engine made with SDL2 & OpenGL								|	
 | Version: 0.0.1															   	|
 | Author: Jacari Harper														|
 | Created: 02/01/2020 09:02													|
 | Updated: 02/03/2021 21:24													|
 *----------------------------------------------------------------------------- */

/* TODO:
	Camera controller
*/
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "modulus.h"

GameManager gGameContext;

unsigned int gFrameBuffer;
unsigned int gRenderBuffer;
Texture gFBOTexture;

unsigned int gmsFrameBuffer;
unsigned int gmsRenderBuffer;
GLuint gmsFBOTexture;

VertArray gFrameBufferVAO;

//PI to 5 digits
const double PI = 3.141590;

//Path to program's directory
const std::string PATH = "/home/harper/Dev/Modulus/";

struct Character{
	unsigned int TextureID;
	glm::vec2 Size;
	glm::vec2 Bearing;
	long int Advance;
};
std::map<char, Character> Characters;

VertArray gTextVAO;
std::string testText = "Hello World!";
int textSize = 24;

std::string compositionText;
Sint32 gTextCursor;
Sint32 selection_len;

//Background transform and texture
Texture backgroundTex;
VertArray backgroundVAO;
Transform backgroundTrans(0.0,0.0,1.0);

//Texture and transform for player
Texture playerTex;
VertArray playerVAO;
glm::mat4 playerMat = glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 1.f) * 75.f), glm::vec3(0.f,0.f,5.f));//glm::scale(glm::vec3(1.f,1.f,1.f) * 0.5f);
Transform playerTrans(0.0,0.0,0.7);
Vector2D playerForce(0.0, 0.0);

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

//Testing 3d Polygon shader
VertArray testPoly;
Texture diffuseTex;
Texture specularTex;
glm::vec4 polyPositions[8] ={
	glm::vec4(-1.5f,  1.5f, -1.5f, 1.5f),
	glm::vec4( 1.5f, -1.5f, -1.5f, 1.5f),
	glm::vec4(-1.5f, -1.5f, -1.5f, 1.5f),
	glm::vec4( 1.5f,  1.5f, -1.5f, 1.5f),
	glm::vec4(-1.5f,  1.5f, 1.5f, -1.5f),
	glm::vec4( 1.5f, -1.5f, 1.5f, -1.5f),
	glm::vec4(-1.5f, -1.5f, 1.5f, -1.5f),
	glm::vec4( 1.5f,  1.5f, 1.5f, -1.5f),
};

//Set player movement speed
double gPlayerSpeed = 20.0;

//Camera transform
SDL_Point prevCursor = {0,0};
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

//Handles xml save file I/O
//XMLFile level;

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

		//Disable test inpu by default
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

	return 0;
}

//Intilizes the OpenGL Graphics Pipeline
bool initGP(){

	//Set default projection matrix
	float fov = 45;
	gProjectionMatrix = glm::perspective(
							glm::radians(fov),
							(float)gGameContext.getScreenWidth() / (float)gGameContext.getScreenHeight(),
							0.1f,
							(float)gGameContext.getScreenWidth() * 10.f);
	/*gProjectionMatrix = glm::ortho( 0.f, (float)gGameContext.getScreenWidth(),
																	0.f, (float)gGameContext.getScreenHeight(),
																	0.f, 100.f);
	*/

	gCamera.position = glm::vec3(0.f, 0.f, 1000.f);

	if(!gPolygonShader.loadProgram()){
			std::cout << "Unable to load polygon shader." << std::endl;
			return false;
	}
	gPolygonShader.bind();
		gPolygonShader.setProjectionMatrix(gProjectionMatrix);
		gPolygonShader.updateProjectionMatrix();
		gPolygonShader.setViewMatrix(gCamera.viewMatrix);
		gPolygonShader.updateViewMatrix();
		gPolygonShader.setModelMatrix(glm::scale(glm::mat4(1.f), glm::vec3(50.f,50.f,50.f)));
		gPolygonShader.updateModelMatrix();

		gPolygonShader.setViewPosition(glm::vec3(0.f, 0.f, 1000.f));
		gPolygonShader.updateViewPosition();

		gPolygonShader.setLightPosition(glm::vec3(-75.f, 100.f, 1000.f));
		gPolygonShader.updateLightPosition();

		gPolygonShader.setInt("material.diffuse", 0);
		gPolygonShader.setInt("material.specular", 1);
		gPolygonShader.setFloat("material.shininess", 32.0f);

		gPolygonShader.setVec3("light.ambiant",  0.2f, 0.2f, 0.2f);
		gPolygonShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
		gPolygonShader.setVec3("light.specular", 0.7f, 0.7f, 0.7f);
	gPolygonShader.unbind();

	/*if(!gLampShader.loadProgram()){
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
	*/

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

	if(!gFBOShader.loadProgram()){
		std::cout << "Unable to load FBO shader." << std::endl;
		return false;
	}
	gFBOShader.bind();
		//gFBOShader.setTexture(0);
		gFBOShader.setInt("screenTexture", 0);
	gFBOShader.unbind();

	//MSAA FBO
	glGenFramebuffers(1, &gmsFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gmsFrameBuffer);

	glGenTextures(1, &gmsFBOTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gmsFBOTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4,
		 											GL_RGB,
													gGameContext.getScreenWidth(), gGameContext.getScreenHeight(),
													GL_TRUE);
	if(gmsFBOTexture == 0){
		std::cout << "Initilize graphics pipeline: Error creating multisampled texture." << std::endl;
		return false;
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, gmsFBOTexture, 0);

	glGenRenderbuffers(1, &gmsRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gmsRenderBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, gGameContext.getScreenWidth(), gGameContext.getScreenHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gmsRenderBuffer);


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Initilize Graphics Pipeline: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//FBO for post-processing
	glGenFramebuffers(1, &gFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);

	glGenRenderbuffers(1, &gRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gGameContext.getScreenWidth(), gGameContext.getScreenHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);

	//Generate texture for gFrameBuffer
	if(!gFBOTexture.loadFromPixel(NULL, gGameContext.getScreenWidth(), gGameContext.getScreenHeight(), GL_RGB, false, false)){
		std::cout << "Initilize Graphics Pipeline: Unable to load frame buffer texture" << std::endl;
	}
	else{
		gFBOTexture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gFBOTexture.getTexID(), 0);
		gFBOTexture.unbind();
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Initilize Graphics Pipeline: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	//Load freetype library and font
	FT_Library ftlib;
	FT_Face face;
	if(FT_Init_FreeType(&ftlib)){
		std::cout << "Unable to initilize FreeType library." << std::endl;
		success = false;
	}
	else{
		if(FT_New_Face(ftlib, "/home/harper/Dev/Modulus/assets/Fonts/Open_Sans/OpenSans-Regular.ttf", 0, &face)){
			std::cout << "Unable to load font." << std::endl;
			success = false;
		}
		else{
			//Load characters form font
			FT_Set_Pixel_Sizes(face, 0, textSize);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for(unsigned char c = 0; c < 128; c++){
				//Load glyph
				if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
					std::cout << "Load Media: Error could not load glyph \'" << c << "\'" << std::endl;
					continue;
				}

				//Generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D,	0, GL_RED,
							 face->glyph->bitmap.width, face->glyph->bitmap.rows,
							 0, GL_RED, GL_UNSIGNED_BYTE,
							 face->glyph->bitmap.buffer);

				//Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

				//Store character to map
				Character character{
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			//Deallocate Freetype library and face
			FT_Done_Face(face);
			FT_Done_FreeType(ftlib);
		}
	}

	//Load textures
	if(!playerTex.loadFromImage("../assets/Debug_Sprite.png")){
		std::cout << "Load Media: Unable to load texture" << std::endl;
		success = false;
	}

	if(!backgroundTex.loadFromImage("../assets/Solo_Jazz.png")){
		std::cout << "Load Media: Unable to load texture" << std::endl;
		success = false;
	}

	if(!diffuseTex.loadFromImage("../assets/Lighting_Texture_Diffuse.png")){
		std::cout << "Load Media: Unable to load texture" << std::endl;
		success = false;
	}

	if(!specularTex.loadFromImage("../assets/Lighting_Texture_Specular.png")){
		std::cout << "Load Media: Unable to load texture" << std::endl;
		success = false;
	}


	std::vector<GLuint> iData{
			0,1,2,3
	};

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
	playerVAO.initVAO(playerData, iData, GL_DYNAMIC_DRAW);


	vWidth  = (float)backgroundTex.getWidth();
	vHeight = (float)backgroundTex.getHeight();
	std::vector<GLfloat> backgroundData{
		0,  		vHeight, 0.0f,  0.0f,
		0, 			0, 			 0.0f,  1.0f,
		vWidth, 0, 			 1.0f,  1.0f,
		vWidth, vHeight, 1.0f,  0.0f,
	};
	backgroundVAO.addAttribute(gTexShader.getVertexPosID(), 2, GL_FLOAT);
	backgroundVAO.addAttribute(gTexShader.getTexCoordID(), 2, GL_FLOAT);
	backgroundVAO.initVAO(backgroundData, iData, GL_STATIC_DRAW);


	//Test polygon
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	//float length =  50.f;
	std::vector<GLfloat>testPolygonVAO{
	 -0.5f, -0.5f,  0.5f,	r,g,b,1.f,	0.f, 1.f, //0
	  0.5f, -0.5f,  0.5f,	r,g,b,1.f,	1.f, 1.f, //1
 	  0.5f,  0.5f,  0.5f,	r,g,b,1.f,	1.f, 0.f, //2
 	 -0.5f,  0.5f,  0.5f,	r,g,b,1.f,	0.f, 0.f, //3
	 -0.5f, -0.5f, -0.5f,	r,g,b,1.f,	1.f, 1.f, //4
	  0.5f, -0.5f, -0.5f,	r,g,b,1.f,	0.f, 1.f, //5
	  0.5f,  0.5f, -0.5f,	r,g,b,1.f,	0.f, 0.f, //6
	 -0.5f,  0.5f, -0.5f,	r,g,b,1.f,	1.f, 0.f, //7

	 -0.5f, -0.5f, -0.5f,	r,g,b,1.f,	0.f, 0.f, //8(4e)
	  0.5f, -0.5f, -0.5f,	r,g,b,1.f,	1.f, 0.f, //9(5e)

	  0.5f,  0.5f, -0.5f,	r,g,b,1.f,	1.f, 1.f, //10(6f)
	 -0.5f,  0.5f, -0.5f,	r,g,b,1.f,	0.f, 1.f, //11(7f)
	};
	testPoly.addAttribute(gPolygonShader.getVertexPosID(), 3, GL_FLOAT);
	testPoly.addAttribute(gPolygonShader.getVertexColorID(), 4, GL_FLOAT);
	testPoly.addAttribute(gPolygonShader.getTextureCoordID(), 2, GL_FLOAT);
	testPoly.initVAO(testPolygonVAO, std::vector<GLuint>{0,1,2,2,3,0, //
														 1,5,6,6,2,1, //b
														 7,6,5,5,4,7, //c
														 4,0,3,3,7,4, //d
														 8,9,1,1,0,8, //e
														 3,2,10,10,11,3}, GL_STATIC_DRAW); //f

	/*std::vector<GLfloat>lampVAO{
	 -0.5f, -0.5f,  0.5f, //0
		0.5f, -0.5f,  0.5f, //1
		0.5f,  0.5f,  0.5f, //2
	 -0.5f,  0.5f,  0.5f, //3
	 -0.5f, -0.5f, -0.5f, //4
		0.5f, -0.5f, -0.5f, //5
		0.5f,  0.5f, -0.5f, //6
	 -0.5f,  0.5f, -0.5f, //7
	};

	lampVAO.addAttribute(gLampShader.getVertexPosID(), 3, GL_FLOAT);
	lampVAO.initVAO(lampVAO, std::vector<GLuint>{0,1,2,2,3,0,
																								1,5,6,6,2,1,
																								7,6,5,5,4,7,
																								4,0,3,3,7,4,
																								4,5,1,1,0,4,
																								3,2,6,6,7,3}, GL_STATIC_DRAW);
	*/

	std::vector<GLfloat> textVAO(24);
	gTextVAO.addAttribute(gTextShader.getVertexID(), 4, GL_FLOAT);
	gTextVAO.initVAO(textVAO, iData, GL_DYNAMIC_DRAW);

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
		if(!gPolygonShader.loadProgram()){
			std::cout << "Unable to load polygon shader." << std::endl;
		}
		else{
			std::cout << "PolygonShader recompiled!" << std::endl;

			gPolygonShader.bind();
				gPolygonShader.setProjectionMatrix(gProjectionMatrix);
				gPolygonShader.updateProjectionMatrix();
				gPolygonShader.setViewMatrix(gCamera.viewMatrix);
				gPolygonShader.updateViewMatrix();
				gPolygonShader.setModelMatrix(glm::mat4(1.f));
				gPolygonShader.updateModelMatrix();
			gPolygonShader.unbind();
		}

		//std::cout << "Text input enabled." << std::endl;
		//SDL_StartTextInput();
	}

	//End text inpuyt
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
	//playerMat = playerMat * glm::translate(glm::vec3(playerForce.getXComp(), playerForce.getYComp(), 0.f));
	//playerMat = glm::rotate<float>(playerMat, 0.01f, glm::vec3(0.f,1.f,0.f));

	//gPolygonShader.setLightPosition( glm::vec3(camx, camz, 1000.f));
	//lampMat = glm::translate(lampMat, glm::vec3(camx, (float)gGameContext.getScreenHeight(), camz));
	
}

//Renders objects
void render(){
	
	//Render to multisampled FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gmsFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	gTexShader.bind();

		//Render texture
		backgroundVAO.bind();
		backgroundTex.bind();
			//gTexShader.setModelMatrix(playerMat);
			//gTexShader.updateModelMatrix();
			gTexShader.setViewMatrix(gCamera.viewMatrix);
			gTexShader.updateViewMatrix();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		backgroundTex.unbind();
		backgroundVAO.unbind();

	gPolygonShader.bind();

		testPoly.bind();
		glActiveTexture(GL_TEXTURE0);
		diffuseTex.bind();
		glActiveTexture(GL_TEXTURE1);
		specularTex.bind();
			gPolygonShader.setViewMatrix(gCamera.viewMatrix);
			gPolygonShader.updateViewMatrix();
			for(int i = 0; i < 8; i++){
				gPolygonShader.setModelMatrix(glm::translate( playerMat, glm::vec3(polyPositions[i])));
				gPolygonShader.updateModelMatrix();
				//gPolygonShader.setVec3("light.diffuse", glm::vec3(8.f/i, 8.f/i, 8.f/i));
				gPolygonShader.updateLightPosition();
				glDrawElements( GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, (void*)0);
			}
		testPoly.unbind();
		diffuseTex.unbind();
		glActiveTexture(GL_TEXTURE0);

	gPolygonShader.unbind();

	gTexShader.unbind();

	/*gSpriteShader.bind();

		//Render player
		playerVAO.bind();
		playerTex.bind();
			gSpriteShader.setTextureDimensions(playerTex.getWidth(), playerTex.getHeight());
			gSpriteShader.setModelMatrix(playerMat);
			gSpriteShader.updateModelMatrix();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		playerVAO.unbind();
		playerTex.unbind();

	gSpriteShader.unbind();*/

	/*gLampShader.bind();

		lampVAO.bind();
			//gLampShader.setModelMatrix(lampMat);
			//gLampShader.updateModelMatrix();
			glDrawElements( GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, (void*)0);
		lampVAO.unbind();

	gLampShader.unbind();*/

	//Print text
	if(textTimer.getTime() > 0.05 * 1000.0){
		testText = "FPS: " + std::to_string(dTime.getTime())/*.substr(0, 5)*/;
		textTimer.start();
	}

	//Copy multisampled FBO to post-processing FBO
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gmsFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gFrameBuffer);
	glBlitFramebuffer(0,0,gGameContext.getScreenWidth(),gGameContext.getScreenHeight(),
										0,0,gGameContext.getScreenWidth(),gGameContext.getScreenHeight(),
										GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//Render frame buffer to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	gFBOShader.bind();
		gFrameBufferVAO.bind();
		gFBOTexture.bind();
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	gFBOShader.unbind();

	int x, y = 0;
	gGameContext.mouseCursor.getCoords(x,y);
	if(textTimer.getTime() >= 0.5){
		testText = "FPS: " + std::to_string(1/dTime.getTime());
		textTimer.start();
	}
	testText = std::to_string(bJump->getState()) + " " + std::to_string(gJumpCount) + " " + std::to_string(gJumpTimer.getTime()) + " " + std::to_string(gCamera.position.y);
	RenderText(gTextShader, gTextVAO, testText, 0.f, gGameContext.getScreenHeight() - textSize, 1.f, glm::vec3(1.f, 1.f, 1.f));

	//Update screen
	SDL_GL_SwapWindow(gGameContext.getWindow());

	GLenum error = glGetError();
 	if(error != GL_NO_ERROR){
		std::cout << "Render: Error while rendering: " << gluErrorString(error) << std::endl;
	}
}

//Renders a given string to screen
void RenderText(TextShader &shader, VertArray &mesh, std::string text, float x, float y, float scale, glm::vec3 color){

	shader.bind();

	shader.setTextColor(color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0);

	mesh.bind();

	std::string::const_iterator c;
	for(c = text.begin(); c != text.end(); c++){
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		std::vector<GLfloat> vData{
			xpos,	  ypos + h,	0.f, 0.f,
			xpos,	  ypos,		0.f, 1.f,
			xpos + w, ypos,		1.f, 1.f,

			xpos,	  ypos + h,	0.f, 0.f,
			xpos + w, ypos,		1.f, 1.f,
			xpos + w, ypos + h,	1.f, 0.f,
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		mesh.update(GL_ARRAY_BUFFER, 0, vData);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader.unbind();
	mesh.bind();
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
