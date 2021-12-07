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

//Initilizes VAO and VBOs with given data arrays and the buffers usage
template<typename type>
void VertArray::initVAO(std::vector<type> vData, std::vector<GLuint> iData, GLenum usage){

	//Generate vertex array
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//Generate vertex buffer and fill with given vertex data
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(type), &vData[0], usage);

	//Generate index buffer and fill with the given indices
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.size() * sizeof(GLuint), &iData[0], usage);

	//Define vertex attributes
	std::size_t offset = 0;
	for(unsigned int i = 0; i < mAttribs.size(); i++){
		glVertexAttribPointer(mAttribs[i]->location, mAttribs[i]->size, mAttribs[i]->type, GL_FALSE, mStride, (void*)offset);
		glEnableVertexAttribArray(mAttribs[i]->location);

		//Increase offset
		offset += mAttribs[i]->capacity;
	}

	//Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
template void VertArray::initVAO<GLfloat>(std::vector<GLfloat>, std::vector<GLuint>, GLenum);
template void VertArray::initVAO<Vertex>(std::vector<Vertex>, std::vector<GLuint>, GLenum);

//Updates buffer objects' data
template<typename type>
void VertArray::updateAttribute(GLuint location, GLenum buffer, std::vector<type> data){
	glBindBuffer(buffer, mVBO);

	//Pointer to attribute to be updated
	VertexAttrib* attribute = nullptr;

	//Find the attribute
	std::size_t offset = 0;
	for(unsigned int i = 0; i < mAttribs.size(); i++){
		if(mAttribs[i]->location == location){
			attribute = mAttribs[i];
			break;
		}

		//Increase offset
		offset += mAttribs[i]->capacity;
	} 

	//If attribute could not found
	if(attribute == nullptr){
		std::cout << "VertArray: UpdateAttribute: Vertex attribute " << location << " could not be found" << std::endl;
		return;
	} 

	//Update attribute
	unsigned int pos = 0;
	while(pos < data.size()){
		glBufferSubData(buffer, offset, attribute->capacity, &data[pos]);

		//Increase offset
		offset += mStride;

		//Incriment data position
		pos += attribute->size;
	} 

	glBindBuffer(buffer, 0);
}
template void VertArray::updateAttribute<GLfloat>(GLuint, GLenum, std::vector<GLfloat> data);

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
