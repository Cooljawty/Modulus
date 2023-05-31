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
#include "parser/lua/utilities.h"

namespace Modulus::Parse::Lua{

	GameManager& getModulusContext( lua_State* L){
		lua_pushstring(L, "Modulus.GameManager");
		lua_gettable(L, LUA_REGISTRYINDEX);

		return *(GameManager*)lua_touserdata(L, -1);
	}

	static int getScreenDimenstions( lua_State* L ){
		
		GameManager& modulusContext = getModulusContext(L);

		lua_pushinteger(L, modulusContext.getScreenWidth());
		lua_pushinteger(L, modulusContext.getScreenHeight());

		return 2;
	}

	static int toggleState( lua_State* L ){
		
		GameManager& modulusContext = getModulusContext(L);

		modulusContext.toggleRunning();

		return 0;
	}

	//Draws mesh or unassociated vertex array and texutures to framebuffer
	static int draw( lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 1, "Modulus.shader");

		Modulus::Mesh* mesh = (Modulus::Mesh*)luaL_testudata(L, 2, "Modulus.mesh");

		//If a draw mode is specified then the framebuffer will be the second to last argument
		int fbIndex = lua_isstring(L, -1) ? -2 : -1;
		Modulus::FrameBuffer* framebuffer = (Modulus::FrameBuffer*)luaL_checkudata(L, fbIndex, "Modulus.framebuffer");
		
		//Draw vertex array, with textures
		if( mesh != NULL){	
			Modulus::GameManager& modulusContext = getModulusContext(L);	
			modulusContext.draw( *shader, *mesh, *framebuffer);
		}
		else{

			//Draw mode is triangles by default
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

		lua_settop(L, 0);

		return 0;
	}

	static int blendFrameBuffers(lua_State* L){
		if( !lua_checkstack(L, 7)){
			luaL_error(L, "Not enough stack space");
		}
		
		Modulus::Shader* shader = (Modulus::Shader*)luaL_checkudata(L, 1, "Modulus.shader");

		Modulus::FrameBuffer* srcFramebuffer  = (Modulus::FrameBuffer*)luaL_testudata(L, 2, "Modulus.framebuffer");
		Modulus::FrameBuffer* destFramebuffer = (Modulus::FrameBuffer*)luaL_testudata(L, 3, "Modulus.framebuffer");

		Modulus::GameManager& modulusContext = getModulusContext(L);	
		modulusContext.draw( *shader, *srcFramebuffer, *destFramebuffer);

		return 0;
	}

	static const struct luaL_Reg gameManagerLib [] = {
		{"getWindow", getScreenDimenstions},
		{"draw", draw},
		{"blend", blendFrameBuffers},
		{"quit", toggleState},
		{NULL, NULL}
	};
}
