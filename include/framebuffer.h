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
			
			Mesh& getMesh(){ return *mFBOMesh; }
			void draw(Shader& shader);
			void clear( GLenum target);
		private:
			unsigned int mFrameBufferID;
			unsigned int mRenderBufferID;
			//Only used when generating multisampled gexture
			unsigned int mmsFBOTextureID;
			
			//Non-multisampled texture
			Mesh* mFBOMesh; 
	};
}
