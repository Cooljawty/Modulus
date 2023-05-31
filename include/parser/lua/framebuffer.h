#pragma once

#include <vector>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "framebuffer.h"

namespace Modulus::Parse::Lua{
	std::vector <Modulus::FrameBuffer*> gLuaFrameBuffers;

	static int newFrameBuffer( lua_State* L ){
		if( !lua_checkstack(L, 3) ) luaL_error(L, "Not enough stack space");

		int isInt;
		int width = lua_tointegerx(L, 1, &isInt);
		luaL_argcheck(L, isInt,     1, "Expected integer for framebuffer width");
		luaL_argcheck(L, width > 0, 1, "Framebuffer width must be greatedr than 0");

		int height = lua_tointegerx(L, 2, &isInt);
		luaL_argcheck(L, isInt,      2, "Expected integer for framebuffer height");
		luaL_argcheck(L, height > 0, 2, "Framebuffer height must be greatedr than 0");

		int sampleSize = 0;
		if( lua_gettop(L) == 3 ){
			sampleSize = lua_tointegerx(L, 3, &isInt);
			luaL_argcheck(L, isInt, 3, "Expected integer for sample size");
		}

		size_t size = sizeof(Modulus::FrameBuffer);

		auto newFrameBuffer = (Modulus::FrameBuffer*)lua_newuserdata(L, size);
		newFrameBuffer = new (newFrameBuffer) Modulus::FrameBuffer();

		if( !newFrameBuffer->init(width, height, sampleSize) )
			luaL_error(L, "Error initilizing framebuffer");

		luaL_setmetatable(L, "Modulus.framebuffer");

		gLuaFrameBuffers.push_back( newFrameBuffer );
		return 1;
	}

	static const struct luaL_Reg frameBufferLib [] = {
		{"new", newFrameBuffer},
		{NULL, NULL}
	};

};
