#pragma once

#include <glm/glm.hpp>
#include "shader.h"

class LampShader : public Shader {
	public:
		LampShader();

		//Loads and compiles program
		bool loadProgram();

		//Sets the position verticies
		void setVertexPos(GLsizei stride, const GLvoid* data);

		void setLightColor(glm::vec3 color){ mLightColor = color; }
		void updateLightColor();

		//Projection matrix functions
		void setProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix; }
		void updateProjectionMatrix();
		void leftMultiplyProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix * mProjectionMatrix; }

		//View matrix functions
		void setViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix; }
		void updateViewMatrix();
		void leftMultiplyViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix * mViewMatrix; }

		//Model matrix functions
		void setModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix; }
		void updateModelMatrix();
		void leftMultiplyModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix * mModelMatrix; }

		//Returns attribute and uniform IDs
		GLint getVertexPosID(){ return mVertexPosID; }
		GLint getLightColorID(){ return mLightColorID; }
		GLint getProjectionMatrixID(){ return mProjectionMatrixID; }
		GLint getViewMatrixID(){ return mViewMatrixID; }
		GLint getModelMatrixID(){ return mModelMatrixID; }

	private:
		GLint mVertexPosID;

		GLint mLightColorID;
		glm::vec3 mLightColor;

		//Matricies
		GLint mProjectionMatrixID;
		GLint mViewMatrixID;
		GLint mModelMatrixID;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mModelMatrix;
};
