#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"

class TestMode {
	UltrasonicSensor testUltrasonic1;
	UltrasonicSensor testUltrasonic2;
	UltrasonicSensor testUltrasonic3;
public:
	AnalogIOButton * testSwitch;
	enum testModes{testGamepad, testJoystick, testTalon, testIO, testEncoder, ultrasonicTestMode, analogSwitchMode};
	
	TestMode(DriverStation * theDriverStation);
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, 
			            Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch,
			            Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic, 
			            UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch);
	~TestMode();
private:
	int m_mode;
	DriverStation * m_ds;
	DriverStationEnhancedIO & m_dsIO;
};
#endif
