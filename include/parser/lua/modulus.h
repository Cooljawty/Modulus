#pragma once

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "VertArray.h"

namespace Modulus::Parse::Lua{
	static int getScreenDimenstions( lua_State* L ){
		
		lua_pushstring(L, "Modulus.GameManager");
		lua_gettable(L, LUA_REGISTRYINDEX);

		GameManager& modulusContext = *(GameManager*)lua_touserdata(L, -1);

		//Width
		lua_pushinteger(L, modulusContext.getScreenWidth());

		//Height
		lua_pushinteger(L, modulusContext.getScreenHeight());

		return 2;
	}

	static int draw( lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		
		Modulus::VertArray* vao = (Modulus::VertArray*)luaL_checkudata(L, 1, "Modulus.vertexArray");
		luaL_argcheck(L, vao != NULL, 1, "Expected a vertex array");

		return 0;
	}
	
	static const struct luaL_Reg gameManagerLib [] = {
		{"getWindow", getScreenDimenstions},
		{NULL, NULL}
	};
}

//draw(Shader&, VertArray&, std::vector<Material>, FrameBuffer&, GLenum drawMode = GL_TRIANGLES);
//draw(Shader&, Mesh&, FrameBuffer&);
//draw(Shader& s, FrameBuffer& srcFB, FrameBuffer& destFB)
			
