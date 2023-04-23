#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include <glm/glm.hpp>

#include "SDL_GL.h"

#include "shader.h"

namespace Modulus::Parse::Lua{
	using namespace std;

	vector<Modulus::Shader*> gLuaShaders;
	
	static int newShader(lua_State* L){
		if(!lua_checkstack(L, 2))
			luaL_error(L, "Not enough stack space");

		luaL_argcheck(L, lua_istable(L, 1), 1, "Shader requires table as argument");
		
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

		/* Rough size estimate */
		size_t size =	  sizeof(LuaShader);
		LuaShader* newShader = (LuaShader*)lua_newuserdata(L, size);

		newShader = new (newShader) LuaShader(shaders, shaderName);
		newShader->loadProgram();

		gLuaShaders.push_back(newShader);
		
		luaL_setmetatable(L, "Modulus.shader");

		return 1;
	} 

	static int setShaderParameter(lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 1, "Modulus.shader");
		
		luaL_argexpected(L, lua_isstring(L, 2), 2, "parameter name");
		luaL_argcheck(L, lua_gettop(L) == 3, 3, "Expected a value for parameter");
		
		Modulus::Parameter param = newParameter(L);

		shader->bind();
		if( !shader->setParameter( param.name, param.type, param.value)){
			luaL_error(L, "Could not set parameter '%s'", param.name.c_str());
		}
		shader->unbind();
		return 0;
	}

	static int getShaderParameter( lua_State *L){
		if( !lua_checkstack(L, 7)) luaL_error(L, "Not enough stack space");
		
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 1, "Modulus.shader");
		luaL_argexpected(L, lua_isstring(L, 2), 2, "parameter name");

		pushParameter(L, shader->getParameter( lua_tostring(L, 2)));

		return 1;
	} 

	static const struct luaL_Reg shaderLib [] = {
		{"new", newShader},
		{NULL, NULL}
	};
	static const struct luaL_Reg shaderMetaLib [] = {
		{"set", setShaderParameter},
		{"get", getShaderParameter},
		{NULL, NULL}
	};
}
