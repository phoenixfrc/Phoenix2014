#ifndef SHOOTER_H
#define SHOOTER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Shooter {
	//loaded means shooter fully retracted and ready to fire
	//released means the shooter is not retracted and needs to be loaded
	//
	enum shooterStates{shoot, winding, braking, unwinding, loaded, unknown};  //Undwinding needs to be investigated
	Talon winchMotor;
	DigitalInput winchRetractedSensor;
	DigitalInput brakeEngaged;
	DigitalInput brakeDisengaged;
	Talon brakeMotor;
public:
		Shooter();  //constructor called when instances created
		void OperateShooter(bool shooterButton, bool loadShooterButton);  //Controll the Shooter
		void DisplayDebugInfo(DriverStationLCD::Line line, DriverStationLCD * lcd);
		void TestShooter(float brakePower, float loaderPower);
		void PrintShooterState(DriverStationLCD::Line line, DriverStationLCD * lcd);
		~Shooter();  //The destructor called instances destroyed
		void init();

private:
		shooterStates m_shooterState;
		float m_loaderPower;
		float m_brakePower;
		int m_unwindDelayCounter;
};
#endif
