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
	if(!compileShaders({ {GL_VERTEX_SHADER,   SHADER_PATH "LampShader.vs"},
					 {GL_FRAGMENT_SHADER, SHADER_PATH "LampShader.fs"} });){ 
		return false;
	}

	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	mLightColorID = getAttributeID("lightColor");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");
	mViewMatrixID = getUniformID("ViewMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	if(getError("loadProgram")){
		return false;
	}

	return true;
}

//Updates the position matrix
void LampShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 3, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertexPos")) printProgramLog(mProgramID);
	
}

void LampShader::updateLightColor(){
	glUniform3fv(mLightColorID, 1, glm::value_ptr(mLightColor));
	getError("updateLightColor");
}

//Updates the projection matrix
void LampShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	getError("updateProjectionMatrix");
}

//Updates the view matrix
void LampShader::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	getError("updateViewMatrix");
}

//Updates the model matrix
void LampShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	getError("updateModelMatrix");
}
