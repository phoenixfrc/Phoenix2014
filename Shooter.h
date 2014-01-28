#ifndef SHOOTER_H
#define SHOOTER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Shooter {
	//loaded means shooter fully retracted and ready to fire
	//released means the shooter is not retracted and needs to be loaded

	//enum shooterStates{released, loaded, loading};
	Relay shooterMotor;
	AnalogIOButton retractedSensor;
	Relay releaseShooter;
	Encoder shooterEncoder;
	//shooterStates shooterState;
	DigitalInput loaderSensor;
	AnalogIOButton shooterLoadLimit;
	Relay loaderMotor;
	
public:
		Shooter();  //constructor called when instances created
		void OperateShooter(Joystick * gamePad);  //Controll the Shooter
		~Shooter();  //The destructor called instances destroyed

private:
		bool m_limitSwitch;
		double m_loaderPower;
		bool reachedLimitForLoad;
};
#endif
