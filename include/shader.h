#pragma once
#include <SDL_GL.h>

#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <map>

#include "data_types.h"

namespace Modulus {

	using namespace std;

	class Shader{
		public:
			Shader();
			virtual ~Shader();

			//Deletes the shader
			virtual void freeShader();

			//Compiles shaders into program, can be called to make anonymous shader
			bool compileShaders(vector<tuple<GLenum, string>> shaders);

			//Loads and compiles a GLSL file
			unsigned int loadShaderFromFile(const string file, GLenum type);
			
			//Loads and compiles a GLSL string
			unsigned int loadShaderFromString(const string ShaderStr, GLenum type);

			//Binds the shader program for use
			bool bind();

			//Unbinds the shader program after use
			void unbind();

			//Loads shader program indended for setting derived shaders
			virtual bool loadProgram() = 0;
			
			//Returns the program ID
			unsigned int getID(){ return mProgramID; }

			//Uniform setters
			bool setParameter(const string &name, GLenum type, void* value, bool save=true);
			
			//Sets all parameters according to mParameters
			void resetParameters();

		protected:
			//Returns the ID for a given uniform value
			GLuint getUniformID(const string name);

			//Returns the ID for a given attribute
			GLuint getAttributeID(const string name);

			//Prints the program logs
			void printProgramLog(unsigned int id) const;
			void printShaderLog(unsigned int id) const;
			
			//Detects opengl errors and prints relevent information
			virtual bool getError(const string name) const{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					cout << mName << "::" << name << ": Error " << gluErrorString(error) << endl;
					printProgramLog(mProgramID);
					return true;
				}
				return false;
			}
			
			//ID for calling program
			unsigned int mProgramID;
			string mName;

			map< string, Parameter > mParameters;
	};
}
