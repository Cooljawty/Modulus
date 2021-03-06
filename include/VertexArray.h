/* TODO: Template class? */

#pragma once

class VertArray {
	public:
		VertArray();
		~VertArray();

		//Adds a vertex attribute to the vertex array
		bool addAttribute(GLuint location, GLuint size, GLenum type);

		//Initilizes VAO and VBOs with given data arrays
		void initVAO(std::vector<GLfloat> vData, std::vector<GLuint> iData, GLenum usage);

		//Updates vertex data
		void update(GLenum buffer, std::size_t offset, std::vector<GLfloat> data);

		//Updates a given attribute in the VBO
		void updateAttribute(GLuint location, GLenum buffer, std::vector<GLfloat> data);

		//Binds and unbinds VAO for rendering
		void bind();
		void unbind();

		//Deletes all attributes made for the VAO
		void freeAttribs();

	private:

		//Vertex buffer
		GLuint mVBO;
		//Index buffer
		GLuint mIBO;
		//Vertex array
		GLuint mVAO;

		//A attribute in the vertex array
		struct VertexAttrib{
			//Layout location in shader
			GLuint location;
			//Number of components [1-4]
			GLint size;
			//Data type
			GLenum type;
			//Offset in VAO
			std::size_t capacity;
		};

		//List of all vertex attributes
		std::vector<VertexAttrib*> mAttribs;

		//Stride of vertex data when assiging attributes
		GLsizei mStride;
};
