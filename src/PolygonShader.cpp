#include <iostream>

#include "PolygonShader.h"

using namespace Modulus;

PolygonShader::PolygonShader(){
	mVertexPosID = 0;
	mVertexColorID = 0;
	mTextureCoordID = 0;

	mLightPositionID = 0;
	mViewPositionID = 0;

	mPVMatrixID = 0;
	mModelMatrixID = 0;

	mName = "PolygonShader";
}

bool PolygonShader::loadProgram(){
	
	if(!compileShaders({ {GL_VERTEX_SHADER, SHADER_PATH "PolygonShader.vs"},
					 {GL_FRAGMENT_SHADER, SHADER_PATH "BasicLightingShader.fs"} }) ){ 
		return false;
	}
	
	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	//mVertexColorID = getAttributeID("color");
	mTextureCoordID = getAttributeID("texcoord");
	mLightPositionID = getUniformID("light.position");
	mViewPositionID = getUniformID("viewPosition");
	mPVMatrixID = getUniformID("PVMatrix");
	mModelMatrixID = getUniformID("ModelMatrix");

	if(getError("loadProgram")){
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
	if(getError("setVertexPos")){
		printProgramLog(mProgramID);
	}
}

void PolygonShader::setVertexColor(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexColorID, 4, GL_FLOAT, GL_FALSE, stride, data);
	getError("setVertexColor");
}

void PolygonShader::setTextureCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTextureCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setTextureCoord")){
		printProgramLog(mProgramID);
	}
}

void PolygonShader::updateLightPosition(){
	glUniform3fv(mLightPositionID, 1, glm::value_ptr(mLightPosition));
	getError("updateLightPosition");
}

void PolygonShader::updateViewPosition(){
	glUniform3fv(mViewPositionID, 1, glm::value_ptr(mViewPosition));
	getError("updateViewPosition");
}

void PolygonShader::updatePVMatrix(){
	glUniformMatrix4fv(mPVMatrixID, 1, GL_FALSE, glm::value_ptr(mPVMatrix));
	getError("updatePVMatrix");
}

void PolygonShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	getError("updateModelMatrix");
}

void PolygonShader::setShininess(float shineiness){
	mShininess = shineiness;
	mParameters["material.shininess"] = {GL_FLOAT, &mShininess};
}

void PolygonShader::setLightAmbiant(float x, float y, float z){
	mLightAmbiant = glm::vec3(x, y, z);
	mParameters["light.ambiant"] = {GL_FLOAT_VEC3, &mLightAmbiant};
}
void PolygonShader::setLightDiffuse(float x, float y, float z){
	mLightDiffuse = glm::vec3(x, y, z);
	mParameters["light.diffuse"] = {GL_FLOAT_VEC3, &mLightDiffuse};
}
void PolygonShader::setLightSpecular(float x, float y, float z){
	mLightSpecular = glm::vec3(x, y, z);
	mParameters["light.specular"] = {GL_FLOAT_VEC3, &mLightSpecular};
}
