#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "SDL_GL.h"
#include "input.h"

using namespace Modulus;

std::vector<Button*> Button::gButtons;

//Button constructor that takes the accepted sdl_event type
Button::Button(){
	
	state = 0;	
	
	gButtons.push_back(this);
}

//Button Constructor, adds a keycode to accepted keys
Button::Button(SDL_Scancode key){
	
	state = 0;

	addInput(key);

	gButtons.push_back(this);
}

Button::Button(const char* key){
	
	state = 0;

	addInput(key);

	gButtons.push_back(this);
}
Button::Button(std::initializer_list<SDL_Scancode> keys){
	
	state = 0;

	addInput(keys);

	gButtons.push_back(this);
}
Button::Button(std::initializer_list<const char*> keys){
	
	state = 0;

	addInput(keys);

	gButtons.push_back(this);
}
//Button::Button(std::initializer_list<SDL_Scancode>);
//Button::Button(std::initializer_list<const char*>);


//Adds a new key to the accepted input
void Button::addInput(SDL_Scancode newKey){	
	mScancodes.push_back(newKey);
}

void Button::addInput(const char* keyName){	
	SDL_Scancode newKey = SDL_GetScancodeFromName(keyName);

	if(newKey == SDL_SCANCODE_UNKNOWN){
		std::cout << "Button: Error adding key '" << keyName 
				  << "'. SDL Error: " << SDL_GetError() << std::endl;
	}
	else{
		mScancodes.push_back(newKey);
	}
}

void Button::addInput( std::vector<SDL_Scancode> keys ){
	for( auto k : keys)
		addInput(k);
}
void Button::addInput( std::vector<const char*> keys ){
	for( auto k : keys)
		addInput(k);
}

//Receives an sdl event and updates buttion state appropriately
void Button::update(const Uint8* keystate){
	
	//Seach if accepted key
	for(unsigned int k = 0; k < mScancodes.size(); k++){
		if(keystate[mScancodes[k]]){
			if(state == 0)
				state = 1;
			else if(state == 1)
				state = 2;
		}
		else{
			state = 0;
		}
	}
}

//Returns the current button state
int Button::getState() const{
	return state;
}

//Cursor constructor
Cursor::Cursor(){}

//Creates a mouse button and adds it to the list and returns it's adress
MouseButton* Cursor::addButton(SDL_Rect* rect){

	//Dynamacally create button
	MouseButton* newButton = new MouseButton;
	
	//Set values
	newButton->rect = rect;
	newButton->state = UNTOUCHED;

	//Add to list
	buttonList.push_back(newButton);
	
	//Debug:
	/*cout << "Cursor::addButton: Returns: " << newButton << endl
		 << "Cursor::addButton: MouseButton.rect points to:" << newButton->rect << endl;*/

	//Return address
	return newButton;
}

//Takes mouse cursor coordinates and which button is pressed to update all buttons on list
void Cursor::update(int x, int y, unsigned int state, unsigned int pressed){
	
	//Update coordinates
	coords.x += x;
	coords.y += y;

	//Find given mouse button 
	for(unsigned int i = 0; i < buttonList.size(); i++){
		//Mouse is over the button
		if(SDL_PointInRect(&coords, buttonList[i]->rect)){
			//A button on the mouse is being pressed
			if(   state == SDL_PRESSED
			   || state == SDL_BUTTON_LMASK
			   /*|| state & SDL_BUTTON(SDL_BUTTON_MIDDLE)
			   || state & SDL_BUTTON(SDL_BUTTON_RIGHT)*/
			   ){
				if(pressed == SDL_BUTTON_LEFT)
					buttonList[i]->state = CLICKEDL;
				else if(pressed == SDL_BUTTON_MIDDLE)
					buttonList[i]->state = CLICKEDM;
				else if(pressed == SDL_BUTTON_RIGHT)
					buttonList[i]->state = CLICKEDR;
			}
			else if(state == SDL_RELEASED){
				buttonList[i]->state = HOVERED;
			}
		}
		else{
			buttonList[i]->state = UNTOUCHED;
		}
	}
}

//Cursor Deconstructor
Cursor::~Cursor(){

	//Clear out button list
	for(unsigned int i = 0; i < buttonList.size(); i++)
		delete buttonList[i];
	buttonList.clear();	
}
