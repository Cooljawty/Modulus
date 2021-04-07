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
void Shader::printProgramLog(uint program){

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
void Shader::printShaderLog(uint shader){

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

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(mProgramID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//Deletes the shader
void Shader::freeShader(){
	glDeleteProgram(mProgramID);
}
