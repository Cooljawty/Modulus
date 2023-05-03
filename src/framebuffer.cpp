#include <iostream>
#include <SDL_GL.h>

#include "framebuffer.h"

using namespace Modulus;

FrameBuffer::FrameBuffer(){
	mFrameBufferID = 0;
	mRenderBufferID = 0;

	mmsFBOTextureID = 0;
	
	mFBOMesh = nullptr;
}

FrameBuffer::~FrameBuffer(){
}

bool FrameBuffer::init(int bufferWidth, int bufferHeight, bool multisample){

	//Generating frame buffer
	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	
	mFBOMesh = new Mesh(
		std::vector<float>{
		  -1.f,  1.f, 0.f,	1.f,
		  -1.f, -1.f, 0.f,	0.f,
		   1.f, -1.f, 1.f, 0.f,
		   1.f,  1.f, 1.f, 1.f,
		},
		std::vector<unsigned int>{0, 1, 2, 3},
		std::vector<Material>{ Material{nullptr, "framebuffer", ""} },
		{ {2, GL_FLOAT}, {2, GL_FLOAT} },
		GL_TRIANGLE_FAN
	);
	//Generating texture for frame buffer
	if(multisample){
		glGenTextures(1, &mmsFBOTextureID);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mmsFBOTextureID);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, bufferWidth, bufferHeight, GL_TRUE);
		if(mmsFBOTextureID == 0){
			std::cout << "FrameBuffer::Init: Error creating multisampled texture." << std::endl;
			return false;
		}
	
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mmsFBOTextureID, 0);
	}
	else{
		mFBOMesh->getMaterials()[0].texture = new Modulus::Texture;

		if(!mFBOMesh->getMaterials()[0].texture->loadFromPixel(NULL, bufferWidth, bufferHeight, GL_RGB, false, false)){
			std::cout << "FrameBuffer::Init: Unable to load frame buffer texture" << std::endl;
		}
		else{
			mFBOMesh->getMaterials()[0].texture->bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFBOMesh->getMaterials()[0].texture->getTexID(), 0);
			mFBOMesh->getMaterials()[0].texture->unbind();
		}
	}

	//Generating render buffer to depth and stencil
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	if(multisample) glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	else			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "FrameBuffer::Init: Framebuffer is not complete!" << std::endl;
		return false;	
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return true;
}

void FrameBuffer::bind(GLenum target){
	glBindFramebuffer(target, mFrameBufferID);
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FrameBuffer::Bind: Error " << gluErrorString(error) << std::endl;
	}

}

void FrameBuffer::bindTexture(){
	if(mFBOMesh->getMaterials()[0].texture != nullptr){
		mFBOMesh->getMaterials()[0].texture->bind();
	}
	else if(mmsFBOTextureID != 0){
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mmsFBOTextureID);
	}
	else
		std::cout << "FrameBuffer::BindTexture: Error no texture initilized" << std::endl;
}

void FrameBuffer::unbindTexture(){
	if(mFBOMesh->getMaterials()[0].texture != nullptr){
		mFBOMesh->getMaterials()[0].texture->unbind();
	}
	else if(mmsFBOTextureID != 0){
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
		std::cout << "FrameBuffer::BindTexture: Error no texture initilized" << std::endl;
}

void FrameBuffer::draw( Shader& shader ){
	
	shader.bind();

	bool depthEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	if( mFBOMesh == nullptr){
		std::cout << "FrameBuffer::Draw: Error Mesh is not initilized" << std::endl;
		return;
	}

	mFBOMesh->draw(shader);

	if( depthEnabled ) glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::clear(GLenum target){
	this->bind( target );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(target, 0);
}
