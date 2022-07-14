#include <iostream>
#include <SDL_GL.h>

#include "framebuffer.h"

using namespace Modulus;

FrameBuffer::FrameBuffer(){
	mFrameBufferID = 0;
	mRenderBufferID = 0;

	mmsFBOTextureID = 0;
	mFBOTexture = nullptr;
}

FrameBuffer::~FrameBuffer(){
	delete mFBOTexture;
}

bool FrameBuffer::init(int bufferWidth, int bufferHeight, bool multisample){

	//Generating frame buffer
	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	
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
		mFBOTexture = new Modulus::Texture;
		if(!mFBOTexture->loadFromPixel(NULL, bufferWidth, bufferHeight, GL_RGB, false, false)){
			std::cout << "FrameBuffer::Init: Unable to load frame buffer texture" << std::endl;
		}
		else{
			mFBOTexture->bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFBOTexture->getTexID(), 0);
			mFBOTexture->unbind();
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
}

void FrameBuffer::bindTexture(){
	if(mFBOTexture != nullptr){
		mFBOTexture->bind();
	}
	else if(mmsFBOTextureID != 0){
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mmsFBOTextureID);
	}
	else
		std::cout << "FrameBuffer::BindTexture: Error no texture initilized" << std::endl;
}

void FrameBuffer::unbindTexture(){
	if(mFBOTexture != nullptr){
		mFBOTexture->unbind();
	}
	else if(mmsFBOTextureID != 0){
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
		std::cout << "FrameBuffer::BindTexture: Error no texture initilized" << std::endl;
}
