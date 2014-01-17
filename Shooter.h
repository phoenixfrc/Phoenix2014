#ifndef Shooter_h
#define SHOOTER_H
#include "WPILib.h"

class Shooter {
	
public:
		Shooter();  //constructor called when instances created
		void OperateShooter(Joystick * gamePad);  //Controll the Shooter
		~Shooter();  //The destructor called instances destroyed
	
};
#endif
