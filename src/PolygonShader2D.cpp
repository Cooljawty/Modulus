#include <iostream>

#include "PolygonShader2D.h"

using namespace Modulus;

PolygonShader2D::PolygonShader2D(){
	mVertexPosID = 0;

	mVertexColorID = 0;

	mProjectionMatrixID = 0;
	mViewMatrixID = 0;
	mModelMatrixID = 0;
	
	mName = "PolygonShader2D";
}

//Loads and compiles shader
bool PolygonShader2D::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create shaders
	GLuint vertexShader = loadShaderFromFile(SHADER_PATH + "PolygonShader2D.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile(SHADER_PATH + "PolygonShader2D.fs", GL_FRAGMENT_SHADER);

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
	mVertexPosID = getAttributeID(mProgramID, "position");
	mVertexColorID = getAttributeID(mProgramID, "color");
	mProjectionMatrixID = getUniformID(mProgramID, "ProjectionMatrix");
	mViewMatrixID = getUniformID(mProgramID, "ViewMatrix");
	mModelMatrixID = getUniformID(mProgramID, "ModelMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader2D: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

//Enable shader attributes
void PolygonShader2D::enableAttributes(){

	//GLenum error = GL_NO_ERROR;

	/*
	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader2D: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "PolygonShader2D: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "PolygonShader2D: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;
	*/


}

//Disable shader attributes
void PolygonShader2D::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
 }

//Updates the position matrix
void PolygonShader2D::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 2, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertexPos")){
		printProgramLog(mProgramID);
	}
}

//Sets the color verticies
void PolygonShader2D::setVertexColor(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexColorID, 4, GL_FLOAT, GL_FALSE, stride, data);
	getError("setVertexColor");
}

//Updates the projection matrix
void PolygonShader2D::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	getError("updateProjectionMatrix");
}

//Updates the view matrix
void PolygonShader2D::updateViewMatrix(){
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
	getError("updateViewMatrix");
}

//Updates the model matrix
void PolygonShader2D::updateModelMatrix(){
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
	getError("updateModelMatrix");
}
