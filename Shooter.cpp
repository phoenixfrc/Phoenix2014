#include "WPILib.h"
#include "Shooter.h"
		
		enum shooterRelease	{released, loaded};

Shooter::Shooter() :
    shooterMotor(2)// Talon(2)

{

}

void Shooter::OperateShooter(Joystick * gamePad) {

	
	bool loadShooter = gamePad->GetRawButton(7);
	
	bool releaseShooter = gamePad->GetRawButton(8);

	//decode load shooter button (LT, button7)
	if (loadShooter) {
		shooterMotor.Set(1.0);//turn on the motor
	}
	else{
		shooterMotor.Set(0.0);
	}
	
	//decode release shooter button (RT, button8)
	if (releaseShooter &&  !loadShooter) {
			//release Shooter	

	}

//Shooter::~Shooter(){
        
}



//rt=button8
//lt=button7
//shooter release digital i/o #1 
