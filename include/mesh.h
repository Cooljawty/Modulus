#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "data_types.h"
#include "shader.h"
#include "VertexArray.h"

namespace Modulus{
	
	using namespace std;

	class Mesh{
		public:
			template<typename vType = float, typename iType = unsigned int>
			Mesh(vector<vType> verticies, 
				 vector<iType> indices,
				 vector<Material> materials,
				 vector<pair<unsigned int, GLenum>> format,
				 GLenum mode = GL_TRIANGLES)
			{
				setup<vType, iType>(verticies, indices, materials, format);
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
			GLenum getDrawMode() const{
				return mDrawMode;
			}
			
			Material* getMaterial(string type){
				for(unsigned int m = 0; m < mMaterials.size(); m++){
					if(mMaterials[m].type == type){
						return &mMaterials[m];
					}
				}

				return nullptr;
			}
			vector<Material>& getMaterials(){
				return mMaterials;	
			}
			
			VertArray& getVertArray(){
				return mVAO;
			}

			// Parameter setting
			void setParameter(const string &name, GLenum type, void* value){
				Parameter param;
				param.name = name;
				param.type = type;
				param.value = value;

				mParameters[name] = param;
			}
			void setParameter(Parameter param){
				mParameters[param.name] = param;
			}

			// Parameter retriveing
			Parameter getParameter( const string &name ){
				return mParameters.at(name);
			}
			vector<Parameter> getParameters(){
				vector<Parameter> params;
				for( auto p : mParameters)
					params.push_back(p.second);
				
				return params;
			}

			void setParameters( Shader& shader){
				for( auto p : mParameters)
					shader.setParameter( p.second.name, p.second.type, p.second.value, false);
			}
			
			bool bindMaterials(Shader &shader){
	 			for(unsigned int m = 0; m < mMaterials.size(); m++){	
					glActiveTexture(GL_TEXTURE0 + m);		
					mMaterials[m].texture->bind();
					if( !shader.setParameter(("material." + mMaterials[m].type), GL_INT, &m, false) )
						return false;
				}
				
				glActiveTexture(GL_TEXTURE0);

				return true;
			}
	 		
			void draw(Shader &shader){
				shader.bind();
				
				if( !bindMaterials(shader) ) return;

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
			template<typename vType, typename iType>
			void setup(	
					vector<vType> verticies, 
					vector<iType> indices, 
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
			
			map< string, Parameter > mParameters;
	};
}
