#pragma once

class Vector2D{
	public:
		Vector2D(double mag = 0.f, double dir = 0.f){ setVector(mag, dir); }
			
		//Sets the force and direction of the vector
		void setVector(double mag, double dir){ force = mag; angle = dir; }
		
		//Adding force vectors
		Vector2D& operator+=(Vector2D);	

		//Return force vetor values
		double getForce(){ return force; }
		double getAngle(){ return angle; }
		
		//Returns the horizontal and verticale comonentes of the vector
		double getXComp(){ return force * cos(angle); }
		double getYComp(){ return force * sin(angle); }
	private:
		double force;
		double angle;
};

