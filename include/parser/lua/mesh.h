#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "SDL_GL.h"

#include "mesh.h"

namespace Modulus::Parse::Lua{
	std::vector<Modulus::Mesh> gLuaMeshes;

	static std::vector<float> getVerticies(lua_State* L, int tableIndex = 4){
		std::vector<float> verticies;
		
		lua_pushnil(L);
		while(lua_next(L, tableIndex) != 0){
			int success;
			verticies.push_back( lua_tonumberx(L, -1, &success));
			if(!success){
				luaL_error(L, "Expected number for vertex attribute");
				return std::vector<float>();
			}
			
			lua_remove(L, -1);
		}
		
		lua_remove(L, -1);

		return verticies;
	}
	
	static int newMesh(lua_State* L){
		if(!lua_checkstack(L, 20))
			luaL_error(L, "Not enough stack space");
		if( !lua_istable(L, 1) )
			luaL_error(L, "Expected table of vertecies");
		if( !lua_istable(L, 2) )
			luaL_error(L, "Expected table of vertecies");
		
		std::vector<float> verticies;
		std::vector<unsigned int> indicies;

		unsigned int i = 0;

		lua_pushnil(L);
		lua_next(L, 1); 
		indicies.push_back(i++);
		verticies = getVerticies(L);
		std::pair<unsigned int, GLenum> format = {verticies.size(), GL_FLOAT};

		while(lua_next(L, 1) != 0){
			indicies.push_back(i++);
			auto vertex = getVerticies(L);
			
			if(vertex.size() != std::get<0>(format))
				luaL_error(L, "Vertex does not follow format %d floats", std::get<0>(format));
			
			verticies.insert(verticies.end(), vertex.begin(), vertex.end());	
		}
		
		gLuaMeshes.emplace_back(verticies, indicies, vector<Material>(), vector<pair<unsigned int, GLenum>>{format}, GL_TRIANGLES);

		lua_settop(L, 0 );
		lua_pushnumber(L, 1);
		return 1;
	}

	static const struct luaL_Reg meshLib [] = {
		{"new", newMesh},
		{NULL, NULL}
	};
}
