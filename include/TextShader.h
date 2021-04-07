#pragma once
#include "SDL_GL.h"
#include "shader.h"

using namespace Modulus;

class TextShader : public Shader {
	public:
		TextShader();

		//Loads and compiles program
		bool loadProgram();
		
		//Enable shader attributes
		void enableAttributes();
		
		//Disable shader attributes
		void disableAttributes();

		//Sets the position and texture coordinate verticies 
		void setVertex(GLsizei stride, const GLvoid* data);

		//Sets the text color
		void setTextColor(GLfloat r, GLfloat g, GLfloat b);
		
		//Sets the projection matrix to given matrix
		void setProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix; }
		
		//Updates the projection matrix
		void updateProjectionMatrix();
		
		//Multiplies projectiom matrix by a given matrix
		void leftMultiplyProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix * mProjectionMatrix; }
		
		//Returns attribute IDs
		GLint getVertexID(){ return mVertexID; }
		GLint getTextColorID(){return mTextColorID; }
		GLint getProjectionMatrixID(){ return mProjectionMatrixID; }

	private:	
		//Vertex position attribute
		GLint mVertexID;
		
		//Text color
		GLint mTextColorID;

		//Projection matrix
		glm::mat4 mProjectionMatrix;
		GLint mProjectionMatrixID;
};
