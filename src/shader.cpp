#include <iostream>
#include <fstream>

#include "shader.h"

using namespace Modulus;

Shader::Shader(){
	mProgramID = 0;
}

Shader::~Shader(){
	freeShader();
}

//Binds the shader program for use
bool Shader::bind(){
	
	//Use the shader
	glUseProgram(mProgramID);

	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "Shader::bind: Error binding shader. " << gluErrorString(error) << std::endl;
		return false;
	}
	
	return true;
}

//Unbinds the shader program after use
void Shader::unbind(){

	//Use default program
	glUseProgram(0);
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "Shader::bind: Error unbinding shader. " << gluErrorString(error) << std::endl;
	}
}

//Loads and compiles a GLSL file
unsigned int Shader::loadShaderFromFile(const std::string file, GLenum type){

	//Open file
	GLuint shaderID = 0;
	std::string shaderStr;
	std::ifstream sourceFile(file.c_str());

	if(!sourceFile){
		std::cout << "Shader::loadShaderFromFile: Unable to open file \"" + file + "\"" << std::endl;
		return 0;
	}

	//Load source to string
	shaderStr.assign( (std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());

	//Create shader ID
	shaderID = glCreateShader(type);

	//Compile shader
	const GLchar* shaderSource = shaderStr.c_str();
	glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(shaderID);

	//Check if successfuly compiled
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
	if(shaderCompiled != GL_TRUE){
		std::cout << "Shader::loadShaderFromFile: Unable to compile shader \"" << shaderID << "\" at source" << file << std::endl;
		printShaderLog(shaderID);
		glDeleteShader(shaderID);
		shaderID = 0;
	}


	//Check for compilation errors
	if(shaderID == 0){
		glDeleteShader(mProgramID);
		mProgramID = 0;
		return 0;
	}

	//Attach shader to program
	glAttachShader(mProgramID, shaderID);

	return shaderID;
}

//Prints errors with the shader or the log
void Shader::printProgramLog(unsigned int program) const {

	//Check if valid shader
	if(glIsProgram(program)){

		//Get the info log length
		int logLength = 0;
		int maxLength = logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Print info log
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, &logLength, infoLog);
		if(logLength > 0) std::cout << infoLog << std::endl;

		//Deallocate string
		delete [] infoLog;
	}
	else
		std::cout << "Shader::printProgramLog: Name \"" << program << "\" is not a program." << std::endl;
}

//Prints the shader's shader log
void Shader::printShaderLog(unsigned int shader) const {

	//Check if valid shader
	if(glIsShader(shader)){

		//Get the info log length
		int logLength = 0;
		int maxLength = logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Print info log
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &logLength, infoLog);
		if(logLength > 0) std::cout << infoLog << std::endl;

		//Deallocate string
		delete [] infoLog;
	}
	else
		std::cout << "Shader::printShaderLog: Name \"" << shader << "\" is not a shader." << std::endl;
}

//Returns the ID for a given uniform value
GLuint Shader::getUniformID(const std::string name){
	GLint uniform = glGetUniformLocation(mProgramID, name.c_str());
	if(uniform == -1)
		std::cout << "Shader: " << name << " is not a valid glsl program variable." << std::endl;
	return uniform;
}

//Returns the ID for a given attribute
GLuint Shader::getAttributeID(const std::string name){
	GLint attribute = glGetAttribLocation(mProgramID, name.c_str());
	if(attribute == -1)
		std::cout << "Shader: " << name << " is not a valid glsl program variable." << std::endl;

	return attribute;
}

bool Shader::setParameter(const std::string &name, GLenum type, void* value, bool global){
	switch(type){ //Type deduction
		case GL_BOOL:
			glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), *(GLint*)value);
			if(getError(name)) return false;
			break;
		case GL_INT:
			glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), *(GLint*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT:
			glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), *(GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_VEC2: 
			glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_VEC3:
    		glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_VEC4:
    		glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_MAT2:
    		glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_MAT3:
    		glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		case GL_FLOAT_MAT4:
    		glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			if(getError(name)) return false;
			break;
		default:
			std::cout << "Shader::setParameter: Invalid type" << std::endl;
			return false;
	}
	if(global) mParameters[name] = {type, &value};
	return true;
}
		
//Sets all parameters acording to mParameters
void Shader::resetParameters(){
	for(auto p: mParameters){
		auto name  = std::get<0>(p);
		auto type  = std::get<0>(std::get<1>(p));
		auto value = std::get<1>(std::get<1>(p));
		setParameter(name, type, value);
	}
}
//Deletes the shader
void Shader::freeShader(){
	glDeleteProgram(mProgramID);
}
