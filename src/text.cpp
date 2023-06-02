/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.		  
 */
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
void Font::initVAO( GLuint vertexAttr, GLuint textureAttr){
	//Initilizing VAO for text rendering
	mTextVAO.addAttribute(vertexAttr, 2, GL_FLOAT);
	mTextVAO.addAttribute(textureAttr, 2, GL_FLOAT);

	std::vector<GLfloat> vData{
		1.0,  0.0,	1.f, 0.f,
		0.0,  0.0,	0.f, 0.f,
		0.0, -1.0,	0.f, 1.f,
		1.0, -1.0,	1.f, 1.f,
	};

	mTextVAO.initVAO(vData, {0,1,2,3}, GL_DYNAMIC_DRAW);	
}

//Loads characters from a font
bool Font::loadFont(const std::string fontPath, unsigned int fontSize, unsigned int resolution){
	
	using namespace std;

	//Load freetype library and font
	FT_Library ftlib;
	FT_Face face;

	if(FT_Init_FreeType(&ftlib)){
		cerr << "Font::Load Font: Unable to initilize FreeType library." << endl;
		return false;
	}
	
	if(FT_New_Face(ftlib, fontPath.c_str(), 0, &face)){
		cerr << "Font::Load Font: Unable to load font." << endl;
		return false;
	}
	
	mParameters.charRange = face->num_glyphs;

	mParameters.size = fontSize * ( face->units_per_EM != 0 ? face->units_per_EM : 1);
	mParameters.scale = static_cast<float>(fontSize) / resolution;

	mParameters.lineHeight = fontSize * face->height / static_cast<float>(resolution);

	//Load characters from font
	FT_Set_Char_Size(face, 0, mParameters.size, 0, resolution);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(unsigned char c = 0; c < CHARACTER_RANGE; c++){
		//Load glyph
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
			cerr << "Font::Load Font: Error could not load glyph \'" << c << "\'" << endl;
			continue;
		}
		
		//Generate texture
		Texture* texture = new Texture;
		texture->loadFromPixel(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED , false, false);

		//Store character to map
		Character character{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		mCharacters.insert(pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	//Deallocate Freetype library and face
	FT_Done_Face(face);
	FT_Done_FreeType(ftlib);

	return true;
}

//Renders a given string to screen
void Font::renderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color){

	GLboolean prevDepth; glGetBooleanv(GL_DEPTH_TEST, &prevDepth);
	glDisable(GL_DEPTH_TEST);

	shader.bind();

	shader.setParameter("TextColor", GL_FLOAT_VEC3, &color);

	glActiveTexture(GL_TEXTURE0);

	mTextVAO.bind();

	std::string::const_iterator c;
	float startX = x;
	scale *= mParameters.scale;
	for(c = text.begin(); c != text.end(); c++){
		switch(*c){
		case '\n': 
			x = startX;
			y -= mParameters.lineHeight * mParameters.lineSpacing * scale;
			continue;
		case '\t':
			x += ( mCharacters[' '].Advance >> 6 ) * mParameters.tabWidth * scale;
			continue;
		case '\r':
			continue;
		}

		Character ch = mCharacters[*c];
		if( ch.Texture == nullptr ) continue;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
	
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (mParameters.lineHeight - ch.Bearing.y) * scale;

		glm::mat4 modelMat = glm::mat4( 1.0 );
		modelMat = glm::translate( modelMat, glm::vec3( xpos, ypos, 0) );
		modelMat = glm::scale( modelMat, glm::vec3( w, h, 1) );

		shader.setParameter( "ModelMatrix", GL_FLOAT_MAT4, &modelMat, false);

		ch.Texture->bind();

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		
		ch.Texture->unbind();

		//Move to next character's position
		x += (ch.Advance >> 6) * scale;
	} 

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mTextVAO.unbind();
	shader.unbind();

	if(prevDepth) glEnable(GL_DEPTH_TEST);
}

