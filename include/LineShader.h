#pragma once
#include "SDL_GL.h"
#include "shader.h"

using namespace Modulus;

class LineShader : public Shader {
	public:
		LineShader();

		//Loads and compiles program
		bool loadProgram();

		//Sets attributes
		void setVertexPos(GLsizei stride, const GLvoid* data);
		void setVertexColor(GLsizei stride, const GLvoid* data);

		//Uniform matrix functions
		void setProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix; }
		void updateProjectionMatrix();
		void leftMultiplyProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix * mProjectionMatrix; }

		void setViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix; }
		void updateViewMatrix();
		void leftMultiplyViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix * mViewMatrix; }

		void setModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix; }
		void updateModelMatrix();
		void leftMultiplyModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix * mModelMatrix; }

		//Returns attribute IDs
		GLint getVertexPosID(){ return mVertexPosID; }
		GLint getVertexColorID(){ return mVertexColorID; }
		GLint getProjectionMatrixID(){ return mProjectionMatrixID; }
		GLint getViewMatrixID(){ return mViewMatrixID; }
		GLint getModelMatrixID(){ return mModelMatrixID; }

	private:
		GLint mVertexPosID;
		GLint mVertexColorID;

		//Projection matrix
		GLint mProjectionMatrixID;
		GLint mViewMatrixID;
		GLint mModelMatrixID;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mModelMatrix;
};
