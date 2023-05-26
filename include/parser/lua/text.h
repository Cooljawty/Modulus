#pragma once

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "text.h"

namespace Modulus::Parse::Lua{
	using namespace std;

	vector<Modulus::Font*> gLuaFonts;
	
	static int newFont(lua_State* L){
		if(!lua_checkstack(L, 2))
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

	static const struct luaL_Reg textLib [] = {
		{"new", newFont},
		{NULL, NULL}
	};
	static const struct luaL_Reg textMetaLib [] = {
		{NULL, NULL}
	};
}
