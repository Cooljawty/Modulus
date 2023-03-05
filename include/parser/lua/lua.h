#pragma once

#include <iostream>
#include <string>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "parser/lua/mesh.h"
#include "parser/lua/shader.h"

namespace Modulus::Parse::Lua{
	class Context{
	public:	
		Context(){
			mLuaContext = nullptr;
		}

		bool init(){
			mLuaContext = luaL_newstate();	/* opens Lua */

			if(mLuaContext == NULL){
				std::cerr << "Lua Context: Could not allocate enough memory to create lua state" << std::endl;
				return false;
			}

			luaL_openlibs(mLuaContext);     /* opens all standard libraries */
			
			/* Loading libraries*/
			luaL_newmetatable(mLuaContext, "Modulus.mesh"); 
			loadLib<2>("mesh", meshLib); 

			luaL_newmetatable(mLuaContext, "Modulus.vertArray"); 
			loadLib<2>("vertArray", vertArrayLib); 
			
			luaL_newmetatable(mLuaContext, "Modulus.shader"); 
			loadLib<2>("shader", shaderLib); 
		
			return true;
		}
		
		template <int N>
		void loadLib(string name, const struct luaL_Reg (&libReg)[N]){
			luaL_newlib(mLuaContext, libReg);
			lua_setglobal(mLuaContext, name.c_str());
	
		}	

		bool loadFile(string path){
			int result =  luaL_loadfile(mLuaContext, path.c_str())
						| lua_pcall(mLuaContext, 0, LUA_MULTRET, 0);
			if(result != LUA_OK){
				std::cerr << "Lua Context: Error loading file '" + path + "'" << std::endl
						  << lua_tostring(mLuaContext, -1) << std::endl;
				return false;
			}
			return true;
		}

		bool loadChunk(string currChunk){
			int result =  luaL_loadbuffer(mLuaContext, currChunk.c_str(), currChunk.length(), "line") 
						| lua_pcall(mLuaContext, 0, LUA_MULTRET, 0);
		
			if (result != LUA_OK) {
				std::cerr << "Lua Context: Error parsing chunk" << std::endl
						  << lua_tostring(mLuaContext, -1) << std::endl;
				return false;
			}

			return true;
		}

		~Context(){
			close();
		}
		
		void close(){
			lua_close(mLuaContext);
		}
		

	private:	
		lua_State* mLuaContext;

	};
	
}
