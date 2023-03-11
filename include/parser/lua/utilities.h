#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

namespace Modulus::Parse::Lua{
	using namespace std;

	void stackDump(lua_State* L){
		int top = lua_gettop(L);
		
		std::cout << "Current Lua Stack:" << std::endl;

		for( int s = 1; s <= top; s++){ 
			int recentType = lua_type(L, s);
			switch( recentType ){
				case LUA_TSTRING:	
					cout << s << ":" << lua_tostring(L, s) << endl;
					break;	
				case LUA_TNUMBER:	
					cout << s << ": " << lua_tonumber(L, s) << endl;
					break;
				case LUA_TBOOLEAN:	
					cout << s << ": boolean " << (lua_toboolean(L, s) == 0 ? "false" : "true") << endl;
					break;
				case LUA_TNIL:
					cout << s << ": Nil value" << endl;
					break;
				default:
					cout << s << ": " << lua_typename(L, recentType) << endl;
					break;
			}
		}
	}
	
	static vector<pair<unsigned int, GLenum>> getFormat(lua_State* L, int tableindex){
		vector<pair<unsigned int, GLenum>> format;
		
		lua_pushnil(L);
		while( lua_next(L, tableindex) != 0 ){ 
			lua_pushnil(L);
			int s = 0;
			
			//Attribs
			GLenum type;
			while(lua_next(L, -2) != 0){
				if( lua_isinteger(L, -1)){
					type = ( type == GL_FLOAT ? GL_FLOAT : GL_INT );
				}
				else if( lua_isnumber(L, -1)){
					type = GL_FLOAT;
				}
				else{
					luaL_error(L, "Expected number for table value");
				}
				
				lua_pop(L, 1);
				s++;
			}
			format.push_back( { s, type } );
			lua_pop(L, 1);
		}

		return format;
	}
	
}
