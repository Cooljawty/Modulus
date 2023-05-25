#pragma once

#include "texture.h"
#include "VertexArray.h"
#include "TextShader.h"

#include <string>
#include <map>

namespace Modulus{
	class Font{
		public:
			//Initilizes the VAO for rendering with a givien shader
			void initVAO( GLuint vertexAttr = 0, GLuint textureAttr = 1);

			//Loads characters from a font
			bool loadFont(const std::string fontPath, unsigned int fontSize, unsigned int resolution);
			
			//Renders a given string to screen
			void renderText(TextShader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
		
		private:
		
			VertArray mTextVAO;
		
			//Contains a character's texture and metrics
			struct Character{
				Modulus::Texture* Texture;
				glm::vec2 Size;
				glm::vec2 Bearing;
				long int Advance;
			};
			
			struct FontParams{
				unsigned int size;
				unsigned long charRange;
				unsigned long width, height;
			} mParameters;

			//Maps each character to it's associated Character object
			std::map<char, Character> mCharacters;
	};
}
