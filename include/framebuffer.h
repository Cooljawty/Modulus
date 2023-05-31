#pragma once

#include "mesh.h"

namespace Modulus{
	class FrameBuffer{
		public:
			FrameBuffer();
			~FrameBuffer();

			bool init(int bufferWidth, int bufferHeight, unsigned int multisample=0);
			
			void bind(GLenum target);
			void bindTexture();
			void unbindTexture();
<<<<<<< HEAD
			
			Mesh& getMesh(){ return *mFBOMesh; }
			void draw(Shader& shader);
=======

			bool isMultisample(){ return mmsFrameBufferID != 0; }
			
			Mesh& getMesh(){ return *mFBOMesh; }
			void draw(Shader& shader);
			void clear( GLenum target);
>>>>>>> cooljawty
		private:
			unsigned int mFrameBufferID;
			unsigned int mmsFrameBufferID;

			unsigned int mRenderBufferID;

			int mWidth, mHeight;
			
			//Non-multisampled texture
			Mesh* mFBOMesh; 
	};
}
