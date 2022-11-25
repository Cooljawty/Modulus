#include <iostream>
#include "LampShader.h"

using namespace Modulus;

LampShader::LampShader(){
	mVertexPosID = 0;

	mLightColorID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;
	mName = "LampShader";
}

//Loads and compiles shader
bool LampShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader   = loadShaderFromFile(SHADER_PATH "LampShader.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "LampShader.fs", GL_FRAGMENT_SHADER);

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
	mLightColorID = getAttributeID("lightColor");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "LampShader: Error compiling texture shader: " << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

//Updates the position matrix
void LampShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 3, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "LampShader: Error setting vertex position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

void LampShader::updateLightColor(){
	glUniform3fv(mLightColorID, 1, glm::value_ptr(mLightColor));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "LampShader: Error updating light Color: " << gluErrorString(error) << std::endl;
}

//Updates the projection matrix
void LampShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "LampShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}

//Updates the view matrix
void LampShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "LampShader: Error updating view matrix: " << gluErrorString(error) << std::endl;
}

//Updates the model matrix
void LampShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "LampShader: Error updating model matrix: " << gluErrorString(error) << std::endl;
}
