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
			Model();
			~Model();

			void loadModel(std::string path);

			void draw(Shader &shader);
			
			void free();
		public:
			std::vector<Mesh*> mMeshes;
			std::string mDirectory;
			
			glm::mat4 getModelMatrix(){ return mModelMatrix; }
			void setModelMatrix(glm::mat4 mat){ mModelMatrix = mat; }

		private:
			
			//Matrix for position, orientaion, and scaling
			glm::mat4 mModelMatrix;
			//Stores loaded textures for reuse
			std::vector<Material*> texturesLoaded;
			
			void processNode(aiNode *node, const aiScene *scene);
			Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Material> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
	};
}
