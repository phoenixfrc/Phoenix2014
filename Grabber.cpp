#include "Grabber.h"




Grabber::Grabber() :
	grabberPuller(PHOENIX2014_GRABBER_ACTUATOR),
	grabberElevator(PHOENIX2014_GRABBER_ELEVATOR_PWM)
{

}

void Grabber::OperateGrabber(Joystick * gamePad){
	
	bool openGrabberButton = gamePad->GetRawButton(1);
	bool closeGrabberButton = gamePad->GetRawButton(3);
	float moveGrabberUpButton = gamePad->GetRawAxis(1);
	float moveGrabberDownButton = gamePad->GetRawAxis(2);
	
	
	//this should open the grabber when you press the open button
	if (openGrabberButton){
	grabberPuller.Set(Relay::kReverse);
	}
	//this should close the grabber when you press the close button
	if (closeGrabberButton){
		grabberPuller.Set(Relay::kForward);
	}
	if (moveGrabberUpButton){
		grabberElevator.Set(1.0);
	}
	if (moveGrabberDownButton && !grabberState == pressed){
		grabberElevator.Set(-1.0);
	}
	
}

Grabber::~Grabber(){
	
}
