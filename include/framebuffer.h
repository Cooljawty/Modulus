#pragma once

#include "texture.h"
namespace Modlulus{
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
			Modulus::Texture* mFBOTexture; 
	};
}
