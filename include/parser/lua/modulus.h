#pragma once

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

namespace Modulus::Parse::Lua{
	static int getScreenDimenstions( lua_State* L ){
		
		lua_pushstring(L, "Modulus.GameManager");
		lua_gettable(L, LUA_REGISTRYINDEX);

		GameManager& modulusContext = *(GameManager*)lua_touserdata(L, -1);

		lua_pushinteger(L, modulusContext.getScreenWidth());

		lua_pushinteger(L, modulusContext.getScreenHeight());

		return 2;
	}

	static const struct luaL_Reg gameManagerLib [] = {
		{"getWindow", getScreenDimenstions},
		{NULL, NULL}
	};
}

//draw(Shader&, VertArray&, std::vector<Material>, FrameBuffer&, GLenum drawMode = GL_TRIANGLES);
//draw(Shader&, Mesh&, FrameBuffer&);
//draw(Shader& s, FrameBuffer& srcFB, FrameBuffer& destFB)
			
