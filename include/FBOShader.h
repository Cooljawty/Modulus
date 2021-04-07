#pragma once
#include "SDL_GL.h"
#include "shader.h"

using namespace Modulus;

class FBOShader : public Shader {
	public:
		FBOShader();

		//Loads and compiles program
		bool loadProgram();

		//Enable shader attributes
		void enableAttributes();

		//Disable shader attributes
		void disableAttributes();

		//Sets vertex attributes
		void setVertexPos(GLsizei stride, const GLvoid* data);
		void setTextureCoord(GLsizei stride, const GLvoid* data);

		//Sets uniform variables
		void setTexture(GLuint tex);

		//Returns attribute IDs
		GLint getVertexPosID(){ return mVertexPosID; }
		GLint getTextureCoordID(){ return mTextureCoordID; }
		GLint getTextureID(){ return mTextureID; }
		
	private:
		GLint mVertexPosID;
		GLint mTextureCoordID;
		GLint mTextureID;
};
