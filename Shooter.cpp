#include "WPILib.h"
#include "Shooter.h"

Shooter::Shooter() :
   shooterMotor(PHOENIX2014_SHOOTER_LOAD),
   retractedSensor(3),
   releaseShooter(PHOENIX2014_SHOOTER_RELEASE),
   shooterEncoder(5,6),
   shooterState(loading),
   loaderSensor(7)
{
   shooterEncoder.Reset();

}
void Shooter::OperateShooter(Joystick * gamePad) {


	bool loadShooterButton = gamePad->GetRawButton(7);//TODO make constants
	bool releaseShooterButton = gamePad->GetRawButton(8);
	bool isRetracted = retractedSensor.Get();
	bool loaderSwitchOn = (loaderSensor.Get() == 0);
	int ShooterEncoderLimit = 100;
	

	//Here I want to shoot the ball
	if(releaseShooterButton && shooterState == loaded){
	releaseShooter.Set(Relay::kReverse);
	shooterState = released;
	}
	
	//Here I want start loading(retracting)
	if (loadShooterButton &&  shooterState == released && !isRetracted) {
	shooterEncoder.Start();
	shooterMotor.Set(Relay::kReverse);
	shooterState = loading;
	}
	
	//Read encoder if limit reached stop retracting

	if(shooterEncoder.Get() >= ShooterEncoderLimit || isRetracted){
	shooterMotor.Set(0.0);
	}
	}
	
Shooter::~Shooter(){
		   
}


/* Runs during test mode
         * Blue X = button 1
         * Green A = button 2
         * Red B = button 3
         * Yellow Y = button 4
         * LB = button 5
         * RB = button 6
         * LT = button 7
         * RT = button 8
         */

