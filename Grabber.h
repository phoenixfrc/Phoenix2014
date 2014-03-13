#ifndef GRABBER_H
#define GRABBER_H
#include "WPILib.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"
#include "DeadZoneTalon.h"
#include "TestMode.h"

class Grabber {
	
public:
		Grabber(Joystick * gamePad);
		void OperateGrabber(bool openToShoot, bool useBallSensor);
		float ButtonControledElevator();
		float ThumbstickControledElevator();
		float ElevatorLimitSwitchBehavior();
		void UpDateWithState(DriverStationLCD::Line line, DriverStationLCD * lcd);
		void DisplayDebugInfo(DriverStationLCD::Line line, DriverStationLCD * lcd);
		void DriveElevatorTestMode(float value);
		float OperatePIDLoop();
		
		~Grabber();
		void init();
		float m_desiredElevatorVoltage;
		void resetSetPoint();
				
private:
		Joystick * m_gamePad;
		enum grabberStates{open, opening, closed, closing, unknown};
public:
		Talon grabberActuator;
		
private:
		//Talon grabberElevator;
		DigitalInput grabberCloseLimit;
		DigitalInput grabberOpenLimit;
		//DigitalInput ballSensor;
		DigitalInput forwardLimitSwitch;
		DigitalInput backLimitSwitch;
		DeadZoneTalon elevatorMotor;
public:
		AnalogChannel elevatorAngleSensor;
		PIDController elevatorController;
private:
		double m_grabberPower;
		double m_elevatorPower;			
		grabberStates m_grabberState;
public:
		UltrasonicSensor ballDetector;
private:
		//DriverStationLCD * lcd;
		int distanceToClose;
		bool detectBall;
		char * m_stateString;


};
#endif
