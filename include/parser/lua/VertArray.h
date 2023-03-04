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
				lua_pushnil(L);
				while(lua_next(L, -2) != 0){ 
					lua_pushnil(L);
					int s = 0;
					//Attribs
					GLenum type = 0;
					while(lua_next(L, -2) != 0){
						s++;
						if( lua_isinteger(L, -1)){
							type = ( type == GL_FLOAT ? GL_FLOAT : GL_INT );
						}
						else if( lua_isnumber(L, -1)){
							type = GL_FLOAT;
						}
						else{
							luaL_error(L, "Expected number for vertex attribute");
						}
						
						lua_pop(L, 1);
					}
					format.push_back( { s, type } );
					lua_pop(L, 1);
				}
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

	static const struct luaL_Reg vertArrayLib [] = {
		{"new", newVertArray},
		{NULL, NULL}
	};
}
