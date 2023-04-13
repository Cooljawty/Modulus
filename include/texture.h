#pragma once
#include "SDL_GL.h"

#include <string>

namespace Modulus{

	class Texture{
		public:
			Texture();
			~Texture();

			//Loads texture from image using loadFromPixel
			bool loadFromImage(const std::string);
			
			//Loads a texture from pixle data
			bool loadFromPixel(void* pixels, GLuint width, GLuint height, GLint colorFormat, bool wrapS = true, bool wrapT = true);

			bool loadFromCache();
			
			//Deallocates teture
			void freeTexture();
			
			//Bind and unbind texure for rendering
			void bind();
			void unbind();

			//Gets member pixels
			bool lock();
			
			//Updates texture with member pixels 
			bool unlock();
			
			//Creates a blank set of pixels
			void createPixels32(GLuint, GLuint);
			
			bool cache();
			//Creates clip out of given position and dimensions
			//void makeClip(int, int, int, int);
			
			//Returns the texture's ID
			GLuint getTexID(){ return mTextureID; }
			
			//Returns the texture's dimensions
			GLuint getWidth(){ return mWidth; }
			GLuint getHeight(){ return mHeight; }
			
			//Returns member pixel data
			GLuint* getPixelData32(){ return mPixels; }

			//Returns pixel at given location
			GLuint getPixel32(GLuint x, GLuint y){ return (y * mWidth + x <= mWidth * mHeight) ? mPixels[y * mWidth + x] : 0; }
			GLuint getPixel32(GLuint x){ return (x <= mWidth * mHeight) ? mPixels[x] : 0; }

			//Sets pixel at given location
			void setPixel32(GLuint x, GLuint y, GLuint pixel){ if(y * mWidth + x <= mWidth * mHeight) mPixels[y * mWidth + x] = pixel; }
			void setPixel32(GLuint x, GLuint pixel){ if(x <= mWidth * mHeight) mPixels[x] = pixel; }

		protected:
			//The texture id
			GLuint mTextureID;

			//Pixel data and format
			GLuint* mPixels;
			GLuint mPixelFormat;

			//Image dimensions
			GLuint mWidth;
			GLuint mHeight;
	};
}
