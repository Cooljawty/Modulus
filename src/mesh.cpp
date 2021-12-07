#include <vector>
#include <string>
#include <iostream>

#include "mesh.h"

using namespace Modulus;

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials){

	setup(verticies, indices, materials);

}

Mesh::~Mesh(){
	free();
}

//Initilizes vertex array with mesh data
void Mesh::setup(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Material> materials){
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

void Mesh::free(){	
	
	mIndices.clear();
	
	mMaterials.clear();
}
