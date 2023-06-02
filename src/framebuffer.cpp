#include <iostream>
#include <SDL_GL.h>

#include "framebuffer.h"

using namespace Modulus;

FrameBuffer::FrameBuffer(){
	mFrameBufferID = 0;
	mRenderBufferID = 0;
	
	mWidth  = 0;
	mHeight = 0;

	mFBOMesh = nullptr;
}

FrameBuffer::~FrameBuffer(){
}

bool FrameBuffer::init(int bufferWidth, int bufferHeight, unsigned int multisample){

	mFBOMesh = new Mesh(
		std::vector<float>{
		  -1.f,  1.f, 0.f,	1.f,
		  -1.f, -1.f, 0.f,	0.f,
		   1.f, -1.f, 1.f, 0.f,
		   1.f,  1.f, 1.f, 1.f,
		},
		std::vector<unsigned int>{0, 1, 2, 3},
		std::vector<Material>{ Material{nullptr, "framebuffer", "non-sampled"}, Material{nullptr, "framebuffer", "multisampled"} },
		{ {2, GL_FLOAT}, {2, GL_FLOAT} },
		GL_TRIANGLE_FAN
	);

	mWidth  = bufferWidth;
	mHeight = bufferHeight;

	//Generating frame buffer
	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	

	//Generating texture for frame buffer
	mFBOMesh->getMaterials()[0].texture = new Modulus::Texture;
	if(!mFBOMesh->getMaterials()[0].texture->loadFromPixel(NULL, bufferWidth, bufferHeight, GL_RGB, false, false)){
		std::cout << "FrameBuffer::Init: Unable to load frame buffer texture" << std::endl;
		return false;
	}
	mFBOMesh->getMaterials()[0].texture->bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFBOMesh->getMaterials()[0].texture->getTexID(), 0);
	mFBOMesh->getMaterials()[0].texture->unbind();

	if( multisample != 0 ){
		glGenFramebuffers(1, &mmsFrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mmsFrameBufferID);
		
		//Generating texture for frame buffer
		mFBOMesh->getMaterials()[1].texture = new Modulus::Texture( multisample );
		if(!mFBOMesh->getMaterials()[1].texture->loadFromPixel(NULL, bufferWidth, bufferHeight, GL_RGB, false, false)){
			std::cout << "FrameBuffer::Init: Unable to load frame buffer texture" << std::endl;
			return false;
		}
		mFBOMesh->getMaterials()[1].texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mFBOMesh->getMaterials()[1].texture->getTexID(), 0);
		mFBOMesh->getMaterials()[1].texture->unbind();
	}

	//Generating render buffer to depth and stencil
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	if(multisample != 0) glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	else glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
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
	glBindFramebuffer(target, mmsFrameBufferID != 0 ? mmsFrameBufferID : mFrameBufferID);
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "FrameBuffer::Bind: Error " << gluErrorString(error) << std::endl;
	}

}

void FrameBuffer::bindTexture(){
	if(mFBOMesh->getMaterials()[0].texture == nullptr)
		std::cout << "FrameBuffer::BindTexture: Error no texture initilized" << std::endl;
	
	if( isMultisample() ){
		//Copy multisampled framebuffer to non sampled framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mmsFrameBufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBufferID);
		glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	mFBOMesh->getMaterials()[0].texture->bind();
}

void FrameBuffer::unbindTexture(){
	if(mFBOMesh->getMaterials()[0].texture != nullptr)
		mFBOMesh->getMaterials()[0].texture->unbind();
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

	VertArray& vao = mFBOMesh->getVertArray();
	auto indicies = vao.getIndexBuffer();
	vao.bind();

	bindTexture();

	int m = 0;
	if( !shader.setParameter(("material.framebuffer"), GL_INT, &m, false) ) return;
		
	glDrawElements(mFBOMesh->getDrawMode(), indicies.size(), GL_UNSIGNED_INT, 0);

	shader.resetParameters();

	vao.unbind();
	shader.unbind();

	if( depthEnabled ) glEnable(GL_DEPTH_TEST);

	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		cout << "FrameBuffer::Draw: error while rendering: " << gluErrorString(error) << endl;
	}
}

void FrameBuffer::clear(GLenum target){
	this->bind( target );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(target, 0);
}
