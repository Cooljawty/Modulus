#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "physics.h"

using namespace std;

Vector2D& Vector2D::operator+=(Vector2D vector){

	//Calculate combined vectors' x and y components
	double xComp = getXComp() + vector.getXComp();
	double yComp = getYComp() + vector.getYComp();
	
	//Find final magitude and direction
	force = sqrt((xComp * xComp) + (yComp * yComp));
	angle = (xComp == 0.0 && yComp == 0.0) ? 0.0 : atan2(yComp,xComp);

	return *this;
}
