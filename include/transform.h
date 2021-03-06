#pragma once

class Transform{
	friend void collision(Transform&, Transform&);

	public:
		
		//Consturctor
		Transform(double=0.0, double=0.0, double=1.0);
		
		//Computes coordinats for rendering
		SDL_Rect* toScreen(double, double, int, int, int, int);
		
		//Copies transform coordinates
		Transform& operator=(Transform);

		//Returns global x and y coordinates
		double x(){return gX;}
		double y(){return gY;}
		
		//Changes global x and y coordinates to given value
		void setX(double dX){ gX = dX; }
		void setY(double dY){ gY = dY; }
		
		//Incriments global x and y coordinates by a given value
		void movX(double dX){ gX += dX; }
		void movY(double dY){ gY += dY; }
		
		//Returns the scale of the object
		double getScale(){return scale;}
		
		//Set the scale
		void setScale(double s){ scale = s; }
		
		//Increment/Decrement the scale
		void movScale(double s){ scale += s; }
	private:
		//Coordinates in world
		double gX;
		double gY;
		
		//Scale of texture
		double scale;
		
		//Destination rect for rendering
		SDL_Rect dstR;
};
