#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

#include "transform.h"

//Handles collisions between two objects
void collision(Transform& a, Transform& b){
	
	SDL_Rect dR;

	//Check for collision
	if(!SDL_IntersectRect(&a.dstR,&b.dstR, &dR))
		return;
	
	//Determine positions of each rect
	if(a.gX < b.gX){
		double dX = (a.gX + a.dstR.w/2 - b.gX + b.dstR.w/2)/2;
		a.gX -= dX;
		b.gX += dX;
	}
	else{
		double dX = (b.gX + b.dstR.w/2 - a.gX + a.dstR.w/2)/2;
		a.gX += dX;
		b.gX -= dX;
	}
	if(a.gY < b.gY){
		double dY = (a.gY + a.dstR.h/2 - b.gY + b.dstR.h/2)/2;
		a.gY += dY;
		b.gY -= dY;
	}
	else{
		double dY = (b.gY + b.dstR.h/2 - a.gY + a.dstR.h/2)/2;
		a.gY -= dY;
		b.gY += dY;
	}
	
	std::cout << "Transform.h: collision(): collision detected" << std::endl
			  << a.gX << "," << a.gY << " " << std::endl 
			  << b.gX << "," << b.gY << " " << std::endl;
}

//Consturcts transform with given initial global coordinates and scale
Transform::Transform(double x, double y, double s){
	
	gX = x;
	gY = y;
	
	scale = s;

	toScreen(0,0,0,0,0,0);
}

//Takes coordinates of camera and the dimensions of object 
//and returns the values of the new destination rect
SDL_Rect* Transform::toScreen(double camx, double camy, int width, int height, int screenW, int screenH){
	
	//Set center point
	dstR.w = round(width * scale);
	dstR.h = round(height * scale);
	
	//Assign screen coordinets with centerpoint and view point offsets to destination rect
	dstR.x = (screenW/2 + (gX - camx)) - (dstR.w / 2);
	dstR.y = (screenH/2 - (gY - camy)) - (dstR.h / 2);

	//Debug:
	//std::cout << "Transform: toScreen: Screen(" << dstR.x << "," << dstR.y << ")" << std::endl
	//		  << "Transform: toScreen: Width:" << dstR.w << " Height: " << dstR.h << "\n" << std::endl;

	return &dstR;
}

//Copies transform coordinates
Transform& Transform::operator=(Transform trans){
	gX = trans.gX;
	gY = trans.gY;

	return *this;
}
