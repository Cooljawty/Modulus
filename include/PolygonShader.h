#pragma once
#include "SDL_GL.h"
#include "shader.h"

using namespace Modulus;

class PolygonShader : public Shader {
	public:
		PolygonShader();

		//Loads and compiles program
		bool loadProgram();

		//Enable shader attributes
		void enableAttributes();

		//Disable shader attributes
		void disableAttributes();

		//Sets attributes
		void setVertexPos(GLsizei stride, const GLvoid* data);
		void setVertexColor(GLsizei stride, const GLvoid* data);
		void setTextureCoord(GLsizei stride, const GLvoid* data);

		//Sets and updates uniforms
		void setLightPosition(glm::vec3 pos){ mLightPosition = pos; }
		void updateLightPosition();
		void setViewPosition(glm::vec3 pos){ mViewPosition = pos; }
		void updateViewPosition();

		//Uniform matrix functions
		void setPVMatrix(glm::mat4 matrix){ mPVMatrix = matrix; }
		void updatePVMatrix();
		void leftMultiplyPVMatrix(glm::mat4 matrix){ mPVMatrix = matrix * mPVMatrix; }

		void setModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix; }
		void updateModelMatrix();
		void leftMultiplyModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix * mModelMatrix; }

		//Returns attribute IDs
		GLint getVertexPosID(){ return mVertexPosID; }
		GLint getVertexColorID(){ return mVertexColorID; }
		GLint getTextureCoordID(){ return mTextureCoordID; }
		GLint getPVMatrixID(){ return mPVMatrixID; }
		GLint getModelMatrixID(){ return mModelMatrixID; }

	private:
		GLint mVertexPosID;
		GLint mVertexColorID;
		GLint mTextureCoordID;

		GLint mLightPositionID;
		glm::vec3 mLightPosition;

		GLint mViewPositionID;
		glm::vec3 mViewPosition;

		//Projection matrix
		GLint mPVMatrixID;
		GLint mModelMatrixID;
		glm::mat4 mPVMatrix;
		glm::mat4 mModelMatrix;
};
