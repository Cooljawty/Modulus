#include <iostream>

#include "FBOShader.h"

using namespace Modulus;

FBOShader::FBOShader(){
	mVertexPosID = 0;
	mTextureCoordID = 0;

	mName = "FBOShader";
}

//Loads and compiles shader
bool FBOShader::loadProgram(){
	if(!compileShaders({ {GL_VERTEX_SHADER,  SHADER_PATH "FBOTextureShader.vs" },
					 {GL_FRAGMENT_SHADER, SHADER_PATH "FBOTextureShader.fs"}});){ 
		return false;
	}
	
	//Get varible IDs
	mVertexPosID = getAttributeID("position");
	mTextureCoordID = getAttributeID("texCoords");
	mTextureID = getUniformID("screenTexture");

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error compileing texture shader: " << gluErrorString(error) << std::endl;

	return true;
}

//Enable shader attributes
void FBOShader::enableAttributes(){

	//GLenum error = GL_NO_ERROR;

	/*
	glEnableVertexAttribArray(mVertexPosID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FBOShader: Error enableing vertex position: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexCoordID);
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FBOShader: Error enableing texture coordinates: " << gluErrorString(error) << std::endl;
		return;
	}

	glEnableVertexAttribArray(mTexColorID);
	error = glGetError();
	if(error != GL_NO_ERROR)
		std::cout << "FBOShader: Error enabaling texture color attribute: " << gluErrorString(error) << std::endl;
	*/


}

//Disable shader attributes
void FBOShader::disableAttributes(){
	//glDisableVertexAttribArray(mVertexPosID);
	//glDisableVertexAttribArray(mTexCoordID);
	//glDisableVertexAttribArray(mTexColorID);
 }

//Updates the position matrix
void FBOShader::setVertexPos(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mVertexPosID, 2, GL_FLOAT, GL_FALSE, stride, data);
	if(getError("setVertexPos")) printProgramLog(mProgramID);
}

//Sets the color verticies
void FBOShader::setTextureCoord(GLsizei stride, const GLvoid* data){
	glVertexAttribPointer(mTextureCoordID, 2, GL_FLOAT, GL_FALSE, stride, data);
	getError("setTextureCoord");
}

void FBOShader::setTexture(GLuint tex){
	glUniform1i(mTextureID, tex);
	getError("setTextureCoord");
}
