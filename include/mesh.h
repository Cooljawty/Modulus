#pragma once
#include "data_types.h"
#include "shader.h"
#include "VertexArray.h"

namespace Modulus{
	
	class Mesh{
		public:
			Mesh(std::vector<Vertex> verticies, 
				 std::vector<unsigned int> indices,
				 std::vector<Material> materials);
			
			~Mesh();

			void draw(Shader &shader);
			
			void free();
		
		private:
			void setup(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials);
			
		public:
			std::vector<unsigned int> mIndices;
			std::vector<Material> mMaterials;
			VertArray mVAO;

	};
}
