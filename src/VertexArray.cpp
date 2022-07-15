#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "VertexArray.h"

using namespace Modulus;

VertArray::VertArray(){
	mVBO = 0;
	mIBO = 0;
	mVAO = 0;

	mStride = 0;
}
VertArray::~VertArray(){
	freeAttribs();

	mVBO = 0;
	mIBO = 0;
	mVAO = 0;

	mStride = 0;
}

//Adds a vertex attribute to the vertex array
bool VertArray::addAttribute(GLuint location, GLuint size, GLenum type){

	//Create object
	VertexAttrib* attrib = new VertexAttrib;
	attrib->location = location;
	attrib->size = size;
	attrib->type = type;

	//Find size of attribute in memory
	switch(type){
		case GL_FLOAT:
			attrib->capacity = size * sizeof(GLfloat);
			break;
		default:
			std::cout << "VertArray: AddAttrib: Error vertex attribute " << attrib->location << " does not have a type." << std::endl;
			delete attrib;
			return false;
	}

	//Update stride
	mStride += attrib->capacity;

	//Add to array in order
	for(unsigned int p = 0; p < mAttribs.size(); p++){
		if(location < mAttribs[p]->location){
			mAttribs.insert(mAttribs.begin() + p, attrib);
			return true;
		}
	}

	//Add to end of mAttribs
	mAttribs.push_back(attrib);

	return true;
}

//Binds VAO for rendering
void VertArray::bind(){
	glBindVertexArray(mVAO);
}

//Unbinds VAO after rendering
void VertArray::unbind(){
	glBindVertexArray(0);
}

//Deletes all attributes made for the VAO
void VertArray::freeAttribs(){

	//Free attributes and empty vector
	for(auto& attrib : mAttribs)
		delete attrib;
	mAttribs.clear();

	mStride = 0;
}
