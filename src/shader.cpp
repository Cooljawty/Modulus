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

bool Shader::compileShaders(std::vector<std::tuple<GLenum, std::string>> shaders){
	using namespace std;

	mProgramID = glCreateProgram();

	vector<GLuint> shaderIDs(shaders.size());
	for( size_t s = 0; s < shaders.size(); s++){
		shaderIDs[s] =
			//Source strings will begin with version on extension
			( get<1>(shaders[s]).find("#version") == 0 ||
			  get<1>(shaders[s]).find("#extension") == 0) ? 
				loadShaderFromString( get<1>(shaders[s]), get<0>(shaders[s])) :
				loadShaderFromFile( get<1>(shaders[s]), get<0>(shaders[s]));
	}

	glLinkProgram(mProgramID);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
		std::cout << "Error linking program \"" << mProgramID << "\"" << std::endl;
		printProgramLog(mProgramID);
		
		for( auto id: shaderIDs )
			glDeleteShader(id);

		glDeleteProgram(mProgramID);
		return false;
	}
	
	for( auto id: shaderIDs )
		glDeleteShader(id);
	
	return true;
}

//Binds the shader program for use
bool Shader::bind(){
	
	//Use the shader
	glUseProgram(mProgramID);

	if(getError("bind")){
		printProgramLog(mProgramID);
		return false;
	}
	
	return true;
}

//Unbinds the shader program after use
void Shader::unbind(){

	//Use default program
	glUseProgram(0);
	
	getError("unbind");
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

unsigned int Shader::loadShaderFromString(const std::string shaderStr, GLenum type){

	//Open file
	GLuint shaderID = 0;

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
		std::cout	<< "Shader::loadShaderFromFile: Unable to compile shader \"" << shaderID 
					<< "\" with source \"\"\"\n" << shaderStr << "\n\"\"\""<< std::endl;
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
		std::cout << mName << "::printProgramLog: Name \"" << program << "\" is not a program." << std::endl;
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
		std::cout << mName << "::printShaderLog: Name \"" << shader << "\" is not a shader." << std::endl;
}

//Returns the ID for a given uniform value
GLuint Shader::getUniformID(const std::string name){
	GLint uniform = glGetUniformLocation(mProgramID, name.c_str());
	if(uniform == -1)
		std::cout << mName << "::getUniformID: " << name << " is not a valid glsl program variable." << std::endl;
	return uniform;
}

//Returns the ID for a given attribute
GLuint Shader::getAttributeID(const std::string name){
	GLint attribute = glGetAttribLocation(mProgramID, name.c_str());
	if(attribute == -1)
		std::cout << mName << "::getAttributeID: " << name << " is not a valid glsl program variable." << std::endl;

	return attribute;
}

bool Shader::setParameter(const std::string &name, GLenum type, void* value, bool save){
	switch(type){ //Type deduction
		case GL_BOOL:
			glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), *(GLint*)value);
			break;
		case GL_INT:
			glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), *(GLint*)value);
			break;
		case GL_FLOAT:
			glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), *(GLfloat*)value);
			break;
		case GL_FLOAT_VEC2: 
			glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			break;
		case GL_FLOAT_VEC3:
    		glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			break;
		case GL_FLOAT_VEC4:
    		glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, (GLfloat*)value);
			break;
		case GL_FLOAT_MAT2:
    		glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			break;
		case GL_FLOAT_MAT3:
    		glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			break;
		case GL_FLOAT_MAT4:
    		glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, (GLfloat*)value);
			break;
		default:
			std::cout << mName + "::setParameter: Invalid type" << std::endl;
			return false;
	}

	if(getError("setParameter(\'" + name + "\')")) return false;
	
	if(save) mParameters[name] = {name, type, value};
	return true;
}
		
//Sets all parameters acording to mParameters
void Shader::resetParameters(){
	for(auto p: mParameters){
		Parameter param = std::get<Parameter>(p);
		setParameter(param.name, param.type, param.value);
	}
}

//Deletes the shader
void Shader::freeShader(){
	glDeleteProgram(mProgramID);
}
