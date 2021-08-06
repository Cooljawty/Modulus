#pragma once
#include "SDL_GL.h"

namespace Modulus{

	//Keyboard and General IButton input
	class Button{
		public:
			//IButton Constructor
			Button();
			
			//IButton Constructor, adds a keycode to accepted keys
			Button(SDL_Scancode);

			//Adds new key to accepted input
			void addInput(SDL_Scancode);

			//Checks if assoiated button is pressed and updates state 
			void update(const Uint8* keystate);

			//Returns the button state
			int getState() const;

		private:
			//The button state: 0 Unpressed, 1 Down, 2 Held
			int state;

			//Dynamic array of accepted keys
			std::vector<SDL_Scancode> mScancodes;
		
		public:
			static std::vector<Button*> gButtons;
	};

	//Enumerates mouse button states
	enum mouseButtonState{
		UNTOUCHED = 0,
		HOVERED = 1,
		CLICKEDL = 2, //Left clicked
		CLICKEDM = 3, //Middle clicked
		CLICKEDR = 4  //Right clicked
	};

	//A mouse button with the bound and clicked state
	struct MouseButton{
		
		//Pointer to a rect reprisenting the bounds of the rect
		SDL_Rect* rect;

		//The state of the button
		mouseButtonState state;
	};
		
	//Clickable buttons on screen
	class Cursor{

		public:
			//Constructor
			Cursor();
			
			//Deconstructor
			~Cursor();
			
			//Returns the coordinate point
			SDL_Point* getCoords() {return &coords;}
			void getCoords(int& x, int& y) { x = coords.x; y = coords.y; }

			//Adds a mouse button to list returning it's memory adress
			MouseButton* addButton(SDL_Rect*);

			//Updates the states of all mouse buttons on the list
			void update(int, int, unsigned int, unsigned int=0);

		private:
			
			//Coordinates of the mouse on the screen
			SDL_Point coords;

			//List of rects to detect mouse clicking
			std::vector<MouseButton*> buttonList;
	};

	//Analog input
	/*class LAnalog{};*/
}
