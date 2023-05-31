#pragma once
#include "input.h"

namespace Modulus::Parse::Lua{
	
	std::vector< Modulus::Button* > gLuaButtons;

	static int newButton(lua_State* L){

		std::vector< const char* > keys;
		for(int arg = 1; arg <= lua_gettop(L); arg++){
			luaL_argexpected(L, lua_isstring(L, arg), arg, "key as string");
			keys.push_back( lua_tostring(L, arg) ); 
		}
		
		Modulus::Button* newButton = (Modulus::Button*)lua_newuserdata(L, sizeof(Modulus::Button));
		newButton = new (newButton) Modulus::Button;
		newButton->addInput( keys );

		luaL_setmetatable(L, "Modulus.button");

		gLuaButtons.push_back( newButton );

		return 1;
	}

	Modulus::Button& getButton(lua_State* L, int index){
		auto* button = (Modulus::Button*)luaL_checkudata(L, index, "Modulus.button");
		return *button;
	}
	
	static int addKey(lua_State* L){
		if( !lua_checkstack(L, 2) ) luaL_error(L, "Not enough stack space"); 

		Modulus::Button& button = getButton(L, 1);

		std::vector< const char* > keys;
		for(int arg = 1; arg < lua_gettop(L); arg++){
			luaL_argexpected(L, lua_isstring(L, arg), arg, "key as string");
			keys.push_back( lua_tostring(L, arg) ); 
		}
		
		button.addInput( keys );

		return 0;
	}
	static int keyUp(lua_State* L){
		if( !lua_checkstack(L, 1) ) luaL_error(L, "Not enough stack space"); 

		Modulus::Button& button = getButton(L, 1);

		lua_pushboolean(L, button.getState() == 0);
		return 1;
	}
	static int keyDown(lua_State* L){
		if( !lua_checkstack(L, 1) ) luaL_error(L, "Not enough stack space"); 

		Modulus::Button& button = getButton(L, 1);

		lua_pushboolean(L, button.getState() != 0);
		return 1;
	}

	static int keyPressed(lua_State* L){
		if( !lua_checkstack(L, 1) ) luaL_error(L, "Not enough stack space"); 

		Modulus::Button& button = getButton(L, 1);

		lua_pushboolean(L, button.getState() == 1);
		return 1;
	}
	static int keyHeld(lua_State* L){
		if( !lua_checkstack(L, 1) ) luaL_error(L, "Not enough stack space"); 

		Modulus::Button& button = getButton(L, 1);

		lua_pushboolean(L, button.getState() == 2);
		return 1;
	}

	const struct luaL_Reg buttonLib [] ={
		{"new", newButton},
		{NULL, NULL}
	};

	const struct luaL_Reg buttonMetaLib[] = {
		{"addKey", addKey},
		{"up", keyUp},
		{"pressed", keyPressed},
		{"down", keyDown},
		{"held", keyHeld},
		{NULL, NULL}
	};
}
