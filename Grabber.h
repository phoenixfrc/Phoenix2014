#ifndef GRABBER_H
#define GRABBER	_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	Relay grabberPuller;
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
	
	
};
#endif
