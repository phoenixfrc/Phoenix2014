#ifndef SHOOTER_H
#define SHOOTER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Shooter {
	//loaded means shooter fully retracted and ready to fire
	//released means the shooter is not retracted and needs to be loaded

	enum shooterStates{shoot, winding, braking, unwinding, loaded, unknown};
	Talon winchMotor;
	AnalogIOButton retractedSensor;
	AnalogIOButton unwoundSensor;
	AnalogIOButton brakeSensor;
	Relay brakeRelease;
	Encoder shooterEncoder;
	AnalogIOButton shooterLoadLimit;
	
public:
		Shooter();  //constructor called when instances created
		void OperateShooter(Joystick * gamePad);  //Controll the Shooter
		~Shooter();  //The destructor called instances destroyed

private:
		bool m_limitSwitch;
		shooterStates m_shooterState;
		double m_loaderPower;
		double m_encoderReachedLimitForLoad;
};
#endif
