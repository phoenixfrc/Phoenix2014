#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	enum grabberStates{pressed, notPressed};
	Relay grabberPuller;
	Talon grabberElevator;
	
	grabberStates grabberState;
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
	
	
};
#endif
