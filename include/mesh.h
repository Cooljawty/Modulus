#pragma once
#include "data_types.h"
#include "shader.h"
#include "VertexArray.h"

namespace Modulus{
	
	class Mesh{
		public:
			//Mesh();
			Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials);
			
			~Mesh(){
				for(auto m : mMaterials)
					delete m.texture;
			}

			void draw(Shader &shader);

		public:
			std::vector<Vertex>	mVerticies;
			std::vector<unsigned int> mIndices;
			std::vector<Material> mMaterials;
			VertArray mVAO;

		private:
			void setup(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials);
			
	};
}
