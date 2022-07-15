#pragma once
#include "SDL_GL.h"
#include "data_types.h"

#include <vector>

namespace Modulus{

	class VertArray {
		public:
			VertArray();
			~VertArray();

			//Adds a vertex attribute to the vertex array
			bool addAttribute(GLuint location, GLuint size, GLenum type);

			//Initilizes VAO and VBOs with given data arrays and the buffers usage
			template<typename type>
			void initVAO(std::vector<type> vData, std::vector<GLuint> iData, GLenum usage){

				//Generate vertex array
				glGenVertexArrays(1, &mVAO);
				glBindVertexArray(mVAO);

				//Generate vertex buffer and fill with given vertex data
				glGenBuffers(1, &mVBO);
				glBindBuffer(GL_ARRAY_BUFFER, mVBO);
				glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(type), &vData[0], usage);

				//Generate index buffer and fill with the given indices
				glGenBuffers(1, &mIBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO); 
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.size() * sizeof(GLuint), &iData[0], usage);

				//Define vertex attributes
				std::size_t offset = 0;
				for(unsigned int i = 0; i < mAttribs.size(); i++){
					glVertexAttribPointer(mAttribs[i]->location, mAttribs[i]->size, mAttribs[i]->type, GL_FALSE, mStride, (void*)offset);
					glEnableVertexAttribArray(mAttribs[i]->location);

					//Increase offset
					offset += mAttribs[i]->capacity;
				}

				//Unbind
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			
			//Updates vertex data
			template<typename type>
			void update(GLenum buffer, std::size_t offset, std::vector<type> data){
				glBindBuffer(buffer, mVBO);
				glBufferSubData(buffer, offset, data.size() * sizeof(type), &data[0]);
				glBindBuffer(buffer, 0);
			}

			//Updates buffer objects' data
			template<typename type>
			void updateAttribute(GLuint location, GLenum buffer, std::vector<type> data){
				glBindBuffer(buffer, mVBO);

				//Pointer to attribute to be updated
				VertexAttrib* attribute = nullptr;

				//Find the attribute
				std::size_t offset = 0;
				for(unsigned int i = 0; i < mAttribs.size(); i++){
					if(mAttribs[i]->location == location){
						attribute = mAttribs[i];
						break;
					}

					//Increase offset
					offset += mAttribs[i]->capacity;
				} 

				//If attribute could not found
				if(attribute == nullptr){
					std::cout << "VertArray: UpdateAttribute: Vertex attribute " << location << " could not be found" << std::endl;
					return;
				} 

				//Update attribute
				unsigned int pos = 0;
				while(pos < data.size()){
					glBufferSubData(buffer, offset, attribute->capacity, &data[pos]);

					//Increase offset
					offset += mStride;

					//Incriment data position
					pos += attribute->size;
				} 

				glBindBuffer(buffer, 0);
			}

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
