#include "Shooter.h"
		
		enum shooterRelease	{released, loaded};

Shooter::Shooter(){
        
}

void Shooter::OperateShooter(Joystick * gamePad){
	
	bool loadShooter = gamePad->GetRawButton(7);
	
	bool releaseShooter = gamePad->GetRawButton(8);

	
	//decode load shooter button (LT, button7)
	if (loadShooter) {
		//turn on the motor
		;
	}
	
	
//decode release shooter button (RT, button8)
	
	

}

Shooter::~Shooter(){
        
}



//rt=button8
//lt=button7
//shooter release digital i/o #1 
