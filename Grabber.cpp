#include "Grabber.h"




Grabber::Grabber() :
	grabberActuator(PHOENIX2014_GRABBER_ACTUATOR_PWM),
	//grabberElevator(PHOENIX2014_GRABBER_ELEVATOR_PWM),
	grabberCloseLimit(PHOENIX2014_ANALOG_GRABBER_CLOSE_LIMIT_SWITCH),
	grabberOpenLimit(PHOENIX2014_ANALOG_GRABBER_OPEN_LIMIT_SWITCH),
	ballSensor(PHOENIX2014_GRABBER_BALL_SENSOR)
{
	//initialize the grabber to trip the closed grabber switch
	m_grabberState = unknown;
	m_grabberPower = 1.0;
}

void Grabber::OperateGrabber(Joystick * gamePad){
	//One button will toggle between open and closed grabber
	bool grabberButton = gamePad->GetRawButton(1);
	//float moveGrabberUpButton = gamePad->GetRawButton(2);
	//float moveGrabberDownButton = gamePad->GetRawButton(4);
	//bool ballPresent =  ballSensor.Get();
	bool reachedLimitForClosed = grabberCloseLimit.Get();
	bool reachedLimitForOpen = grabberOpenLimit.Get();
	
	//This will 
	switch(m_grabberState){
		case closeing:
			grabberActuator.Set(m_grabberPower*-1);
			if(reachedLimitForClosed){
				m_grabberState = closed;
			}
			break;
		case closed:
			grabberActuator.Set(0.0);
			if(grabberButton){
				m_grabberState = opening;
			}
			break;
		case opening:
			grabberActuator.Set(m_grabberPower);
			if(reachedLimitForOpen){
				m_grabberState = open;
			}
			break;
		case open:
			grabberActuator.Set(0.0);
			if (grabberButton){
				m_grabberState = closeing;
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
	/*if (openGrabberButton == true){
		grabberActuator.Set(-25.0);
	}
	if (reachedLimitForOpen == true){
		grabberActuator.Set(0.0);
	}
	//this should close the grabber when you press the close button
	if (closeGrabberButton == true || ballPresent == true && reachedLimitForClosed == false){
		grabberActuator.Set(25.0);
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
	
}
Grabber::~Grabber(){
	
}
