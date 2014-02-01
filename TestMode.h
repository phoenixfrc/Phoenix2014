#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"
#include "Phoenix2014.h"

class TestMode {
	
public:
	AnalogChannel * ultrasonic;
	TestMode(DriverStation * theDriverStation);
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, Joystick * rightStick, Joystick * leftStick, DigitalInput * testSwitch, Talon * testTalons);
	enum testModes{testGamepad, testJoystick, testTalon, testIO, testEncoder, ultrasonicTestMode};
	~TestMode();
private:
	int m_mode;
	DriverStation * m_ds;
	DriverStationEnhancedIO & m_dsIO;
};
#endif
