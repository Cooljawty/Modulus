#pragma once
namespace Modulus{
	class Font{
		public:
			//Initilizes the VAO for rendering with a givien shader
			void initVAO(TextShader &shader);

			//Loads characters from a font
			bool loadFont(const std::string fontPath, unsigned int fontSize);
			
			//Renders a given string to screen
			void renderText(TextShader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
		
		private:
		
			VertArray mTextVAO;
		
			//Contains a character's texture and metrics
			struct Character{
				GLuint TextureID;
				glm::vec2 Size;
				glm::vec2 Bearing;
				long int Advance;
			};
			
			//Maps each character to it's associated Character object
			std::map<char, Character> mCharacters;
	};
}
