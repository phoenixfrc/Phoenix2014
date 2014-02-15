#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"

class Grabber {
	
public:
		Grabber();
		void OperateGrabber(Joystick * gamePad);
		~Grabber();
		float desiredElevatorVoltage;
private:
		enum grabberStates{open, opening, closed, closing, unknown};
		Talon grabberActuator;
		//Talon grabberElevator;
		DigitalInput grabberCloseLimit;
		DigitalInput grabberOpenLimit;
		//DigitalInput ballSensor;
		DigitalInput bottomLimitSwitch;
		DigitalInput topLimitSwitch;
		Talon elevatorMotor;
public:
		AnalogChannel elevatorAngleSensor;
		PIDController elevatorController;
private:
		double m_grabberPower;
		double m_elevatorPower;
		double m_encoderLimit;
		grabberStates m_grabberState;
public:
		UltrasonicSensor ballDetector;
private:
		//DriverStationLCD * lcd;
		int distanceToClose;
		bool detectBall;

};
#endif
