#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "data_types.h"
#include "shader.h"
#include "VertexArray.h"

namespace Modulus{
	
	template<typename vType, typename iType>
	class Mesh{
		public:
			Mesh(std::vector<vType> verticies, 
				 std::vector<iType> indices,
				 std::vector<Material> materials,
				 std::vector<std::pair<unsigned int, GLenum>> format){
				setup(verticies, indices, materials, format);
			}
			
			~Mesh(){
				free();
			}

			void draw(Shader &shader){
				shader.bind();
				
				//Binding materials	
				std::string number;
				for(unsigned int m = 0; m < mMaterials.size(); m++){	
					glActiveTexture(GL_TEXTURE0 + m);		
					mMaterials[m].texture->bind();
					shader.setInt(("material." + mMaterials[m].type), m); 	
				}
				
				glActiveTexture(GL_TEXTURE0);
				
				mVAO.bind();
				
				glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

				mVAO.unbind();
				shader.unbind();
				
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					std::cout << "Mesh::Draw: error while rendering: " << gluErrorString(error) << std::endl;
				}
			}

			void free(){
				mIndices.clear(); 
				mMaterials.clear();
			}
		
		private:
			void setup(std::vector<vType> verticies, std::vector<iType> indices, std::vector<Material> materials, std::vector<std::pair<unsigned int, GLenum>> format){
				mIndices = indices;
				mMaterials = materials;
				
				for(unsigned int attr = 0; attr < format.size(); attr++)
					mVAO.addAttribute(attr, format[attr].first, format[attr].second);

				mVAO.initVAO<vType>(verticies, indices, GL_STATIC_DRAW);
			}
			
		public:
			std::vector<iType> mIndices;
			std::vector<Material> mMaterials;
			VertArray mVAO;

	};
}
