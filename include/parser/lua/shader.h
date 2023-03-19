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

	static int setParameter(lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		if( lua_gettop(L) != 3){
			luaL_error(L, "Expected a shader, parameter name and value as arguments");
		}
		
		string name = lua_tostring(L, 2);
		luaL_argcheck(L, name.c_str() != NULL, 2, "Expected parameter name for argument 2");

		Modulus::Shader* shader = (Modulus::Shader*)lua_touserdata(L, 1);
		luaL_argcheck(L, shader != nullptr, 1, "Expected shader");
		
		void* value;
		unsigned int type = lua_type(L, 3);
		switch( type ){
			case LUA_TBOOLEAN:
				type = GL_BOOL;
				value = new bool(lua_toboolean(L, 3));
				break;
			//Scalar
			case LUA_TNUMBER:
				{
					if( lua_isinteger(L, 3) ){
						type = GL_INT;
						value = new int(lua_tonumber(L, 3));
					}
					else{
						type = GL_FLOAT;
						value = new float(lua_tonumber(L, 3));
					}
					break;
				}
			//Array
			case LUA_TTABLE:
				{
					auto format = getFormat(L, 3);		
					
					//Format is vector
					if( format.size() == 1){
						unsigned int size = std::get<0>(format.front());
						vector<float> vec;

						lua_pushnil(L);
						for( unsigned int n = 0; lua_next(L, 3); n++ ){
							vec[n] = lua_tonumber(L, -1);
							lua_pop(L, 1);
						}
						lua_pop(L, 1);
						if( size == 2){ 
							type = GL_FLOAT_VEC2;
							value = new glm::vec2( glm::make_vec2(vec.data()) );
						}
						else if( size == 3){ 
							type = GL_FLOAT_VEC3; 
							value = new glm::vec3( glm::make_vec3(vec.data()) );
						}
						else if( size == 4){ 
							type = GL_FLOAT_VEC4; 
							value = new glm::vec4( glm::make_vec4(vec.data()) );
						}
						else
							luaL_error(L, "Parameter cannot be a vector with size %i", size);
						
					}
					else{
						vector<float> mat;
						lua_pushnil(L);
						while(lua_next(L, 3)){
							lua_pushnil(L);
							while(lua_next(L, -2)){
								mat.push_back(lua_tonumber(L, -1));
								lua_pop(L, 1);
							}
							lua_pop(L,1);
						}
						lua_pop(L,1);

						//format is matrix
						if( format.size() == 2){
							type = GL_FLOAT_MAT2; 
							value = new glm::mat2( glm::make_mat2<float>(mat.data()) );
						}
						else if( format.size() == 3){
							type = GL_FLOAT_MAT3; 
							value = new glm::mat3( glm::make_mat3<float>(mat.data()) );
						}
						else if( format.size() == 4){
							type = GL_FLOAT_MAT4; 
							value = new glm::mat4( glm::make_mat4<float>(mat.data()) );
						}
						
					}
					break;
				}
			default:
				luaL_error(L, "Parameter '%s' could not be set to type %s", 
							name, lua_typename(L, type));
		}
		
		shader->bind();
		if( !shader->setParameter( lua_tostring(L, 2), type, value, false)){
			luaL_error(L, "Could not set parameter %s", name.c_str());
		}
		shader->unbind();
		return 0;
	}

	static const struct luaL_Reg shaderLib [] = {
		{"new", newShader},
		{NULL, NULL}
	};
	static const struct luaL_Reg shaderMetaLib [] = {
		{"set", setParameter},
		{NULL, NULL}
	};
}
