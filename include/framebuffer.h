#pragma once

#include "mesh.h"

namespace Modulus{
	class FrameBuffer{
		public:
			FrameBuffer();
			~FrameBuffer();

			bool init(int bufferWidth, int bufferHeight, bool multisample=false);
			
			void bind(GLenum target);
			void bindTexture();
			void unbindTexture();
			
			void draw(Shader& shader);
		private:
			unsigned int mFrameBufferID;
			unsigned int mRenderBufferID;
			//Only used when generating multisampled gexture
			unsigned int mmsFBOTextureID;
			
		public:
			//Non-multisampled texture
			Mesh* mFBOMesh; 
	};
}
