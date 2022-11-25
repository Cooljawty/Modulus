#include <iostream>

#include "TextureShader.h"

using namespace Modulus;

TextureShader::TextureShader(){
	mVertexPosID = 0;

	mTexCoordID = 0;
	mTexUnitID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;

	mName = "TextureShader";
}

//Loads and compiles shader
bool TextureShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader = loadShaderFromFile(SHADER_PATH "textureVertexShader.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "textureFragmentShader.fs", GL_FRAGMENT_SHADER);

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
	mTexUnitID = getUniformID("material.texture");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

//Enable shader attributes
void TextureShader::enableAttributes(){

	GLenum error = GL_NO_ERROR;

	/*
	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShader: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShader: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;
	*/

	glEnableVertexAttribArray (mTexUnitID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShader: Error enableing texture unit attribute: " << gluErrorString(error) << std::endl;
		return;
	}
}

//Disable shader attributes
void TextureShader::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
	glDisableVertexAttribArray(mTexUnitID);
 }

//Updates the position matrix
void TextureShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShader: Error setting vertex position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

//Sets the texture verticies
void TextureShader::setTexCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTexCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error setting texture coordinates: " << gluErrorString(error) << std::endl;
}

//Sets multitexture unit
void TextureShader::setTexUnit(GLuint unit){
	glUniform1i(mTexUnitID, unit);
}

//Updates the projection matrix
void TextureShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}

//Updates the view matrix
void TextureShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error updating view matrix: " << gluErrorString(error) << std::endl;
}

//Updates the model matrix
void TextureShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error updating model matrix: " << gluErrorString(error) << std::endl;
}
