#pragma once
#include <SDL_GL.h>

#include <string>
#include <iostream>
#include <vector>
#include <functional>

namespace Modulus {

	class Shader{
		public:
			Shader();
			virtual ~Shader();

			//Deletes the shader
			virtual void freeShader();

			//Binds the shader program for use
			bool bind();

			//Unbinds the shader program after use
			void unbind();

			//Loads shader program
			virtual bool loadProgram() = 0;

			//Loads and compiles a GLSL file
			unsigned int loadShaderFromFile(const std::string file, GLenum type);

			//Returns the program ID
			unsigned int getID(){ return mProgramID; }

			//Uniform setters
			bool setBool(const std::string &name, bool value) const;
			bool setInt(const std::string &name, int value) const;
			bool setFloat(const std::string &name, float value) const;
			bool setVec2(const std::string &name, const glm::vec2 &value) const;
			bool setVec2(const std::string &name, float x, float y) const;
			bool setVec3(const std::string &name, const glm::vec3 &value) const;
			bool setVec3(const std::string &name, float x, float y, float z) const;
			bool setVec4(const std::string &name, const glm::vec4 &value) const;
			bool setVec4(const std::string &name, float x, float y, float z, float w);
			bool setMat2(const std::string &name, const glm::mat2 &mat) const;
			bool setMat3(const std::string &name, const glm::mat3 &mat) const;
			bool setMat4(const std::string &name, const glm::mat4 &mat) const;
		
		protected:
			//Returns the ID for a given uniform value
			GLuint getUniformID(const std::string name);

			//Returns the ID for a given attribute
			GLuint getAttributeID(const std::string name);

			//Prints the program logs
			void printProgramLog(unsigned int id) const;
			void printShaderLog(unsigned int id) const;
			
			//Detects opengl errors and prints relevent information
			virtual bool getError(const std::string name, std::string shaderName = "Shader") const{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					std::cout << shaderName << ": Error setting " << name << ":" << gluErrorString(error) << std::endl;
					printProgramLog(mProgramID);
					return true;
				}
				return false;
			}

			//ID for calling program
			unsigned int mProgramID;
	};
}
