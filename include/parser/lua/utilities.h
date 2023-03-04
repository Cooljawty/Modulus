#pragma once

#include <iostream>
#include <string>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

namespace Modulus::Parse::Lua{
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
}
