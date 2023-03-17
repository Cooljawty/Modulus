#pragma once

#include <vector>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "framebuffer.h"

namespace Modulus::Parse::Lua{
	std::vector <Modulus::FrameBuffer*> gLuaFrameBuffers;

	static int newFrameBuffer( lua_State* L ){
		if( !lua_checkstack(L, 3) ){
			luaL_error(L, "Not enough stack space");
		}

		luaL_argcheck(L, lua_isinteger(L, 1), 1, "Expected integer for framebuffer width");
		int width = lua_tonumber(L,1);

		luaL_argcheck(L, lua_isinteger(L, 2), 2, "Expected integer for framebuffer height");
		int height = lua_tonumber(L,2);

		size_t size = sizeof(Modulus::FrameBuffer);

		auto newFrameBuffer = (Modulus::FrameBuffer*)lua_newuserdata(L, size);
		newFrameBuffer = new (newFrameBuffer) Modulus::FrameBuffer();

		if( !newFrameBuffer->init(width, height) )
			luaL_error(L, "Error initilizing framebuffer");

		gLuaFrameBuffers.push_back( newFrameBuffer );
		return 1;
	}

	static const struct luaL_Reg frameBufferLib [] = {
		{"new", newFrameBuffer},
		{NULL, NULL}
	};

};
