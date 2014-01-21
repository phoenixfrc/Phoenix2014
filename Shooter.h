#ifndef Shooter_h
#define SHOOTER_H
#include "WPILib.h"
#include "Phoenix2014.h"

class Shooter {
	Talon shooterMotor;
	DigitalInput retractedSensor;
	Relay releaseShooter;
	Encoder shooterEncoder;
public:
		Shooter();  //constructor called when instances created
		void OperateShooter(Joystick * gamePad);  //Controll the Shooter
		~Shooter();  //The destructor called instances destroyed

		
};
#endif
