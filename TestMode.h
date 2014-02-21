#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"
#include "Shooter.h"

class TestMode {
	UltrasonicSensor testUltrasonic1;
	UltrasonicSensor testUltrasonic2;
	UltrasonicSensor testUltrasonic3;
public:
	AnalogIOButton * testSwitch;
	enum testModes{testGamepad, testShooter, testGrabber, testElevator, testJoystick,
				   testTalon, testIO, testEncoder, ultrasonicTestMode, analogSwitchMode, testingElevatorMotor};
	
	TestMode(DriverStation * theDriverStation);
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, 
			            Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch,
			            Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic, 
			            UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch,
			            Shooter * theShooter);
	~TestMode();
private:
	int m_mode;
	bool m_driving_motor;
	DriverStation * m_ds;
	DriverStationEnhancedIO & m_dsIO;
};
#endif
