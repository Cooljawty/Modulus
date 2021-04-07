#include <chrono>
#include <iostream>

#include "modulus_time.h"

using namespace std::chrono;
using namespace Modulus;

Timer::Timer(){
	startTime = 0.0;
	pauseTime = 0.0;

	isPaused = false;
	isStarted = false;
}

void Timer::start(){

	//Set timer states
	isStarted = true;
	isPaused = false;

	//Update start time
	startTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

	//Reset paused time
	pauseTime = 0.0;
}

void Timer::stop(){

	//Set timer states
	isStarted = false;
	isPaused = false;

	//Clear saved times
	startTime = 0.0;
	pauseTime = 0.0;
}

void Timer::pause(){

	//Check if timer is started and not paused
	if(isStarted && !isPaused){
		isPaused = true;

		//Save time
		pauseTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count() - startTime;

		startTime = 0.0;
	}
}

void Timer::unpause(){

	//Check if timer is started and not paused
	if(!isStarted && isPaused){
		isPaused = false;

		//Save time
		startTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count() - pauseTime;

		pauseTime = 0.0;
	}
}

double Timer::getTime(){

	if(isStarted){
		if(isPaused){
			return pauseTime * 0.001;
		}
		else{
			return ((double)duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count() - startTime) * 0.001;
		}
	}

	return 0.0;
}
