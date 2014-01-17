#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"

class TestMode {
	
public:
	TestMode();
	void PerformTesting(Joystick * gamePad, DriverStationLCD * lcd);
	enum testModes{testGamepad, testJoystick, testTalon, testIO, testEncoder};
	~TestMode();
private:
	int m_mode;
	
};
#endif
