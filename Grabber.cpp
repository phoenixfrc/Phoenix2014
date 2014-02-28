#include "Grabber.h"




Grabber::Grabber() :
	grabberActuator(PHOENIX2014_GRABBER_CLAW_MOTOR_PWM),
	//grabberElevator(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	grabberCloseLimit(PHOENIX2014_GRABBER_CLOSE_LIMIT_SWITCH),
	grabberOpenLimit(PHOENIX2014_GRABBER_OPEN_LIMIT_SWITCH),
	//forward is battery side
	forwardLimitSwitch(PHOENIX2014_ELEVATOR_FORWARD_LIMIT_SWITCH),
	backLimitSwitch(PHOENIX2014_ELEVATOR_BACK_LIMIT_SWITCH),
	elevatorMotor(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	//the angle is 0 when shooter is backward (transmition side)
	elevatorAngleSensor(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ELEVATOR_ANGLE),
	//elevatorController(0.1*60, 0.001*20, 0.0, &elevatorAngleSensor, &elevatorMotor ),
	ballDetector(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_GRABBER_BALL_ULTRASONIC_SENSOR)
	//lcd(DriverStationLCD::GetInstance())

	
{
	//initialize the grabber to trip the closed grabber switch
	m_grabberState = unknown;
	m_grabberPower = 1.0;
	m_elevatorPower = 1.0;
	//initialize elevator PiD loop
	//elevatorController.SetContinuous(false);
	//elevatorController.SetOutputRange(-m_elevatorPower, m_elevatorPower);//motor run from -1 to 1
	//elevatorController.SetInputRange(PHOENIX2014_VOLTAGE_AT_FRONT, PHOENIX2014_VOLTAGE_AT_BACK);
	//elevatorController.SetAbsoluteTolerance(0.002);//about one degree
	elevatorAngleSensor.SetVoltageForPID(true);
	distanceToClose = 12;
	detectBall = true;
	m_stateString = "";
}

void Grabber::init(){
	m_grabberState = unknown;
}
void Grabber::resetSetPoint(){
	m_desiredElevatorVoltage = elevatorAngleSensor.GetVoltage();
	//elevatorController.Reset();
	//elevatorController.SetSetpoint(m_desiredElevatorVoltage);
	
}


void Grabber::OperateGrabber(bool openToShoot, bool useBallSensor, Joystick * gamePad){
	//One button will toggle between open and closed grabber
	bool grabberButton = gamePad->GetRawButton(1);
	//float moveGrabberUpButton = gamePad->GetRawButton(2);
	//float moveGrabberDownButton = gamePad->GetRawButton(4);
	//bool ballPresent =  ballSensor.Get();
	bool reachedLimitForClosed = !grabberCloseLimit.Get();
	bool reachedLimitForOpen = !grabberOpenLimit.Get();
	bool elevatorForwardRequest = gamePad->GetRawButton(4);
	bool elevatorBackwardRequest = gamePad->GetRawButton(2);
	bool forwardLimit = !forwardLimitSwitch.Get();
	bool backLimit = !backLimitSwitch.Get();
	float voltageIncrement = 0.001;

	//Ball detector.
	if(ballDetector.GetDistance() < distanceToClose){
		detectBall = true;
	}
	else{
		detectBall = false;
	}
	if(openToShoot){
			m_grabberState = opening;
		}
	//int currentElevatorAngle =(int) (elevatorAngleSensor.GetVoltage()*PHOENIX2014_POT_DEGREES_PER_VOLT);
	//This will 
	switch(m_grabberState){
		case closing:
			m_stateString = "GS=closing";
			grabberActuator.Set(m_grabberPower*-1);
			if(reachedLimitForClosed){
				m_grabberState = closed;
			}
			break;
		case closed:
			
			m_stateString = "GS=closed";
			grabberActuator.Set(0.0);
			if(grabberButton){
				m_grabberState = opening;
			}
			break;
		case opening:
			m_stateString = "GS=opening";
			grabberActuator.Set(m_grabberPower);
			if(reachedLimitForOpen){
				m_grabberState = open;
			}
			break;
		case open:
			m_stateString = "GS=open";
			grabberActuator.Set(0.0);//if button is pressed or ball is detected.
			if (grabberButton || (detectBall && useBallSensor)){
				m_grabberState = closing;
			}
			break;
		case unknown://unknown is the same as default
		default:
			if(reachedLimitForClosed){
				m_grabberState = closed;
			}
			else if(reachedLimitForOpen){
				m_grabberState = open;
			}
			else{
				m_grabberState = opening;
			}
			break;
	}
	/*if (grabberButton && m_limitSwitch){
		grabberActuator.Set(m_grabberPower*-1);
		if(m_limitSwitch == reachedLimitForClosed){
			m_limitSwitch = reachedLimitForOpen;
		}
		else{
			m_limitSwitch = reachedLimitForClosed;
		}
	}
	if (m_limitSwitch){
		grabberActuator.Set(0.0);
	}
	if(ballPresent && reachedLimitForOpen){
		m_limitSwitch = reachedLimitForClosed;
		grabberActuator.Set(-1.0);
	}
	if(reachedLimitForClosed){
		grabberActuator.Set(0.0);
	}
	
	//this should open the grabber when you press the open button
	if (openGrabberButton == true){
		grabberActuator.Set(-25.0);
	}
	if (reachedLimitForOpen == true){
		grabberActuator.Set(0.0);
	}
	//this should close the grabber when you press the close button
	if (closeGrabberButton == true || ballPresent == true && reachedLimitForClosed == false){
		1s grabberActuator.Set(25.0);
	}
	if (reachedLimitForClosed == true){
		grabberActuator.Set(0.0);
	}
	if (moveGrabberUpButton == true){
		grabberElevator.Set(1.0);
	}
	if (moveGrabberDownButton == true && !grabberState == pressed){
		grabberElevator.Set(-1.0);
	}*/
	/*if (yButton){
		elevatorMotor.Set(m_elevatorPower);
		if (topLimit){
			elevatorMotor.Set(0.0);
		}
	}*/
	
	//PID Loop for the grabber elevator which controlls the elevator arm
	//

	

	if(elevatorForwardRequest && !elevatorBackwardRequest && !forwardLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage + voltageIncrement;
				//elevatorMotor.Set(m_elevatorPower);
	}
	
	else if(elevatorBackwardRequest && !elevatorForwardRequest && !backLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage - voltageIncrement;
				//elevatorMotor.Set(m_elevatorPower*-1);
		
		}
		else {
		if(backLimit){
			m_desiredElevatorVoltage = m_desiredElevatorVoltage + voltageIncrement;
					}
		if(forwardLimit){
			m_desiredElevatorVoltage = m_desiredElevatorVoltage - voltageIncrement;
					}
	}
	//elevatorController.SetSetpoint(m_desiredElevatorVoltage);
	//elevatorController.SetSetpoint(450.0);
	
/*	else(yButton && bottomLimit){
		elevatorEncoder.Reset();
		elevatorEncoder.Start();
		elevatorMotor.Set(m_elevatorPower);
	}
	if(aButton && topLimit){
		elevatorEncoder.Reset();
		elevatorEncoder.Start();
		elevatorMotor.Set(m_elevatorPower*-1);
	}*/
	
}
void Grabber::UpDateWithState(DriverStationLCD::Line line, DriverStationLCD * lcd){
	lcd->PrintfLine(line, m_stateString);
}
void Grabber::DisplayDebugInfo(DriverStationLCD::Line line, DriverStationLCD * lcd){
	bool reachedLimitForClosed = !grabberCloseLimit.Get();
	bool reachedLimitForOpen = !grabberOpenLimit.Get();
	bool bottomLimit = !forwardLimitSwitch.Get();
	bool topLimit = !backLimitSwitch.Get();

	lcd->PrintfLine(line, "gb=%c%c ev=%c%c", //prints the button values to LCD display
						reachedLimitForOpen ? 'O':'o',
						reachedLimitForClosed ? 'C':'c',
						bottomLimit ? 'F':'f',
						topLimit ? 'B':'b'
						);
}
void Grabber::DriveElevatorTestMode(float value){
	bool bottomLimit = !forwardLimitSwitch.Get();
	bool topLimit = !backLimitSwitch.Get();

	if (bottomLimit){
		elevatorMotor.Set(-0.2);
	}
	else if (topLimit){
		elevatorMotor.Set(0.2);
	}
	else{
		elevatorMotor.Set(value);
	}
}

Grabber::~Grabber(){
}
