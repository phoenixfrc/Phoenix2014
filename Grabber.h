#ifndef GRABBER_H
#define GRABBER	_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	Relay grabberPuller;
	Talon grabberElevator;
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
	
	
};
#endif
