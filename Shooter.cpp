#include "WPILib.h"
#include "Shooter.h"
		
		enum shooterRelease	{released, loaded};

Shooter::Shooter() :
    shooterMotor(PHOENIX2014_SHOOTER_LOAD_PWM),
    retractedSensor(3)
{

}

void Shooter::OperateShooter(Joystick * gamePad) {

	
	bool loadShooter = gamePad->GetRawButton(7);
	
	bool releaseShooter = gamePad->GetRawButton(8);
	
	bool isRetracted = retractedSensor.Get();

	//decode load shooter button (LT, button7)
	if (loadShooter) {//I think you have to hold the button down the whole time for this to work
			if (isRetracted){
					shooterMotor.Set(0.0);
			}
			else{
				shooterMotor.Set(1.0);//turn on the motor
	}
}
	else{
		shooterMotor.Set(0.0);
	}
	
	//decode release shooter button (RT, button8)
	if (releaseShooter &&  !loadShooter) {
	//	releaseShooter.set(relay::kReverse);
			//release Shooter	
	}
	else{
		//releaseShooter.set(relay::kOff);
	}
}

Shooter::~Shooter(){
        
}



//rt=button8
//lt=button7
//shooter release digital i/o #1 
