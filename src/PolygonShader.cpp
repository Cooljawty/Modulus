#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#define NO_SDL_GLEXT //SDL_opengl conflicts w/ glew.h without definition
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/gtc/type_ptr.hpp>

#include "PolygonShader.h"

PolygonShader::PolygonShader(){
	mVertexPosID = 0;
	mVertexColorID = 0;
	mTextureCoordID = 0;

	mLightPositionID = 0;
	mViewPositionID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;
}

bool PolygonShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader   = loadShaderFromFile("../shaders/PolygonShader.vs", GL_VERTEX_SHADER);
	GLuint geometryShader = loadShaderFromFile("../shaders/PolygonShader.gs", GL_GEOMETRY_SHADER);
	GLuint fragmentShader = loadShaderFromFile("../shaders/BasicLightingShader.fs", GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(mProgramID);

	//Check for errors
	GLint programSuccess = GL_FALSE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
		std::cout << "Error linking program \"" << mProgramID << "\"" << std::endl;
		printProgramLog(mProgramID);
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(mProgramID);
		return false;
	}

	//Delete temparary shader references
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	mVertexColorID = getAttributeID("color");
	mTextureCoordID = getAttributeID("texcoord");
	mLightPositionID = getUniformID("light.position");
	mViewPositionID = getUniformID("viewPosition");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader: Error compiling texture shader: " << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

void PolygonShader::enableAttributes(){

	//GLenum error = GL_NO_ERROR;

	/*
	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;
	*/


}

void PolygonShader::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
 }

void PolygonShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 3, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader: Error setting vertex position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

void PolygonShader::setVertexColor(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexColorID, 4, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error setting vertexs' color: " << gluErrorString(error) << std::endl;
}

void PolygonShader::setTextureCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTextureCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader: Error setting texture coordinates: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

void PolygonShader::updateLightPosition(){
	glUniform3fv(mLightPositionID, 1, glm::value_ptr(mLightPosition));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating light position: " << gluErrorString(error) << std::endl;
}

void PolygonShader::updateViewPosition(){
	glUniform3fv(mViewPositionID, 1, glm::value_ptr(mViewPosition));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating view position: " << gluErrorString(error) << std::endl;
}

void PolygonShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}

void PolygonShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating view matrix: " << gluErrorString(error) << std::endl;
}

void PolygonShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating model matrix: " << gluErrorString(error) << std::endl;
}
