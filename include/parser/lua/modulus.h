#pragma once

#include <vector>
#include <map>
#include <string>

#include <lua5.4/lua.hpp>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include "VertArray.h"
#include "mesh.h"
#include "shader.h"
#include "framebuffer.h"

#include "parser/lua/mesh.h"

namespace Modulus::Parse::Lua{

	GameManager& getModulusContext( lua_State* L){
		lua_pushstring(L, "Modulus.GameManager");
		lua_gettable(L, LUA_REGISTRYINDEX);

		return *(GameManager*)lua_touserdata(L, -1);
	}

	static int getScreenDimenstions( lua_State* L ){
		
		GameManager& modulusContext = getModulusContext(L);

		//Width
		lua_pushinteger(L, modulusContext.getScreenWidth());

		//Height
		lua_pushinteger(L, modulusContext.getScreenHeight());

		return 2;
	}

	std::map<std::string, GLenum> drawModes{
		{"points", GL_POINTS},

		{"lines", GL_LINES},
		{"lines_adjacency", GL_LINES_ADJACENCY},
		{"line_loop", GL_LINE_LOOP},
		{"line_strip", GL_LINE_STRIP},
		{"line_strip_adjacency", GL_LINE_STRIP_ADJACENCY},

		{"triangles", GL_TRIANGLES},
		{"triangles_adjacency", GL_TRIANGLES_ADJACENCY},
		{"triangle_strip", GL_TRIANGLE_STRIP},
		{"triangle_strip_adjacency", GL_TRIANGLE_STRIP_ADJACENCY},
		{"triangle_fan", GL_TRIANGLE_FAN},

		{"patches", GL_PATCHES} 
	};

	static int draw( lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 1, "Modulus.shader");

		Modulus::Mesh* mesh = (Modulus::Mesh*)luaL_testudata(L, 2, "Modulus.mesh");

		int fbIndex = lua_isstring(L, -1) ? -2 : -1;
		Modulus::FrameBuffer* framebuffer = (Modulus::FrameBuffer*)luaL_checkudata(L, fbIndex, "Modulus.framebuffer");
		
		/* Render vertex array and textures */
		if( mesh == NULL){	
			GLenum drawMode = GL_TRIANGLES;
			if( lua_gettop(L) == 5 ){
				luaL_argcheck(L, lua_isstring(L, 5), 5, "Expected the draw mode as a string");	

				std::string drawModeStr(lua_tostring(L, 5));
				luaL_argcheck(L, drawModes.find(drawModeStr) != drawModes.end(), 
							  5, std::string("'" + drawModeStr + "' is not a valid draw mode").c_str());	

				drawMode = drawModes[drawModeStr];
			}

			Modulus::VertArray* vao = (Modulus::VertArray*)luaL_checkudata(L, 2, "Modulus.vertexArray");
			
			luaL_argcheck(L, lua_istable(L, 3), 3, "Expected table of materials");
			std::vector<Modulus::Material> materials = getMaterials(L, 3);
			
			Modulus::GameManager& modulusContext = getModulusContext(L);	
			modulusContext.draw( *shader, *vao, materials, *framebuffer, drawMode);
		}
		else{
			Modulus::GameManager& modulusContext = getModulusContext(L);	
			modulusContext.draw( *shader, *mesh, *framebuffer);
		}

		lua_settop(L, 0);

		return 0;
	}


	
	static const struct luaL_Reg gameManagerLib [] = {
		{"getWindow", getScreenDimenstions},
		{"draw", draw},
		{NULL, NULL}
	};
}

//draw(Shader&, Mesh&, FrameBuffer&);
//draw(Shader& s, FrameBuffer& srcFB, FrameBuffer& destFB)
			
