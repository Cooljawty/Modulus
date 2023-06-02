/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.		  
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "textureshadertest.h"

using namespace Modulus;

TextureShaderTest::TextureShaderTest(){
	mPositionID = 0;
	mProjectionID = 0;

}

bool TextureShaderTest::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = loadShaderFromFile("../shaders/TextureTest.vs", GL_VERTEX_SHADER);

	//Check for compilation errors
	if(vertexShader == 0){
		glDeleteShader(mProgramID);
		mProgramID = 0;
		return false;
	}

	//Attach shader to program
	glAttachShader(mProgramID, vertexShader);

	//Create fragment shader
	GLuint fragmentShader = loadShaderFromFile("../shaders/TextureTest.fs", GL_FRAGMENT_SHADER);

	//Check for compilation errors
	if(fragmentShader == 0){
		glDeleteShader(mProgramID);
		mProgramID = 0;
		return false;
	}

	//Attach shader to program
	glAttachShader(mProgramID, fragmentShader);

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

	mPositionID = glGetAttribLocation(mProgramID, "position");
	if(mPositionID == -1)
		std::cout << "position" << " is not a valid glsl program variable" << std::endl;

	mProjectionID = glGetUniformLocation(mProgramID, "Projection");
	if(mProjectionID == -1)
		std::cout << "Projection" << " is not a valid glsl program variable" << std::endl;

	mModelViewID = glGetUniformLocation(mProgramID, "ModelView");
	if(mModelViewID == -1)
		std::cout << "ModelView" << " is not a valid glsl program variable" << std::endl;

	//Get varible IDs
	return true;
}

void TextureShaderTest::enableAttributes(){

	GLenum error = GL_NO_ERROR;

	glEnableVertexAttribArray(mPositionID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShader: Error enableing position attribute: " << gluErrorString(error) << std::endl;
		return;
	}
}

void TextureShaderTest::disableAttributes(){
	glDisableVertexAttribArray(mPositionID);
}

void TextureShaderTest::setPosition(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mPositionID, 2, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextureShaderTest: Error setting position: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

void TextureShaderTest::updateProjection(){
	glUniformMatrix4fv(mProjectionID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}

void TextureShaderTest::updateModelView(){
	glUniformMatrix4fv(mModelViewID, 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextureShader: Error updating model view matrix: " << gluErrorString(error) << std::endl;
}
