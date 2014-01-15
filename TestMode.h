#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"

class TestMode {
	
public:
	TestMode();
	void performTesting(Joystick * gamePad, DriverStationLCD * lcd);
	~TestMode();
	
};
#endif
