#pragma once

#include <glm/glm.hpp>
#include "shader.h"

class TextureShaderTest : public Shader{
	
	public:
	TextureShaderTest();

	bool loadProgram();

	void enableAttributes();
	void disableAttributes();
	
	void setPosition(GLsizei, const GLvoid*);
	
	void setProjection(glm::mat4 matrix){ mProjectionMatrix = matrix; }
	void updateProjection();

	void setModelView(glm::mat4 matrix){ mModelViewMatrix = matrix; }
	void updateModelView();

	private:
	
	GLuint mPositionID;

	GLuint mProjectionID;
	glm::mat4 mProjectionMatrix;
	
	GLuint mModelViewID;
	glm::mat4 mModelViewMatrix;
};
