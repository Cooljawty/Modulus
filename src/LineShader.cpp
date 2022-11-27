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
	if(!compileShaders({ {GL_VERTEX_SHADER,   SHADER_PATH "LineShader.vs"},
					{GL_GEOMETRY_SHADER, SHADER_PATH "LineShader.gs"},
					{GL_FRAGMENT_SHADER, SHADER_PATH "LineShader.fs"} });){ 
		return false;
	}
	
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
