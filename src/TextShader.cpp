#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#define NO_SDL_GLEXT //SDL_opengl conflicts w/ glew.h without definition
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/gtc/type_ptr.hpp>

#include "TextShader.h"

TextShader::TextShader(){
	mVertexID = 0;

	mTextColorID = 0;
	mProjectionMatrixID = 0;
}

//Loads and compiles shader
bool TextShader::loadProgram(){

	//Generate program
	mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = loadShaderFromFile("../shaders/TextShader.vs", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile("../shaders/TextShader.fs", GL_FRAGMENT_SHADER);

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
	mVertexID = getAttributeID("vertex");
	mTextColorID = getUniformID("TextColor");
	mProjectionMatrixID = getUniformID("ProjectionMatrix");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextShader: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

//Enable shader attributes
void TextShader::enableAttributes(){

	GLenum error = GL_NO_ERROR;

	glEnableVertexAttribArray(mVertexID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextShader: Error enableing vertex: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTextColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextShader: Error enabaling text color attribute: " << gluErrorString(error) << std::endl;
}

//Disable shader attributes
void TextShader::disableAttributes(){
	glDisableVertexAttribArray(mVertexID);
	glDisableVertexAttribArray(mTextColorID);
 }

//Sets the position and texture coordinate verticies
void TextShader::setVertex(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexID, 4, GL_FLOAT, GL_FALSE, stride, data);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "TextShader: Error setting vertex attribute: " << gluErrorString(error) << std::endl;
		printProgramLog(mProgramID);
	}
}

//Sets the text color
void TextShader::setTextColor(GLfloat r, GLfloat g, GLfloat b){
	glUniform3f(mTextColorID, r, g, b);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextShader: Error setting text color: " << gluErrorString(error) << std::endl;
}

//Updates the projection matrix
void TextShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "TextShader: Error updating projection matrix: " << gluErrorString(error) << std::endl;
}
