#pragma once

#include <glm/glm.hpp>
#include "shader.h"

class SpriteShader : public Shader {
	public:
		SpriteShader();

		//Loads and compiles program
		bool loadProgram();
		
		//Enable shader attributes
		//void enableAttributes();
		
		//Disable shader attributes
		//void disableAttributes();

		//Sets the position verticies 
		void setVertexPos(GLsizei stride, const GLvoid* data);

		//Sets the texture verticies
		void setTexCoord(GLsizei stride, const GLvoid* data);
		
		//Sets the texture color
		void setTexColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		
		//Sets the sprite's texture
		void setSpriteTexture(GLuint unit);
		
		//Sets the textures dimensions
		void setTextureDimensions(GLuint w, GLuint h);

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
		GLuint getVertexPosID(){ return mVertexPosID; }
		GLuint getTexCoordID(){ return mTexCoordID; }
		GLuint getSpriteTextureID(){ return mSpriteTextureID; }
		GLuint getTextureDimensionsID(){return mTextureDimensionsID;}
		GLuint getProjectionMatrixID(){ return mProjectionMatrixID; }
		GLuint getViewMatrixID(){ return mViewMatrixID; }
		GLuint getModelMatrixID(){ return mModelMatrixID; }

	private:	
		//Vertex position attribute
		GLuint mVertexPosID;
		
		//Texture coordinate attribute
		GLuint mTexCoordID;

		//Texture unit attribute
		GLuint mSpriteTextureID;
		
		//Texture dimensions
		GLuint mTextureDimensionsID;

		//Projection matrix
		glm::mat4 mProjectionMatrix;
		GLuint mProjectionMatrixID;

		//View matrix
		glm::mat4 mViewMatrix;
		GLuint mViewMatrixID;
		
		//Model matrix
		glm::mat4 mModelMatrix;
		GLuint mModelMatrixID;
};

