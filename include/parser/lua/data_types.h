#pragma once

#include <vector>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "data_types.h"

#include "parser/lua/utilities.h"

namespace Modulus::Parse::Lua{
	Parameter newParameter( lua_State* L){

		string name = lua_tostring(L, -2);
		
		void* value = nullptr;

		//Type deduction
		unsigned int type = lua_type(L, -1);
		switch( type ){
			case LUA_TBOOLEAN:
			{
				type = GL_BOOL;
				value = new bool(lua_toboolean(L, -1));
				break;
			}
			//Scalar
			case LUA_TNUMBER:
			{
				if( lua_isinteger(L, -1) ){
					type = GL_INT;
					value = new int(lua_tonumber(L, -1));
				}
				else{
					type = GL_FLOAT;
					value = new float(lua_tonumber(L, -1));
				}
				break;
			}
			//Array
			case LUA_TTABLE:
			{
				auto format = getFormat(L, lua_gettop(L));
				
				//Format is vector
				if( format.size() == 1){
					unsigned int size = std::get<0>(format.front());
					vector<float> vec;

					lua_pushnil(L);
					for( unsigned int n = 0; lua_next(L, -2); n++ ){
						vec.push_back(lua_tonumber(L, -1));
						lua_pop(L, 1);
					}

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
					while(lua_next(L, -2)){
						lua_pushnil(L);
						while(lua_next(L, -2)){
							mat.push_back(lua_tonumber(L, -1));
							lua_pop(L, 1);
						}
						lua_pop(L,1);
					}

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

		Parameter param;
		param.name = name;
		param.type = type;
		param.value = value;
		return param;
	}	

	static int pushParameter( lua_State* L, Parameter param){
		
		lua_pushstring(L, param.name.c_str());
		switch( param.type ){
			//Boolean
			case GL_BOOL:
				lua_pushboolean(L, *(bool*)param.value ? 1 : 0);
				break;

			//Scalar
			case GL_INT:
				lua_pushinteger(L, *(int*)param.value);
				break;

			case GL_FLOAT:
				lua_pushnumber(L, *(float*)param.value);
				break;

			//Vectors
			case GL_FLOAT_VEC2:
			{
				lua_createtable(L, 2, 0);
				for( int i = 0; i < 2; i++){
					lua_pushinteger(L, i+1);
					lua_pushnumber(L, (*(glm::vec2*)param.value)[i]);
					lua_settable(L, -3);
				} 
				break;
			}
			case GL_FLOAT_VEC3:
			{
				lua_createtable(L, 3, 0);
				for( int i = 0; i < 3; i++){
					lua_pushinteger(L, i+1);
					lua_pushnumber(L, (*(glm::vec3*)param.value)[i]);
					lua_settable(L, -3);
				} 
				break;
			}
			case GL_FLOAT_VEC4:
			{
				lua_createtable(L, 4, 0);
				for( int i = 0; i < 4; i++){
					lua_pushinteger(L, i+1);
					lua_pushnumber(L, (*(glm::vec4*)param.value)[i]);
					lua_settable(L, -3);
				} 
				break;
			}
			// Matricies
			case GL_FLOAT_MAT2:
			{
				lua_createtable(L, 2, 0); 
				for( int i = 0; i < 2; i++){
					lua_pushinteger(L, i+1); 

					lua_createtable(L, 2, 0); 
					for( int j = 0; j < 2; j++){
						lua_pushinteger(L, j+1); 
						lua_pushnumber(L, (*(glm::mat2*)param.value)[i][j]); 
						lua_settable(L, -3);
					}

					lua_settable(L, -3);
				} 
				break;
			}
			case GL_FLOAT_MAT3:
			{
				lua_createtable(L, 3, 0); 
				for( int i = 0; i < 3; i++){
					lua_pushinteger(L, i+1); 

					lua_createtable(L, 3, 0); 
					for( int j = 0; j < 3; j++){
						lua_pushinteger(L, j+1); 
						lua_pushnumber(L, (*(glm::mat3*)param.value)[i][j]); 
						lua_settable(L, -3);
					}

					lua_settable(L, -3);
				} 
				break;
			}
			case GL_FLOAT_MAT4:
			{
				lua_createtable(L, 4, 0); 
				for( int i = 0; i < 4; i++){
					lua_pushinteger(L, i+1); 

					lua_createtable(L, 4, 0); 
					for( int j = 0; j < 4; j++){
						lua_pushinteger(L, j+1); 
						lua_pushnumber(L, (*(glm::mat4*)param.value)[i][j]); 
						lua_settable(L, -3);
					}

					lua_settable(L, -3);
				} 
				break;
			}
		}

		return 2;
	}	
}
