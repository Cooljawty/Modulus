#pragma once

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "text.h"
#include "framebuffer.h"

namespace Modulus::Parse::Lua{
	using namespace std;

	vector<Modulus::Font*> gLuaFonts;
	
	static int newFont(lua_State* L){
		if(!lua_checkstack(L, 3))
			luaL_error(L, "Not enough stack space");

		luaL_argexpected(L, lua_isstring(L,  1), 1, "Path to font");
		luaL_argexpected(L, lua_isinteger(L, 2), 2, "Font size");
		luaL_argexpected(L, lua_isinteger(L, 3), 3, "Font resolution");

		/* Rough size estimate */
		size_t size = sizeof(Modulus::Font);
		Modulus::Font* newFont = (Modulus::Font*)lua_newuserdata(L, size);

		newFont = new (newFont) Modulus::Font;
		newFont->initVAO();
		newFont->loadFont( lua_tostring(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3) );

		gLuaFonts.push_back(newFont);
		
		luaL_setmetatable(L, "Modulus.font");

		return 1;
	} 

	static int renderText( lua_State* L ){
		if(!lua_checkstack(L, 6))
			luaL_error(L, "Not enough stack space");
		
		//get font
		Modulus::Font* font = (Modulus::Font*)luaL_checkudata(L, 1, "Modulus.font");
		
		//get shader
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 2, "Modulus.shader");
		
		luaL_argexpected(L, lua_isstring(L, 3), 3, "String");
		std::string text = lua_tostring(L, 3);

		//	position
		float x, y;
		luaL_argexpected(L, lua_istable(L, 4), 4, "Position coordinates");

		lua_pushinteger(L, 1);
		lua_gettable(L, 4);

		luaL_argexpected(L, lua_isnumber(L, -1), 4, "X coordinate");
		x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushinteger(L, 2);
		lua_gettable(L, 4);

		luaL_argexpected(L, lua_isnumber(L, -1), 4, "Y coordinate");
		y = lua_tonumber(L, -1);
		lua_pop(L, 1);

		//	scale
		luaL_argexpected(L, lua_isnumber(L, 5), 5, "Text scale");
		float scale = lua_tonumber(L, 5);

		//	color
		glm::vec3 color(0.f);
		luaL_argexpected(L, lua_istable(L, 6), 6, "Color vector");

		lua_pushinteger(L, 1);
		lua_gettable(L, 6);

		luaL_argexpected(L, lua_isnumber(L, -1), 6, "Red component");
		color[0] = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushinteger(L, 2);
		lua_gettable(L, 6);

		luaL_argexpected(L, lua_isnumber(L, -1), 6, "Green component");
		color[1] = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushinteger(L, 3);
		lua_gettable(L, 6);

		luaL_argexpected(L, lua_isnumber(L, -1), 6, "Blue component");
		color[2] = lua_tonumber(L, -1);
		lua_pop(L, 1);

		//get frame buffer
		Modulus::FrameBuffer* framebuffer = (Modulus::FrameBuffer*)luaL_checkudata(L, 7, "Modulus.framebuffer");
		
		//render text
		framebuffer->bind(GL_FRAMEBUFFER);
		font->renderText( *shader, text, x, y, scale, color );

		return 0;
	}
	
	static const struct luaL_Reg fontLib [] = {
		{"new", newFont},
		{NULL, NULL}
	};
	static const struct luaL_Reg fontMetaLib [] = {
		{"draw", renderText},
		{NULL, NULL}
	};
}
