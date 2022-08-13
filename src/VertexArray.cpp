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
	VertexAttrib attrib;
	attrib.location = location;
	attrib.size = size;
	attrib.type = type;

	//Find size of attribute in memory
	switch(type){
		case GL_FLOAT:
			attrib.capacity = size * sizeof(GLfloat);
			break;
		case GL_INT:
			attrib.capacity = size * sizeof(GLint);
			break;	
		default:
			std::cout << "VertArray: AddAttrib: Error vertex attribute " << attrib.location << " does not have a type." << std::endl;
			return false;
	}

	//Update stride
	mStride += attrib.capacity;

	//Add to array in order
	for(unsigned int p = 0; p < mAttribs.size(); p++){
		if(location < mAttribs[p].location){
			mAttribs.insert(mAttribs.begin() + p, attrib);
			return true;
		}
	}

	//Add to end of mAttribs
	mAttribs.push_back(attrib);

	return true;
}

//Returns contentes of index buffer as vertex
std::vector<GLuint> VertArray::getIndexBuffer(){
	bind();

	std::vector<GLuint> indecies(mIBSize / sizeof(GLuint));
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mIBSize, &indecies.front());
	
	unbind();

	return indecies;
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

	mAttribs.clear();

	mStride = 0;
}
