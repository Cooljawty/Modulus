#include <iostream>

#include "TextShader.h"

using namespace Modulus;

TextShader::TextShader(){
	mVertexID = 0;
	mTexCoordsID = 0;

	mTextColorID = 0;
	mProjectionMatrixID = 0;
	
	mName = "TextShader";
}

//Loads and compiles shader
bool TextShader::loadProgram(){
	if(!compileShaders({ {GL_VERTEX_SHADER, SHADER_PATH "TextShader.vs"},
					 {GL_FRAGMENT_SHADER, SHADER_PATH "TextShader.fs"} })){ 
		return false;
	}
	
	//Get varible IDs
	mVertexID = getAttributeID("pos");
	mTexCoordsID = getAttributeID("uv");
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
	glVertexAttribPointer(mVertexID, 2, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertex")){
		printProgramLog(mProgramID);
	}
}
void TextShader::setTexCoords(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTexCoordsID, 2, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertex")){
		printProgramLog(mProgramID);
	}
}

//Sets the text color
void TextShader::setTextColor(GLfloat r, GLfloat g, GLfloat b){
	glUniform3f(mTextColorID, r, g, b);
	getError("setTextColor");
}

//Updates the projection matrix
void TextShader::updateProjectionMatrix(){
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
	getError("updateProjectionMatrix");
}
