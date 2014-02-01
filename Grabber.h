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
		AnalogIOButton bottomLimitSwitch;
		AnalogIOButton topLimitSwitch;
		Encoder elevatorEncoder;
		Talon elevatorMotor;
		double m_grabberPower;
		double m_elevatorPower;
		double m_encoderLimit;
		grabberStates m_grabberState;
};
#endif
