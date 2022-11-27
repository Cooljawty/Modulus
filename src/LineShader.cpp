#include <iostream>

#include "LineShader.h"

using namespace Modulus;

LineShader::LineShader(){
	mVertexPosID = 0;
	mVertexColorID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;
	
	mName = "LineShader";
}

bool LineShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader   = loadShaderFromFile(SHADER_PATH "LineShader.vs", GL_VERTEX_SHADER);
	GLuint geometryShader = loadShaderFromFile(SHADER_PATH "LineShader.gs", GL_GEOMETRY_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "LineShader.fs", GL_FRAGMENT_SHADER);

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
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	if(getError("loadProgram")){
		return false;
	}

	return true;
}

void LineShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 3, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertexPos")){
		printProgramLog(mProgramID);
	}
}

void LineShader::setVertexColor(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexColorID, 4, GL_FLOAT, GL_FALSE, stride, data);
	getError("setVertexColor");
}

void LineShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	getError("updateProjectionMatrix");
}

void LineShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	getError("updateViewMatrix");
}

void LineShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	getError("updateModelMatrix");
}
