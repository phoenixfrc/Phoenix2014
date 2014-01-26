#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	enum grabberStates{open, closed};
	Talon grabberActuator;
	//Talon grabberElevator;
	DigitalInput grabberCloseLimit;
	DigitalInput grabberOpenLimit;
	DigitalInput ballSensor;
	
	grabberStates grabberState;
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
private:
		bool m_limitSwitch;
		double m_grabberPower;
	
};
#endif
