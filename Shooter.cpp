#include "WPILib.h"
#include "Shooter.h"
#include "Phoenix2014.h"
Shooter::Shooter() :
   shooterMotor(PHOENIX2014_SHOOTER_LOAD),
   retractedSensor(PHOENIX2014_ANALOG_SHOOTER_LIMIT_SWITCH),
   releaseShooter(PHOENIX2014_SHOOTER_RELEASE),
   shooterEncoder(PHOENIX2014_SHOOTER_ENCODER_A,PHOENIX2014_SHOOTER_ENCODER_B),
   shooterState(loading),
   loaderSensor(PHOENIX2014_SHOOTER_LOAD),
   shooterLoadLimit(PHOENIX2014_ANALOG_SHOOTER_LIMIT_SWITCH),
   loaderMotor(PHOENIX2014_SHOOTER_LOAD)

{
		m_limitSwitch = true;
			m_loaderPower = 1.0;
   shooterEncoder.Reset();

}
void Shooter::OperateShooter(Joystick * gamePad) {


	bool loadShooterButton = gamePad->GetRawButton(7);//TODO make constants
	bool releaseShooterButton = gamePad->GetRawButton(8);																																																																																																																																																																																	
	bool isRetracted = retractedSensor.Get();
	//bool loaderSwitchOn = (retractedSensor.Get() == 0);
	int ShooterEncoderLimit = 100;
	//bool loadComplete =  loaderSensor.Get();
	

	//Here I want to shoot the ball=
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

	if(shooterEncoder.Get() >= ShooterEncoderLimit && shooterState == loading || isRetracted){
	shooterMotor.Set(Relay::kOff);
	shooterState = loaded;
	}
}

	/*if (loadShooterButton && m_limitSwitch){
			loaderMotor.Set(Relay::kReverse);
			if(m_limitSwitch == reachedLimitForLoad){
			}
			else{
				m_limitSwitch = reachedLimitForLoad;
			}
		}
		if (m_limitSwitch){
			loaderMotor.Set(Relay::kOff);
		}
		if(loaderSensor.Get() && reachedLimitForLoad){
			m_limitSwitch = reachedLimitForLoad;
			loaderMotor.Set(Relay::kOff);
		}
		if(reachedLimitForLoad){
			loaderMotor.Set(Relay::kOff);
		}*/
	
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

