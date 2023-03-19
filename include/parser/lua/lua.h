#pragma once

#include <iostream>
#include <string>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "parser/lua/mesh.h"
#include "parser/lua/shader.h"
#include "parser/lua/framebuffer.h"

namespace Modulus::Parse::Lua{
	class Context{
	public:	
		Context(){
			mLuaContext = nullptr;
		}

		bool init(){
			mLuaContext = luaL_newstate();

			if(mLuaContext == NULL){
				std::cerr << "Lua Context: Could not allocate enough memory to create lua state" << std::endl;
				return false;
			}

			/* Open standard libraries */
			luaL_openlibs(mLuaContext);			

			/* Loading modulus libraries*/
			loadLib<1,0>("mesh", meshLib); 
			loadLib<1,0>("vertArray", vertArrayLib); 
			loadLib<1,1>("shader", shaderLib, shaderMetaLib); 
			loadLib<1,0>("framebuffer", frameBufferLib);

			return true;
		}
		
		template <int N, int M>
		void loadLib(string name, const struct luaL_Reg (&libReg)[N+1], const struct luaL_Reg (&metaReg)[M+1] = {{NULL,NULL}}){
			
			//Create metatable with metamethods
			luaL_newmetatable(mLuaContext, ("Modulus." + name).c_str()); 
			if(M != 0) luaL_setfuncs(mLuaContext, metaReg, 0);
			
			//Set metatable index to itself
			lua_pushstring(mLuaContext, "__index");
			lua_pushvalue(mLuaContext, -2);
			lua_settable(mLuaContext, -3);
			
			
			//Add functions to global object
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
