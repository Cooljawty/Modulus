#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "SDL_GL.h"

#include "data_types.h"
#include "mesh.h"

namespace Modulus::Parse::Lua{
	std::vector<Modulus::Mesh*> gLuaMeshes;

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
	
	static Modulus::Material getMaterial(lua_State* L){
		Modulus::Material material;
		
		lua_pushstring(L, "path");
		if( lua_gettable(L, -1) != LUA_TSTRING ){
			luaL_error(L, "Expected path to material texture");
			return material;
		}

		material.path = lua_tostring(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "type");
		if( lua_gettable(L, -1) != LUA_TSTRING )
			material.type = "texture";
		else
			material.type = lua_tostring(L, -1);

		return material;
	}
	
	static int newMesh(lua_State* L){
		if(!lua_checkstack(L, 20))
			luaL_error(L, "Not enough stack space");
		if( !lua_istable(L, 1) )
			luaL_error(L, "Expected table of vertecies");
		if( !lua_istable(L, 2) )
			luaL_error(L, "Expected table of materials");
		
		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		
		/* Getting vao */
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
	
		/* Getting materials */
		vector<Modulus::Material> materials;

		lua_pushnil(L);
		while(lua_next(L, 2) !=0){
			materials.emplace_back( getMaterial(L));		
			
			lua_pop(L, 1);
		}

		/* Rough size estimate */
		size_t size =	  sizeof(GL_FLOAT) * verticies.size() 
						+ sizeof(unsigned int) * indicies.size() 
				 	 	+ sizeof(Modulus::Material) * materials.size()
				 	 	+ sizeof(unsigned int) + sizeof(GLenum) 
				 	 	+ sizeof(GLenum);

		Modulus::Mesh* newMesh = (Modulus::Mesh*)lua_newuserdata(L, size);
		newMesh= new Mesh(verticies, indicies, materials, vector<pair<unsigned int, GLenum>>{format}, GL_TRIANGLES);
		gLuaMeshes.push_back(newMesh);
		
		luaL_setmetatable(L, "Modulus.mesh");

		return 1;
	}

	static const struct luaL_Reg meshLib [] = {
		{"new", newMesh},
		{NULL, NULL}
	};
}
