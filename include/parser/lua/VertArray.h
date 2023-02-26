#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "SDL_GL.h"

#include "VertArray.h"

namespace Modulus::Parse::Lua{
	std::vector<Modulus::VertArray*> gLuaVertArrays;

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
	
	static int newVertArray(lua_State* L){
		if(!lua_checkstack(L, 5))
			luaL_error(L, "Not enough stack space");
		if( !lua_istable(L, 1) )
			luaL_error(L, "Expected table of vertecies");
		
		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		
		/* Getting vao */
		unsigned int i = 0;
		std::vector<std::pair<unsigned int, GLenum>> format;

		lua_pushstring(L, "format");
		switch(lua_gettable(L, -2)){
			//Vertex array is preformatted
			case LUA_TTABLE: 
			{}
			break;
			//Format of vertex array must be deduced
			case LUA_TNIL:   			
			{
				lua_next(L, 1); 
				indicies.push_back(i++);
				verticies = getVerticies(L);

				format.push_back( {verticies.size(), GL_FLOAT} );
				while(lua_next(L, 1) != 0){
					indicies.push_back(i++);
					auto vertex = getVerticies(L);
					
					if(vertex.size() != std::get<0>(format.back()))
						luaL_error(L, "Vertex does not follow format %d floats", std::get<0>(format.back()));
					
					verticies.insert(verticies.end(), vertex.begin(), vertex.end());	
				}
				break;
			}
			default:
				luaL_error(L, "Format for vertex array must be given as table");
			break;
		}

		/* Rough size estimate */
		size_t size =	  sizeof(Modulus::VertArray) 
						+ format.size() * (sizeof(GLuint) + sizeof(GLint) + sizeof(GLenum) + sizeof(size_t)); 

		Modulus::VertArray* newVertArray = (Modulus::VertArray*)lua_newuserdata(L, size);
		newVertArray = new (newVertArray) Modulus::VertArray();
		gLuaVertArrays.emplace_back(newVertArray);

		i = 0;
		for(auto attr: format){
			if(!newVertArray->addAttribute( i++, std::get<0>(attr), std::get<1>(attr))){
				luaL_error(L, "Error with given format"); 
			}
		}
		newVertArray->initVAO(verticies, indicies, GL_STATIC_DRAW);
	
		luaL_setmetatable(L, "Modulus.VertArray");
			
		return 1;
	}

	static const struct luaL_Reg vertArrayLib [] = {
		{"new", newVertArray},
		{NULL, NULL}
	};
}
