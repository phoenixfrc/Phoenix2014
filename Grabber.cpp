#include "Grabber.h"




Grabber::Grabber() :
	grabberPuller(PHOENIX2014_GRABBER_ACTUATOR)
{

}

void Grabber::OperateGrabber(Joystick * gamePad){
	
	bool openGrabber = gamePad->GetRawButton(2);
	
	if (openGrabber){
	grabberPuller.Set(Relay::kOn);
	}
}

Grabber::~Grabber(){
	
}
