#include "WPILib.h"
#include "Shooter.h"
#include "Phoenix2014.h"
Shooter::Shooter() :
   winchMotor(PHOENIX2014_SHOOTER_LOAD_MOTOR),
   winchEncoder(PHOENIX2014_SHOOTER_ENCODER_A,PHOENIX2014_SHOOTER_ENCODER_B),
   winchRetractedSensor(PHOENIX2014_SHOOTER_LIMIT_SWITCH),
   unwoundSensor(PHOENIX2014_UNWINED_LIMIT_SWITCH),
   brakeEngaged(PHOENIX2014_BRAKE_ENGAGED_LIMIT_SWITCH),
   brakeDisengaged(PHOENIX2014_BRAKE_DISENGAGED_LIMIT_SWITCH),
   brakeMotor(PHOENIX2014_SHOOTER_BRAKE_MOTOR_SPIKE)
   //lcd(DriverStationLCD::GetInstance())
{
		m_shooterState = unknown;
		m_loaderPower = 1.0;
		m_encoderReachedLimitForLoad = 100.0;
		
   winchEncoder.Reset();

}
void Shooter::OperateShooter(bool shootRequest, bool loadRequest) {

	bool isWound = !winchRetractedSensor.Get();
	bool isUnwound = !unwoundSensor.Get();//revisit unwound logic
	bool isBraked = !brakeEngaged.Get();
	bool isUnbraked = !brakeDisengaged.Get();
	float encoderValue = winchEncoder.Get();
	int ShooterEncoderLimit = 100;
	switch (m_shooterState){
		case shoot:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = shoot");
			brakeMotor.Set(Relay::kReverse);
			if(isUnbraked){
				winchEncoder.Reset();
				winchEncoder.Start();
				m_shooterState = winding;
			}
			if (loadRequest){
				m_shooterState = winding;
			}
			break;
		case winding:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = winding");
			winchMotor.Set(m_loaderPower);
			if(encoderValue >= ShooterEncoderLimit || isWound){
				m_shooterState = braking;
			}
			break;
		case braking:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = braking");
			winchMotor.Set(0.0);
			winchEncoder.Reset();
			winchEncoder.Start();
			if (isBraked){
				m_shooterState = unwinding;
			}
			else{
				brakeMotor.Set(Relay::kForward);
			}
			break;
		case unwinding:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = unwinding");
			brakeMotor.Set(Relay::kOff);
			if((encoderValue >= ShooterEncoderLimit) || isUnwound){
				m_shooterState = loaded;
			}
			else{
				winchMotor.Set(-m_loaderPower);
			}
			break;
		case loaded:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = loaded");
			winchMotor.Set(0.0);
			if (shootRequest){
				m_shooterState = shoot;
			}
			break;
		case unknown://unknown is the same as default
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = unknown");
		default:
			if (isBraked){
				if(isWound){
					m_shooterState = unwinding;
				}
				else{
					m_shooterState = loaded;
				}
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

