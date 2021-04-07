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
		GLint getTextureCoordID(){ return mTextureCoordID; }
		GLint getProjectionMatrixID(){ return mProjectionMatrixID; }
		GLint getViewMatrixID(){ return mViewMatrixID; }
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
		GLint mProjectionMatrixID;
		GLint mViewMatrixID;
		GLint mModelMatrixID;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mModelMatrix;
};
