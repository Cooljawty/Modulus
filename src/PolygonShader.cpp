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
}

bool PolygonShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader   = loadShaderFromFile(SHADER_PATH "PolygonShader.vs", GL_VERTEX_SHADER);
	//GLuint geometryShader = loadShaderFromFile(SHADER_PATH "PolygonShader.gs", GL_GEOMETRY_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH "BasicLightingShader.fs", GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(mProgramID);

	//Check for errors
	GLint programSuccess = GL_FALSE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
		std::cout << "Error linking program \"" << mProgramID << "\"" << std::endl;
		printProgramLog(mProgramID);
		glDeleteShader(vertexShader);
		//glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(mProgramID);
		return false;
	}

	//Delete temparary shader references
	glDeleteShader(vertexShader);
	//glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	//mVertexColorID = getAttributeID("color");
	mTextureCoordID = getAttributeID("texcoord");
	mLightPositionID = getUniformID("light.position");
	mViewPositionID = getUniformID("viewPosition");
	mPVMatrixID = getUniformID("PVMatrix");
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

void PolygonShader::updatePVMatrix(){
	glUniformMatrix4fv(mPVMatrixID, 1, GL_FALSE, glm::value_ptr(mPVMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating projection/view matrix: " << gluErrorString(error) << std::endl;
}

void PolygonShader::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader: Error updating model matrix: " << gluErrorString(error) << std::endl;
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
