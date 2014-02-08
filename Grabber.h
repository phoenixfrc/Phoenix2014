#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Grabber {
	
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
		PIDController elevatorController;
		int desiredElevatorAngle;
private:
		enum grabberStates{open, opening, closed, closing, unknown};
		Talon grabberActuator;
		//Talon grabberElevator;
		DigitalInput grabberCloseLimit;
		DigitalInput grabberOpenLimit;
		DigitalInput ballSensor;
		DigitalInput bottomLimitSwitch;
		DigitalInput topLimitSwitch;
		Talon elevatorMotor;
		double m_grabberPower;
		double m_elevatorPower;
		double m_encoderLimit;
		grabberStates m_grabberState;
		AnalogChannel elevatorAngleSensor;
		DriverStationLCD * lcd;

};
#endif
