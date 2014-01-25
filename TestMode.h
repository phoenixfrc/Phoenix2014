#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"

class TestMode {
	
public:
	TestMode(DriverStation * theDriverStation);
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, Joystick * rightStick, Joystick * leftStick, DigitalInput * testSwitch, Talon * testTalons);
	enum testModes{testGamepad, testJoystick, testTalon, testIO, testEncoder};
	~TestMode();
private:
	int m_mode;
	DriverStation * m_ds;
};
#endif
