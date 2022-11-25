#include <iostream>

#include "FBOShader.h"

using namespace Modulus;

FBOShader::FBOShader(){
	mVertexPosID = 0;
	mTextureCoordID = 0;

	mName = "FBOShader";
}

//Loads and compiles shader
bool FBOShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader =	loadShaderFromFile(SHADER_PATH "FBOTextureShader.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "FBOTextureShader.fs", GL_FRAGMENT_SHADER);

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
	mTextureCoordID = getAttributeID("texCoords");
	mTextureID = getUniformID("screenTexture");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

//Enable shader attributes
void FBOShader::enableAttributes(){

	//GLenum error = GL_NO_ERROR;

	/*
	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FBOShader: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FBOShader: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;
	*/


}

//Disable shader attributes
void FBOShader::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
 }

//Updates the position matrix
void FBOShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FBOShader: Error setting vertex position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

//Sets the color verticies
void FBOShader::setTextureCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTextureCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error setting texture coordinates: " << gluErrorString(error) << std::endl;
}

void FBOShader::setTexture(GLuint tex){
	glUniform1i(mTextureID, tex);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error setting texture: " << gluErrorString(error) << std::endl;
}
