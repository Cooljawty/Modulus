#pragma once
#include "SDL_GL.h"
#include "shader.h"

using namespace Modulus;
class PolygonShader2D : public Shader {
	public:
		PolygonShader2D();

		//Loads and compiles program
		bool loadProgram();

		//Enable shader attributes
		void enableAttributes();

		//Disable shader attributes
		void disableAttributes();

		//Sets the position verticies
		void setVertexPos(GLsizei stride, const GLvoid* data);

		//Sets the texture verticies
		void setVertexColor(GLsizei stride, const GLvoid* data);

		//Sets the projection matrix to given matrix
		void setProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix; }
		//Updates the projection matrix
		void updateProjectionMatrix();
		//Multiplies projectiom matrix by a given matrix
		void leftMultiplyProjectionMatrix(glm::mat4 matrix){ mProjectionMatrix = matrix * mProjectionMatrix; }

		//Sets the view matrix to given matrix
		void setViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix; }
		//Updates the model view matrix
		void updateViewMatrix();
		//Multiplies model view matrix by a given matrix
		void leftMultiplyViewMatrix(glm::mat4 matrix){ mViewMatrix = matrix * mViewMatrix; }

		//Sets the model matrix to given matrix
		void setModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix; }
		//Updates the model view matrix
		void updateModelMatrix();
		//Multiplies model view matrix by a given matrix
		void leftMultiplyModelMatrix(glm::mat4 matrix){ mModelMatrix = matrix * mModelMatrix; }

		//Returns attribute IDs
		GLint getVertexPosID(){ return mVertexPosID; }
		GLint getVertexColorID(){ return mVertexColorID; }
		GLint getProjectionMatrixID(){ return mProjectionMatrixID; }
		GLint getViewMatrixID(){ return mViewMatrixID; }
		GLint getModelMatrixID(){ return mModelMatrixID; }

	private:
		//Vertex position attribute
		GLint mVertexPosID;

		//Vertex color
		GLint mVertexColorID;

		//Projection matrix
		glm::mat4 mProjectionMatrix;
		GLint mProjectionMatrixID;

		//View matrix
		glm::mat4 mViewMatrix;
		GLint mViewMatrixID;

		//Model matrix
		glm::mat4 mModelMatrix;
		GLint mModelMatrixID;
};
