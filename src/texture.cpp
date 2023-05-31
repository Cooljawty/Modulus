#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "texture.h"

using namespace Modulus;

//Initialize defalt values for texture
Texture::Texture( unsigned int samples ){

	mTextureID = 0;

	mSampleSize = samples;
	
	mPixels = nullptr;
	mPixelFormat = 0;

	mWidth = 0;
	mHeight = 0;
}

//Deallocates memory
Texture::~Texture(){

	//Free texture
	freeTexture();
}

//Loads texture from image using loadFromPixel
bool Texture::loadFromImage(const std::string path){

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	
	if(loadedSurface == NULL){
		std::cout << "Texture: LoadFromImage: Unable to load image \"" << path
			 << "\" SDL_image Error: " << IMG_GetError()
			 << std::endl;

		return false;
	}
	
	/* SDL's texture format is flipped from OpenGL's, thus flip
	 * Code from Stackoverflow user vvanpelt
	 * URL: https://stackoverflow.com/a/65817254/5302759 */
	/*---------------------------------------------------------*/
	SDL_LockSurface(loadedSurface);
    
    int pitch = loadedSurface->pitch; // row size
    char* temp = new char[pitch]; // intermediate buffer
    char* pixels = (char*) loadedSurface->pixels;
    
    for(int i = 0; i < loadedSurface->h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (loadedSurface->h - i - 1) * pitch;
        
        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }
    
    delete[] temp;
	/*---------------------------------------------------------*/

    SDL_UnlockSurface(loadedSurface);

	return loadFromPixel(loadedSurface->pixels, loadedSurface->w, loadedSurface->h, (loadedSurface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB));	
}

//Loads a texture from pixle data
bool Texture::loadFromPixel(void* pixels, GLuint width, GLuint height, GLint colorFormat, bool wrapS, bool wrapT){

	//Free if texture is not clearded
	//freeTexture();

	//Get Dimensions
	mWidth = width;
	mHeight = height;

	//Save color format
	mPixelFormat = colorFormat;

	//Generate a texture ID
	GLenum textureType = mSampleSize != 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glGenTextures(1, &mTextureID);
	glBindTexture( textureType , mTextureID);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "Texture: LoadFromPixle: Error generating texture. "
			 << gluErrorString(error) << std::endl;
		return false;
	}

	//Set texture parameters
	if( textureType == GL_TEXTURE_2D ){
		if(wrapS) glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		else	  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if(wrapT) glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		else	  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//Check for errors
		error = glGetError();
		if(error != GL_NO_ERROR){
			std::cout << "Texture: LoadFromPixle: Error setting tex parameters. "
				 << gluErrorString(error) << std::endl;
			return false;
		}
	}

	//Generate texture
	if( textureType == GL_TEXTURE_2D_MULTISAMPLE){	
		glTexImage2DMultisample(textureType, mSampleSize, colorFormat, mWidth, mHeight, GL_TRUE);
	}
	else {
		glTexImage2D(textureType, 0 , colorFormat, mWidth, mHeight, 0, colorFormat, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(textureType);
	}

	//Check for errors
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "Texture: LoadFromPixle: Error loading from pixels. "
			 << gluErrorString(error) << std::endl;
		return false;
	}

	return true;
}

bool Texture::loadFromCache(){
	if(mPixels == nullptr){
		std::cout << "Texture: LoadFromCache: Could not load from cache." << std::endl;
		return false;
	}

	return loadFromPixel(mPixels, mWidth, mHeight, mPixelFormat);
}

//Deallocates texture
void Texture::freeTexture(){

	//Free texture if it exists
  	if(mTextureID != 0 && mSampleSize == 0){
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	//Delete pixels
	if(mPixels != nullptr){
		delete [] mPixels;
		mPixels = nullptr;
	}

	mWidth = 0;
	mHeight = 0;
}

//Bind texure for rendering
void Texture::bind(){
	glBindTexture(mSampleSize != 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, mTextureID);
}

//Unbind texure after rendering
void Texture::unbind(){
	glBindTexture(mSampleSize != 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);
}

//Locks the texture for manipulation
bool Texture::lock(){
	
	if(mPixels == nullptr && mTextureID != 0 && mSampleSize == 0){

		//Allocate memory
		GLuint size = mWidth * mHeight;
		mPixels = new GLuint[size];

		//Set current texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		//Get Pixels
		glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mPixels);

		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	else{
		std::cout << "Texture: Lock: Unsuccessful lock: "
			 << (mPixels != nullptr ? "Texture Not unlocked\n": "")
			 << (mTextureID == 0 ? "Texture does not exist\n" : "")
			 << (mSampleSize != 0 ? "Texture is a multisampled texture\n" : "")
			 << std::flush;
	}
	return false;
}

//Updates texture with member pixels
bool Texture::unlock(){

	if(mPixels == nullptr && mTextureID != 0 && mSampleSize == 0){

		//Set current texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		//Update texture
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mPixels);

		//Delete Pixels
		delete [] mPixels;
		mPixels = nullptr;

		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	else{
		std::cout << "Texture: Unlock: Unsuccessful unlock: "
			 << (mPixels == nullptr ? "Texture Not locked\n": "")
			 << (mTextureID == 0 ? "Texture does not exist\n" : "")
			 << (mSampleSize != 0 ? "Texture is a multisampled texture\n" : "")
			 << std::flush;
	}

	return false;
}

//Caches texture from OpenGL texture to member mPixels
bool Texture::cache(){

	//Check if texture exists and is unlocked
	if(mPixels == nullptr && mTextureID != 0 && mSampleSize == 0){

		//Allocate memory
		GLuint size = mWidth * mHeight;
		mPixels = new GLuint[size];

		//Set current texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		//Get Pixels
		glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mPixels);

		//Delete texture from OpenGL
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
		
		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	else{
		std::cout << "Texture: Cache: Unsuccessful cache: "
			 << (mPixels != nullptr ? "Texture Not unlocked ": "")
			 << (mTextureID == 0 ? "Texture does not exist " : "")
			 << (mSampleSize != 0 ? "Texture is a multisampled texture\n" : "")
			 << std::endl;
	}
	return false;
}

//Creates a blank set of pixels
void Texture::createPixels32(GLuint imgWidth, GLuint imgHeight){

	//Check for valid dimensions
	if(imgWidth > 0 && imgHeight > 0){

		//Get rid of any previous texture
		freeTexture();

		//Create pixels
		GLuint size = imgWidth * imgHeight;
		mPixels = new GLuint[size];

		//Copy pixel data
		mWidth = imgWidth;
		mHeight = imgHeight;

		//Set pixel format
		mPixelFormat = GL_RGBA;
	}
}
