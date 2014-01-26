#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
private:
		enum grabberStates{open, opening, closed, closeing, unknown};
		Talon grabberActuator;
		//Talon grabberElevator;
		AnalogIOButton grabberCloseLimit;
		AnalogIOButton grabberOpenLimit;
		DigitalInput ballSensor;
		double m_grabberPower;
		grabberStates m_grabberState;
};
#endif
