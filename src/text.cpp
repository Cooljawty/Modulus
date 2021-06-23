#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "SDL_GL.h"

#include "texture.h"
#include "VertexArray.h"
#include "TextShader.h"

#include "text.h"

//The rance of caracters to render fromm the font
const unsigned char CHARACTER_RANGE = 128;

//Initilizes the VAO for rendering with a givien shader
void Font::initVAO(TextShader &shader){
	//Initilizing VAO for text rendering
	mTextVAO.addAttribute(shader.getVertexID(), 4, GL_FLOAT);
	mTextVAO.initVAO(std::vector<GLfloat>(24), {0,1,2,3}, GL_DYNAMIC_DRAW);	
}

//Loads characters from a font
bool Font::loadFont(const std::string fontPath, unsigned int fontSize){
	//Load freetype library and font
	FT_Library ftlib;
	FT_Face face;
	if(FT_Init_FreeType(&ftlib)){
		std::cout << "Font::Load Font: Unable to initilize FreeType library." << std::endl;
		return false;
	}
	else{
		if(FT_New_Face(ftlib, fontPath.c_str(), 0, &face)){
			std::cout << "Font::Load Font: Unable to load font." << std::endl;
			return false;
		}
		else{
			//Load characters from font
			FT_Set_Pixel_Sizes(face, 0, fontSize);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for(unsigned char c = 0; c < CHARACTER_RANGE; c++){
				//Load glyph
				if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
					std::cout << "Font::Load Font: Error could not load glyph \'" << c << "\'" << std::endl;
					continue;
				}
				
				//Generate texture
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D,	0, GL_RED,
							 face->glyph->bitmap.width, face->glyph->bitmap.rows,
							 0, GL_RED, GL_UNSIGNED_BYTE,
							 face->glyph->bitmap.buffer);

				//Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

				//Store character to map
				Character character{
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				mCharacters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			
			//Deallocate Freetype library and face
			FT_Done_Face(face);
			FT_Done_FreeType(ftlib);
		}
	}
	
	return true;
}

//Renders a given string to screen
void Font::renderText(TextShader &shader, std::string text, float x, float y, float scale, glm::vec3 color){

	shader.bind();

	shader.setTextColor(color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0);

	mTextVAO.bind();

	std::string::const_iterator c;
	for(c = text.begin(); c != text.end(); c++){
		Character ch = mCharacters[*c];
		
		//Translate to character's posiiton
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
	
	 	std::vector<GLfloat> vData{
			xpos,	  ypos + h,	0.f, 0.f,
			xpos,	  ypos,		0.f, 1.f,
			xpos + w, ypos,		1.f, 1.f,

			xpos,	  ypos + h,	0.f, 0.f,
			xpos + w, ypos,		1.f, 1.f,
			xpos + w, ypos + h,	1.f, 0.f,
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		mTextVAO.update(GL_ARRAY_BUFFER, 0, vData);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//Move to next character's position
		x += (ch.Advance >> 6) * scale;
	} 

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mTextVAO.unbind();
	shader.unbind();
}

