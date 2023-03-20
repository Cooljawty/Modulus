#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "parser/lua/utilities.h"

#include "SDL_GL.h"

#include "VertArray.h"

namespace Modulus::Parse::Lua{
	using std::vector;
	using std::pair;

	std::vector<Modulus::VertArray*> gLuaVertArrays;
	
	template< typename vType >
	static vector<vType> getVerticies(lua_State* L, vector<pair<unsigned int, GLenum>> format, int tableIndex = 4){
		vector<vType> verticies;
		
		//Attribute
		lua_pushnil(L);
		for(pair<unsigned int, GLenum> attr: format){
			if( lua_next(L, tableIndex) == 0)
				luaL_error(L, "Vertex had too few attributes");
			else{	
				lua_pushnil(L);
				for(int v = std::get<0>(attr); lua_next(L, -2) != 0; v-- ){

					//Format checking
					if( v <= 0 ) 
						luaL_error(L, "Vertex has too many values for attribute, expected %d", std::get<0>(attr));
					else if( std::get<1>(attr) == GL_INT && !lua_isinteger(L, -1))
						luaL_error(L, "Expected integer for vertex value");
					else if( std::get<1>(attr) == GL_FLOAT && !lua_isnumber(L, -1))
						luaL_error(L, "Expected number for vertex value");

					verticies.push_back( lua_tonumber(L, -1) );
					
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
			}
		}
		
		if( lua_next(L, tableIndex) != 0 )
			luaL_error(L, "Vetex has too many attribes");

		return verticies;
	}
	
	static int newVertArray(lua_State* L){
		if( !lua_checkstack(L, 8) )
			luaL_error(L, "Not enough stack space");
		if( !lua_istable(L, 1) )
			luaL_error(L, "Expected table of vertecies");
		
		vector<float> verticies;
		vector<unsigned int> indicies;
		vector<pair<unsigned int, GLenum>> format;
		
		/* Getting vao */
		unsigned int i = 0;

		lua_pushstring(L, "format");
		switch(lua_gettable(L, 1)){
			//Vertex array is preformatted
			case LUA_TTABLE: 
			{
				//TODO Get stuff
				
			}
			break;
			//Format of vertex array must be deduced
			case LUA_TNIL:   			
			{
				//Verticies
				lua_next(L, 1);
				format = getFormat(L, lua_gettop(L));
				break;
			}
			default:
				luaL_error(L, "Format for vertex array must be given as table");
			break;
		}

		lua_pop(L, 2);
		lua_pushnil(L);

		while(lua_next(L, 1) != 0){
			indicies.push_back(i++);
			auto vertex = getVerticies<float>(L, format);
			
			verticies.insert(verticies.end(), vertex.begin(), vertex.end());	

			lua_pop(L, 1);
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
	
		luaL_setmetatable(L, "Modulus.vertArray");
			
		return 1;
	}
	
	static int getVertex(lua_State* L){
		if( !lua_checkstack(L, 5) )
			luaL_error(L, "Not enough stack space");

		Modulus::VertArray* vao = (Modulus::VertArray*)luaL_checkudata(L, 1, "Modulus.vertexArray");
		luaL_argcheck(L, vao != nullptr, 1, "Expected vertex array");

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected index of vertex");
		int index = lua_tonumber(L, 2);
		
		vector<float> vertex = vao->getVertex<vector<float>>(index);

		lua_newtable(L);
		for( unsigned int i = 0; i < vertex.size(); i++){
			lua_pushnumber(L, vertex[i]);
			lua_pushnumber(L, index);
			lua_settable(L, -3);	
		}

		return 1;
	}
	static const struct luaL_Reg vertArrayLib [] = {
		{"new", newVertArray},
		{NULL, NULL}
	};

	static const struct luaL_Reg vertArrayMetaLib [] = {
		{"index", getVertex},
		{NULL, NULL}
	};
}
