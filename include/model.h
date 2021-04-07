#pragma once
#include "data_types.h"
#include "shader.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Modulus{

	class Model{
		public:
			Model(char* path);

			void loadModel(std::string path);

			void draw(Shader &shader);
		
		public:
			//Stores textures already loaded
			std::vector<Material> texturesLoaded;
			std::vector<Mesh*> mMeshes;
			std::string mDirectory;

		private:
			
			void processNode(aiNode *node, const aiScene *scene);
			Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Material> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
	};
}
