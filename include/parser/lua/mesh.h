#pragma once

#include <vector>
#include <tuple>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "SDL_GL.h"

#include "data_types.h"
#include "mesh.h"

#include "parser/lua/VertArray.h"

namespace Modulus::Parse::Lua{
	std::vector<Modulus::Mesh*> gLuaMeshes;
	
	static Modulus::Material getMaterial(lua_State* L){
		Modulus::Material material;
		
		lua_pushstring(L, "path");
		if( lua_gettable(L, -2) != LUA_TSTRING ){
			luaL_error(L, "Expected path to material texture");
			return material;
		}

		material.path = lua_tostring(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "type");
		if( lua_gettable(L, -2) != LUA_TSTRING )
			material.type = "texture";
		else
			material.type = lua_tostring(L, -1);

		lua_pop(L, 1);
		
		material.texture = new Texture();
		if(!material.texture->loadFromImage(material.path)){
			luaL_error(L, "Could not load texture from image '%s'", material.path);
		}

		return material;
	}
	
	static int newMesh(lua_State* L){
		if(!lua_checkstack(L, 6)) luaL_error(L, "Not enough stack space");

		luaL_argcheck(L, lua_istable(L, 1), 1, "Expected table of verticies");
		luaL_argcheck(L, lua_istable(L, 2), 2, "Expected table of materials");
		
		newVertArray(L);
		Modulus::VertArray* vao = (Modulus::VertArray*)lua_touserdata(L, -1);
		luaL_argcheck(L, vao != NULL, 1, "Failed to retrive vertex array");

		lua_pop(L, 1);
			
		/* Getting vertex array */
		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		
		indicies = vao->getIndexBuffer();

		/* Getting materials */
		vector<Modulus::Material> materials;

		lua_pushnil(L);
		while(lua_next(L, 2) !=0){
			materials.emplace_back( getMaterial(L));		
			
			lua_pop(L, 1);
		}

		/* Rough size estimate */
		size_t size =	  sizeof(Modulus::Mesh)
						+ sizeof(unsigned int) * indicies.size() 
				 	 	+ sizeof(Modulus::Material) * materials.size();

		Modulus::Mesh* newMesh = (Modulus::Mesh*)lua_newuserdata(L, size);
		newMesh = new (newMesh) Mesh(*vao, materials, GL_TRIANGLES);
		gLuaMeshes.push_back(newMesh);
		
		luaL_setmetatable(L, "Modulus.mesh");

		return 1;
	}

	static const struct luaL_Reg meshLib [] = {
		{"new", newMesh},
		{NULL, NULL}
	};
}
