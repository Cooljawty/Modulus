#pragma once
#include "SDL_GL.h"
#include "data_types.h"

namespace Modulus{

	class VertArray {
		public:
			VertArray();
			~VertArray();

			//Adds a vertex attribute to the vertex array
			bool addAttribute(GLuint location, GLuint size, GLenum type);

			template<typename type>
			void initVAO(std::vector<type> vData, std::vector<GLuint> iData, GLenum usage);
			
			//Updates vertex data
			template<typename type>
			void update(GLenum buffer, std::size_t offset, std::vector<type> data){
				glBindBuffer(buffer, mVBO);
				glBufferSubData(buffer, offset, data.size() * sizeof(type), &data[0]);
				glBindBuffer(buffer, 0);
			}

			template<typename type>
			void updateAttribute(GLuint location, GLenum buffer, std::vector<type> data);

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
}
