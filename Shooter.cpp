#include "WPILib.h"
#include "Shooter.h"
#include "Phoenix2014.h"
Shooter::Shooter() :
   winchMotor(PHOENIX2014_SHOOTER_LOAD),
   retractedSensor(PHOENIX2014_ANALOG_SHOOTER_LIMIT_SWITCH),
   unwindedSensor(PHOENIX2014_ANALOG_UNWINED_LIMIT_SWITCH),
   brakeSensor(PHOENIX2014_ANALOG_BRAKE_LIMIT_SWITCH),
   brakeRelease(PHOENIX2014_SHOOTER_BRAKE_RELEASE),
   shooterEncoder(PHOENIX2014_SHOOTER_ENCODER_A,PHOENIX2014_SHOOTER_ENCODER_B),
   shooterLoadLimit(PHOENIX2014_ANALOG_SHOOTER_LIMIT_SWITCH)
{
		m_limitSwitch = true;
		m_shooterState = unknown;
		m_loaderPower = 1.0;
		m_encoderReachedLimitForLoad = 100.0;
		
   shooterEncoder.Reset();

}
void Shooter::OperateShooter(Joystick * gamePad) {


	bool shooterButton = gamePad->GetRawButton(7);//TODO make constants
	bool loadShooterButton = gamePad->GetRawButton(8);
	bool isWinded = retractedSensor.Get();
	bool isUnwinded = unwindedSensor.Get();
	bool isBraked = brakeSensor.Get();
	float encoderValue = shooterEncoder.Get();
	//bool loaderSwitchOn = (retractedSensor.Get() == 0);
	int ShooterEncoderLimit = 100;
	int brakeCounter = 0;
	//bool loadComplete =  loaderSensor.Get();
	switch (m_shooterState){
		case shoot:
			brakeRelease.Set(Relay::kReverse);
			brakeCounter = brakeCounter++;
			if(brakeCounter == 5){
				m_shooterState = winding;
			}
			break;
		case winding:
			winchMotor.Set(1.0);
			if(isWinded){
				m_shooterState = braking;
			}
			break;
		case braking:
			winchMotor.Set(0.0);
			if (isBraked){
				m_shooterState = unwinding;
			}
			else{
				brakeRelease.Set(Relay::kForward);
			}
			break;
		case unwinding:
			brakeRelease.Set(Relay::kOff);
			if(isUnwinded){
				m_shooterState = loaded;
			}
			else{
				winchMotor.Set(-1.0);
			}
			break;
		case loaded:
			winchMotor.Set(0.0);
			if (shooterButton){
				m_shooterState = shoot;
			}
			break;
		case unknown://unknown is the same as default
		default:
			if (isWinded && !isUnwinded && !isBraked){
				m_shooterState = braking;
			}
			else if (!isWinded && isUnwinded && isBraked){
				m_shooterState = loaded;
			}
			else if (!isUnwinded && isBraked){
				m_shooterState = unwinding;
			}
			else if (isUnwinded && isWinded){
				winchMotor.Set(0.0);
			}
			else{
				m_shooterState = winding;
			}
			break;
		}


/*	if (loadShooterButton && shooterState == released){
		shooterMotor.Set(1.0);//turn on winch motor
		if(encoderValue >= m_reachedLimitForLoad || isretracted){
			shooterMotor.Set(0.0);
			if (brakeButton){
				brakeSwitch.Get();
			}
			
		}
	}
			
	
	
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

	if (loadShooterButton && m_limitSwitch){
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

