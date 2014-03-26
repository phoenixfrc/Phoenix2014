#ifndef TESTMODE_H
#define TESTMODE_H
#include "WPILib.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"
#include "Shooter.h"
#include "Grabber.h"
//this class decleration is needed to compile Grabber
class Grabber;

class TestMode {
	UltrasonicSensor testUltrasonic1;
	UltrasonicSensor testUltrasonic2;
	UltrasonicSensor testUltrasonic3;
	Preferences * dashboardPreferences;
public:
	AnalogIOButton * testSwitch;
	enum testModes{testGamepad, testLimitSwitches, testShooter, testGrabber, testElevator, testJoystick,
				   testTalon, testIO, testEncoder, ultrasonicTestMode, analogSwitchMode, testingElevatorMotor, savePreferences};
	
	TestMode(DriverStation * theDriverStation);
	void PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd, 
			            Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch,
			            Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic, 
			            UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch,
			            Shooter * theShooter, Grabber * theElevator);
	static float GetThumbstickWithZero(Joystick * gamePad);
	static float GetTwistWithZero(Joystick * gamePad);
	~TestMode();
private:
	int m_mode;
	bool m_driving_motor;
	bool m_savePreferencesToFlash;
	DriverStation * m_ds;
	DriverStationEnhancedIO & m_dsIO;
};
#endif
