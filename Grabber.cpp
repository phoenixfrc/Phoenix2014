#include "Grabber.h"




Grabber::Grabber() :
	elevatorController(0.1, 0.001, 0.0, &elevatorAngleSensor, &elevatorMotor ),
	grabberActuator(PHOENIX2014_GRABBER_CLAW_MOTOR_PWM),
	//grabberElevator(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	grabberCloseLimit(PHOENIX2014_GRABBER_CLOSE_LIMIT_SWITCH),
	grabberOpenLimit(PHOENIX2014_GRABBER_OPEN_LIMIT_SWITCH),
	bottomLimitSwitch(PHOENIX2014_ELEVATOR_BOTTOM_LIMIT_SWITCH),
	topLimitSwitch(PHOENIX2014_ELEVATOR_TOP_LIMIT_SWITCH),
	elevatorMotor(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	elevatorAngleSensor(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ELEVATOR_ANGLE),
	ballDetector(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_GRABBER_BALL_ULTRASONIC_SENSOR)
	//lcd(DriverStationLCD::GetInstance())

	
{
	//initialize the grabber to trip the closed grabber switch
	m_grabberState = unknown;
	m_grabberPower = 1.0;
	m_elevatorPower = 1.0;
	m_encoderLimit = 100;//Need to change later
	//initialize elevator PiD loop
	elevatorController.SetOutputRange(-0.25, 0.25);
	elevatorController.SetInputRange(0, 5.0);
	distanceToClose = 12;
	detectBall = true;
	

}

void Grabber::OperateGrabber(Joystick * gamePad){
	//One button will toggle between open and closed grabber
	bool grabberButton = gamePad->GetRawButton(1);
	//float moveGrabberUpButton = gamePad->GetRawButton(2);
	//float moveGrabberDownButton = gamePad->GetRawButton(4);
	//bool ballPresent =  ballSensor.Get();
	bool reachedLimitForClosed = !grabberCloseLimit.Get();
	bool reachedLimitForOpen = !grabberOpenLimit.Get();
	bool elevatorForwardRequest = gamePad->GetRawButton(4);
	bool elevatorBackwardRequest = gamePad->GetRawButton(2);
	bool bottomLimit = !bottomLimitSwitch.Get();
	bool topLimit = !topLimitSwitch.Get();
	float angleIncrement = 0.05;

	//Ball detector.
	if(ballDetector.GetDistance() < distanceToClose){
		detectBall = true;
	}
	else{
		detectBall = false;
	}
	
	//int currentElevatorAngle =(int) (elevatorAngleSensor.GetVoltage()*PHOENIX2014_POT_DEGREES_PER_VOLT);
	//This will 
	switch(m_grabberState){
		case closing:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "GS = closing");
			grabberActuator.Set(m_grabberPower*-1);
			if(reachedLimitForClosed){
				m_grabberState = closed;
			}
			break;
		case closed:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "GS = closed");
			grabberActuator.Set(0.0);
			if(grabberButton){
				m_grabberState = opening;
			}
			break;
		case opening:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "GS = opening");
			grabberActuator.Set(m_grabberPower);
			if(reachedLimitForOpen){
				m_grabberState = open;
			}
			break;
		case open:
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "GS = open");
			grabberActuator.Set(0.0);//if button is pressed or ball is detected.
			if (grabberButton || detectBall){
				m_grabberState = closing;
			}
			break;
		case unknown://unknown is the same as default
			//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "GS = unknown");
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
	if(elevatorForwardRequest && !elevatorBackwardRequest && !topLimit){
		desiredElevatorAngle = desiredElevatorAngle + angleIncrement;
				//elevatorMotor.Set(m_elevatorPower);
	}
	
	else if(elevatorBackwardRequest && !elevatorForwardRequest && !bottomLimit){
		desiredElevatorAngle = desiredElevatorAngle - angleIncrement;
				//elevatorMotor.Set(m_elevatorPower*-1);
		
		}
		else {
		if(topLimit){
			desiredElevatorAngle = desiredElevatorAngle - angleIncrement;
					}
		if(bottomLimit){
			desiredElevatorAngle = desiredElevatorAngle + angleIncrement;
					}
	}
	elevatorController.SetSetpoint(desiredElevatorAngle / PHOENIX2014_POT_DEGREES_PER_VOLT);
	
	
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
Grabber::~Grabber(){
	
}
