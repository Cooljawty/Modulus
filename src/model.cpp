#include <iostream>
#include <vector>
#include <string>

#include "model.h"

using namespace Modulus;

Model::Model(){
	mModelMatrix = glm::mat4(1.0);
	mParameters["ModelMatrix"] = { Parameter{"ModelMatrix", GL_FLOAT_MAT4, &mModelMatrix}};
}

Model::~Model(){
	free();
}

void Model::loadModel(std::string path){
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/ | aiProcess_GenNormals);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "Model::LoadModel: Unable to load model \'" << path << "\':" << importer.GetErrorString() << std::endl;
		return;
	}
	
	mDirectory = path.substr(0, path.find_last_of('/')) + "/";

	processNode(scene->mRootNode, scene);
}

void Model::draw(Shader &shader){
	for(auto mesh : mMeshes)
		mesh->draw(shader);
}

void Model::processNode(aiNode *node, const aiScene *scene){
	//Process meshes
	for(unsigned int m = 0; m < node->mNumMeshes; m++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[m]];	
		mMeshes.push_back(processMesh(mesh, scene));
	}
	
	//Process child nodes
	for(unsigned int c = 0; c < node->mNumChildren; c++){
		processNode(node->mChildren[c], scene);
	}
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene){
	std::vector<Vertex> verticies;
	std::vector<unsigned int> indices;
	std::vector<Material> materials;
	
	//Process Verticies
	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		
		Vertex vertex;
		glm::vec3 vector;
		//Position
		vector.x = mesh->mVertices[i].x; 
		vector.y = mesh->mVertices[i].y; 
		vector.z = mesh->mVertices[i].z; 
		vertex.Position = vector;
		//Normal
		vector.x= mesh->mNormals[i].x; 
		vector.y= mesh->mNormals[i].y; 
		vector.z= mesh->mNormals[i].z; 
		vertex.Normal = vector;
		//Texture Coordinates
		if(mesh->mTextureCoords[0]){
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoordinate = vec;
		}
		else{
			vertex.TexCoordinate = glm::vec2(0.f);
		}
		
		verticies.push_back(vertex);
	}
	
	//Process indices
	for(unsigned int f = 0; f < mesh->mNumFaces; f++){
		aiFace face = mesh->mFaces[f];
		for(unsigned int i = 0; i < face.mNumIndices; i++)
			indices.push_back(face.mIndices[i]);
	}
	
	//Process materials
	//TODO: Material type independant loading
	if(mesh->mMaterialIndex >= 0){
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		//Obtain diffues maps
		std::vector<Material> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		materials.insert(materials.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		//Obtain specular maps
		std::vector<Material> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		materials.insert(materials.end(), specularMaps.begin(), specularMaps.end());
	 }	

	auto ptr = new Mesh(verticies, indices, materials, {{3, GL_FLOAT},{3, GL_FLOAT},{2, GL_FLOAT}});
	
	ptr->getParameters().push_back(mParameters["ModelMatrix"]);

	return ptr;
}

//TODO: Send absoute path to Texture::LoadFromFile
//		Make load material texture function add pointer to material instead of copy
std::vector<Material> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName){
	
	std::vector<Material> materials;
	
	for(unsigned int t = 0; t < material->GetTextureCount(type); t++){	
		aiString str;
		material->GetTexture(type, t, &str);
		
		//Flags previously loaded textures
		bool loaded = false;
		for(unsigned int i = 0; i < texturesLoaded.size(); i++){
 			if(std::strcmp(texturesLoaded[i]->path.data(), str.C_Str()) == 0){
				materials.push_back(*texturesLoaded[i]);
				loaded = true;
				break;
			}
		
		}
		//Load new texture
		if(!loaded){
			Material* material = new Material;
			material->texture = new Texture;

			if(!material->texture->loadFromImage( mDirectory + std::string(str.C_Str()) )){
				std::cout << "Model::Load Material Textures: Unable to load texture \'" 
						  << mDirectory + "/" + std::string(str.C_Str()) << "\'" << std::endl;
			}
			
			material->type = typeName;
			//std::cout << mDirectory + "/" + std::string(str.C_Str()) << std::endl;
			material->path = str.C_Str();
			materials.push_back(*material);
			texturesLoaded.push_back(material);
 		}	
 	}	
	return materials;
}

void Model::free(){

	mMeshes.clear();
	
	texturesLoaded.clear();
}
