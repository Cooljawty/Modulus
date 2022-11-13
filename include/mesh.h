#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "data_types.h"
#include "shader.h"
#include "VertexArray.h"

namespace Modulus{
	
	using namespace std;

	class Mesh{
		public:
			template<typename vType, typename iType>
			Mesh(vector<vType> verticies, 
				 vector<iType> indices,
				 vector<Material> materials,
				 vector<pair<unsigned int, GLenum>> format,
				 GLenum mode = GL_TRIANGLES)
			{
				setup(verticies, indices, materials, format);
				mDrawMode = mode;
			}			

			Mesh(VertArray &vao,
				 vector<Material> materials,
				 GLenum mode = GL_TRIANGLES)
			{
				setup(vao, materials);
				mDrawMode = mode;
			}

			~Mesh(){
				free();
			}
			
			void setDrawMode(GLenum mode){
				mDrawMode = mode;
			}

	 		void draw(Shader &shader){
				shader.bind();
				
				//Binding materials	
				string number;
	 			for(unsigned int m = 0; m < mMaterials.size(); m++){	
					glActiveTexture(GL_TEXTURE0 + m);		
					mMaterials[m].texture->bind();
					shader.setParameter(("material." + mMaterials[m].type), GL_INT, &m); 	
				}
				
				glActiveTexture(GL_TEXTURE0);
				
				mVAO.bind();
				
				glDrawElements(mDrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);

				mVAO.unbind();
				
				GLenum error = glGetError();
				if(error != GL_NO_ERROR){
					cout << "Mesh::Draw: error while rendering: " << gluErrorString(error) << endl;
				}
			}

			void free(){
				mIndices.clear(); 
				mMaterials.clear();
			}
		
		private:
			template<typename vType>
			void setup(	vector<vType> verticies, 
						vector<unsigned int> indices, 
						vector<Material> materials, 
						vector<pair<unsigned int, GLenum>> format)
			{
				mIndices = indices;
				mMaterials = materials;
				
				for(unsigned int attr = 0; attr < format.size(); attr++)
					mVAO.addAttribute(attr, format[attr].first, format[attr].second);

				mVAO.initVAO<vType>(verticies, indices, GL_STATIC_DRAW);
			}
			
			void setup(VertArray vao, vector<Material> materials){
				mMaterials = materials;
				
				mVAO = vao;

				mIndices = mVAO.getIndexBuffer();
			}
			
			vector<unsigned int> mIndices;
			vector<Material> mMaterials;
			VertArray mVAO;
			
			GLenum mDrawMode;
	};
}
