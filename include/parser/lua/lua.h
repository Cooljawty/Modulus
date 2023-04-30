#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "parser/lua/mesh.h"
#include "parser/lua/shader.h"
#include "parser/lua/framebuffer.h"
#include "parser/lua/input.h"

#include "modulus.h"

namespace Modulus::Parse::Lua{
	class Context{
	public:	
		Context( Modulus::GameManager& manager ) : mModulusContext(manager){
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

			/* Store modulus context to Lua Registry */
			lua_pushstring(mLuaContext, "Modulus.GameManager");
			lua_pushlightuserdata(mLuaContext, (void*)&mModulusContext);
			lua_settable(mLuaContext, LUA_REGISTRYINDEX);

			/* Loading modulus libraries*/
			loadLib<1,2>("mesh", meshLib, meshMetaLib); 
			loadLib<1,1>("vertexArray", vertArrayLib, vertArrayMetaLib); 
			loadLib<1,2>("shader", shaderLib, shaderMetaLib); 
			loadLib<1,0>("framebuffer", frameBufferLib);

			loadLib<2,0>("gameManager", gameManagerLib);
			loadLib<1,5>("button", buttonLib, buttonMetaLib);

			return true;
		}
		
		template <int N, int M>
		void loadLib(string name, const struct luaL_Reg (&libReg)[N+1], const struct luaL_Reg (&metaReg)[M+1] = {{NULL,NULL}}){
			
			//Create metatable with metamethods
			luaL_newmetatable(mLuaContext, ("Modulus." + name).c_str()); 

			//Set metatable's default index metamethod to itself
			if( M == 0 || std::string(metaReg[0].name) != "index"){
				lua_pushstring(mLuaContext, "__index");
				lua_pushvalue(mLuaContext, -2);
				lua_settable(mLuaContext, -3);	
			}

			//Add functions to global object
			luaL_newlib(mLuaContext, libReg);
			lua_setglobal(mLuaContext, name.c_str());
	
			if(M != 0){
				static const std::array<string,29> metamethodNames = {
					"add", "sub", "mul", "div", "mod", "pow", "unm", "idiv",
					"band", "bnot", "bor", "bxor", "shl", "shr",
					"concat", "len",
					"eq", "lt", "le", 
					"index", "newindex",
					"call",
					"gc", "close", "mode", "name",
					"tostring", "metatable", "pairs"
				};

				luaL_newlib(mLuaContext, metaReg);	
				for( int i = 0; i < M; i++){
					string methodName = metaReg[i].name;
					bool isMetamethod = std::find(metamethodNames.begin(), metamethodNames.end(), methodName) != std::end(metamethodNames);
					lua_pushstring(mLuaContext, ( (isMetamethod ? "__" : "") + methodName).c_str());
					lua_pushstring(mLuaContext, (methodName).c_str());
					lua_gettable(mLuaContext, -3);
					lua_settable(mLuaContext, 1);
				}
				lua_pop(mLuaContext, 1);

			}
			
			lua_settop( mLuaContext, 0 );
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

		GameManager& mModulusContext;

	};
	
}
