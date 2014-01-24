#include "WPILib.h"
#include "Shooter.h"

Shooter::Shooter() :
   shooterMotor(PHOENIX2014_SHOOTER_LOAD_PWM),
   retractedSensor(3),
   releaseShooter(PHOENIX2014_SHOOTER_RELEASE),
   shooterEncoder(5,6),
   shooterState(loading)
{
   shooterEncoder.Reset();
}

void Shooter::OperateShooter(Joystick * gamePad) {


	bool loadShooterButton = gamePad->GetRawButton(7);//TODO make constants
	bool releaseShooterButton = gamePad->GetRawButton(8);
	bool isRetracted = retractedSensor.Get();
	int ShooterEncoderLimit = 100;
	
	//Here I want to shoot the ball
	if(releaseShooterButton && shooterState == loaded){
	releaseShooter.Set(Relay::kReverse);
	shooterState = released;
	}
	
	//Here I want start loading(retracting)
	if (loadShooterButton &&  shooterState == released && !isRetracted) {
	shooterEncoder.Start();
	shooterMotor.Set(1.0);
	shooterState = loading;
	}
	
	//Read encoder if limit reached stop retracting
	if(shooterEncoder.Get() >= ShooterEncoderLimit || isRetracted){
	shooterMotor.Set(0.0);
	}
	}
	
Shooter::~Shooter(){
		   
}



//rt=button8
//lt=button7
//shooter release digital i/o #1 
