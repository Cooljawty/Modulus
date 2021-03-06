#pragma once
class Timer {	
	public:
	Timer();

	//Timer functions
	void start();
	void stop();
	void pause();
	void unpause();
	
	//Get current time
	double getTime();
	
	//Get timer state
	bool started(){ return isStarted; }
	bool paused(){ return isPaused; }

	private:
	//Time at timer's start
	unsigned int startTime;

	//Time when timer is paused
	unsigned int pauseTime;
	
	//Timer states
	bool isPaused;
	bool isStarted;
};
