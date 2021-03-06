#pragma once

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
		void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
	protected:
		//Returns the ID for a given uniform value
		GLuint getUniformID(const std::string name);

		//Returns the ID for a given attribute
		GLuint getAttributeID(const std::string name);

		//Prints the program logs
		void printProgramLog(unsigned int id);
		void printShaderLog(unsigned int id);

		//ID for calling program
		unsigned int mProgramID;
};
