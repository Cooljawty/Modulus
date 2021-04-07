#include <vector>
#include <string>
#include <iostream>

#include "mesh.h"

using namespace Modulus;

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials){

	setup(verticies, indices, materials);

}


//Initilizes vertex array with mesh data
void Mesh::setup(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials){
	mVerticies = verticies;
	mIndices = indices;
	mMaterials = materials;

	mVAO.addAttribute(0, 3, GL_FLOAT);
	mVAO.addAttribute(1, 3, GL_FLOAT);
	mVAO.addAttribute(2, 2, GL_FLOAT);

	mVAO.initVAO<Vertex>(verticies, indices, GL_STATIC_DRAW);
}

//Draws VAO with materials
void Mesh::draw(Shader &shader){

	shader.bind();
	
	//Binding materials	
	std::string number;
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	unsigned int normalNum = 1;
	for(unsigned int m = 0; m < mMaterials.size(); m++){	
		/*	 if(mMaterials[m].type == "texture_diffuse")  number = std::to_string(diffuseNum++);
		else if(mMaterials[m].type == "texture_specular") number = std::to_string(specularNum++);
		else if(mMaterials[m].type == "texture_normal")	  number = std::to_string(normalNum++);
		else{
			std::cout << "Mesh::draw: " << "Texture " << mMaterials[m].texture.getTexID() 
					  << " has invalid type \'" << mMaterials[m].type << "\'." << std::endl;
			continue;
		
		}*/
		
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
