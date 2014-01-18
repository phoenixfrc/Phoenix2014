#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"

class TestMode {
	
public:
	TestMode();
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, Joystick * rightStick, Joystick * leftStick);
	enum testModes{testGamepad, testJoystick, testTalon, testIO, testEncoder};
	~TestMode();
private:
	int m_mode;
	
};
#endif
