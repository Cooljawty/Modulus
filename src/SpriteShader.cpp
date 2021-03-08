#include <iostream>

#include <SDL_GL.h>

#include <glm/gtc/type_ptr.hpp>

#include "SpriteShader.h"

SpriteShader::SpriteShader(){
	mVertexPosID = 0;

	mTexCoordID = 0;
	mSpriteTextureID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;
}

//Loads and compiles shader
bool SpriteShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader = loadShaderFromFile(SHADER_PATH "SpriteShader.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "SpriteShader.fs", GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(mProgramID);

	//Check for errors
	GLint programSuccess = GL_FALSE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
		std::cout << "Error linking program \"" << mProgramID << "\"" << std::endl;
		printProgramLog(mProgramID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(mProgramID);
		return false;
	}

	//Delete temparary shader references
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	mTexCoordID = getAttributeID("texture");
	mSpriteTextureID = getUniformID("Texture");
	mTextureDimensionsID = getUniformID("TextureDimensions");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

/*
//Enable shader attributes
void SpriteShader::enableAttributes(){

	GLenum error = GL_NO_ERROR;


	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "SpriteShader: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "SpriteShader: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;

	glEnableVertexAttribArray (mSpriteTextureID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "SpriteShader: Error enableing texture unit attribute: " << gluErrorString(error) << std::endl;
		return;
	}
}


//Disable shader attributes
void SpriteShader::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
	glDisableVertexAttribArray(mSpriteTextureID);
 }
*/

//Updates the position matrix
void SpriteShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "SpriteShader: Error setting vertex position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

//Sets the texture verticies
void SpriteShader::setTexCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTexCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error setting texture coordinates: " << gluErrorString(error) << std::endl;
}

//Sets multitexture unit
void SpriteShader::setSpriteTexture(GLuint unit){
	glUniform1i(mSpriteTextureID, unit);
}

//Sets the textures dimensions
void SpriteShader::setTextureDimensions(GLuint w, GLuint h){
	glUniform2ui(mTextureDimensionsID, w, h);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error updating texture dimensions: " << gluErrorString(error) << std::endl;
}

//Updates the projection matrix
void SpriteShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}

//Updates the view matrix
void SpriteShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error updating view matrix: " << gluErrorString(error) << std::endl;
}

//Updates the model matrix
void SpriteShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "SpriteShader: Error updating model matrix: " << gluErrorString(error) << std::endl;
}
