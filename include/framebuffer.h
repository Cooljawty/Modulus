#pragma once

#include "texture.h"
#include "shader.h"

#include <vector>

namespace Modulus{
	
	class FrameBuffer{
		public:
			FrameBuffer();
			~FrameBuffer();

			bool init(int bufferWidth, int bufferHeight, bool multisample=false);
			
			void bind(GLenum target);
			void bindTexture();
			void unbindTexture();

		private:
			unsigned int mFrameBufferID;
			unsigned int mRenderBufferID;
			//Only used when generating multisampled gexture
			unsigned int mmsFBOTextureID;
			
			//Non-multisampled texture
			Texture* mFBOTexture; 

			std::vector<Shader*> mShaderQueue;
	};
}
