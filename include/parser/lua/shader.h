#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "SDL_GL.h"

#include "shader.h"

namespace Modulus::Parse::Lua{
	using namespace std;

	vector<Modulus::Shader*> gLuaShaders;
	
	static int newShader(lua_State* L){
		if(!lua_checkstack(L, 2))
			luaL_error(L, "Not enough stack space");
		if( !lua_istable(L, 1) )
			luaL_error(L, "Shader requires table as argument");
		
		string shaderName;
		lua_pushstring(L, "name");
		if( lua_gettable(L, 1) == LUA_TSTRING ){
			shaderName = lua_tostring(L, -1);
			lua_pop(L,1); 
		}

		/* Retrieve shader sources */
		vector<tuple<GLenum, string>> shaders;
		lua_pushstring(L, "vertex");
		if( lua_gettable(L, 1) == LUA_TSTRING ){
			shaders.push_back( { GL_VERTEX_SHADER, lua_tostring(L, -1) });
			lua_pop(L,1); 
		}
		
		lua_pushstring(L, "geometry");
		if( lua_gettable(L, 1) == LUA_TSTRING ){
			shaders.push_back( { GL_GEOMETRY_SHADER, lua_tostring(L, -1) });
			lua_pop(L,1); 
		}

		lua_pushstring(L, "fragment");
		if( lua_gettable(L, 1) == LUA_TSTRING ){
			shaders.push_back( { GL_FRAGMENT_SHADER, lua_tostring(L, -1) });	
			lua_pop(L,1); 
		}

		lua_pushstring(L, "compute");
		if( lua_gettable(L, 1) == LUA_TSTRING ){
			shaders.push_back( { GL_COMPUTE_SHADER, lua_tostring(L, -1) });	
			lua_pop(L,1); 
		}

		/* Rough size estimate */
		size_t size =	  sizeof(Modulus::Shader);

		Modulus::Shader* newShader = (Modulus::Shader*)lua_newuserdata(L, size);

		class LuaShader : public Modulus::Shader{
			public:
			LuaShader( vector<tuple<GLenum, string>> sources, string name = "Lua Shader") : Shader(){
				mSources = sources;
				mName = name;
			}

			bool loadProgram(){
				return compileShaders(mSources);
			}

			private:
			vector<tuple<GLenum, string>> mSources;
		};
		newShader = new LuaShader(shaders, shaderName);

		gLuaShaders.push_back(newShader);
		
		luaL_setmetatable(L, "Modulus.shader");

		return 1;
	}

	static const struct luaL_Reg shaderLib [] = {
		{"new", newShader},
		{NULL, NULL}
	};
}
