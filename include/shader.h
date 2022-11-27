#pragma once
#include <SDL_GL.h>

#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <map>

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
			bool setParameter(const std::string &name, GLenum type, void* value, bool save=true);
			
			//Sets all parameters according to mParameters
			void resetParameters();
		protected:
			bool compileShaders(std::vector<std::tuple<GLenum, std::string>> shaders);
			
			//Returns the ID for a given uniform value
			GLuint getUniformID(const std::string name);

			//Returns the ID for a given attribute
			GLuint getAttributeID(const std::string name);

			//Prints the program logs
			void printProgramLog(unsigned int id) const;
			void printShaderLog(unsigned int id) const;
			
			//Detects opengl errors and prints relevent information
			virtual bool getError(const std::string name) const{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					std::cout << mName << "::" << name << ": Error " << gluErrorString(error) << std::endl;
					printProgramLog(mProgramID);
					return true;
				}
				return false;
			}
			
			//ID for calling program
			unsigned int mProgramID;
			std::string mName;

			std::map< std::string, std::tuple<GLenum, void*>> mParameters;
	};
}
