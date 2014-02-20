#include "WPILib.h"
#include "Shooter.h"
#include "Phoenix2014.h"
Shooter::Shooter() :
   winchMotor(PHOENIX2014_SHOOTER_LOAD_MOTOR),
   winchEncoder(PHOENIX2014_SHOOTER_ENCODER_A,
		   PHOENIX2014_SHOOTER_ENCODER_B),
   winchRetractedSensor(PHOENIX2014_SHOOTER_LIMIT_SWITCH),
   brakeEngaged(PHOENIX2014_BRAKE_ENGAGED_LIMIT_SWITCH),
   brakeDisengaged(PHOENIX2014_BRAKE_DISENGAGED_LIMIT_SWITCH),
   brakeMotor(PHOENIX2014_SHOOTER_BRAKE_MOTOR_SPIKE)
{
	m_shooterState = unknown;
	m_loaderPower = 1.0;
	m_encoderReachedLimitForLoad = -1;
	winchEncoder.Reset();
	winchEncoder.Start();
}

void Shooter::init(){
	winchEncoder.Reset();
	winchEncoder.Start();
	m_shooterState = unknown;
	m_encoderReachedLimitForLoad = -1;
}

void Shooter::OperateShooter(bool shootRequest, bool loadRequest) {

	bool isWound = !winchRetractedSensor.Get();
	bool isBraked = !brakeEngaged.Get();
	bool isUnbraked = !brakeDisengaged.Get();
	int encoderValue = winchEncoder.Get();
	int ShooterEncoderLimit = 100;
	switch (m_shooterState){
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
		case winding:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = winding");
			winchMotor.Set(m_loaderPower);
			if(isWound){
				// Transition to next state - braking
				winchMotor.Set(0.0);
				winchEncoder.Reset();
				winchEncoder.Start();
				m_shooterState = braking;
			}
			break;
		case braking:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = braking");
			brakeMotor.Set(Relay::kForward);
			if (isBraked){
				brakeMotor.Set(Relay::kOff);
				m_shooterState = unwinding;
			}
			break;
		case unwinding:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = unwinding");
			winchMotor.Set(-m_loaderPower);
			if(encoderValue >= ShooterEncoderLimit){
				winchMotor.Set(0.0);
				m_shooterState = loaded;
			}
			break;
		case loaded:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = loaded");
			if (shootRequest){
				winchEncoder.Reset();
				winchEncoder.Start();
				m_shooterState = shoot;
			}
			break;
		case shoot:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "SS = shoot");
			brakeMotor.Set(Relay::kReverse);
			if(isUnbraked || loadRequest){
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
void Shooter::DisplayDebugInfo(DriverStationLCD::Line line, DriverStationLCD * lcd){
	bool isWound = !winchRetractedSensor.Get();
	bool isBraked = !brakeEngaged.Get();
	bool isUnbraked = !brakeDisengaged.Get();
	int encoderValue = winchEncoder.Get();

	lcd->PrintfLine(line, "Ss=%c%c%c en=%d", //prints the button values to LCD display
					isWound ? '1':'0',
					isBraked ? '1':'0',
					isUnbraked ? '1':'0',
					encoderValue
					);
}
void Shooter::TestShooter(){
	
}
void Shooter::PrintShooterState(DriverStationLCD::Line line, DriverStationLCD * lcd){
	if(m_shooterState == shoot){
		lcd->PrintfLine(line, "ShSt=shoot");
	}
	else if (m_shooterState == winding){
		lcd->PrintfLine(line, "ShSt=winding");
	}
	else if (m_shooterState == braking){
		lcd->PrintfLine(line, "ShSt=braking");
		}
	else if (m_shooterState == unwinding){
		lcd->PrintfLine(line, "ShSt=unwinding");
	}
	else if (m_shooterState == loaded){
		lcd->PrintfLine(line, "ShSt=loaded");
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

