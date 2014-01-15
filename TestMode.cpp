#include "TestMode.h"

TestMode::TestMode(){
	
}

void TestMode::performTesting(Joystick * gamePad, DriverStationLCD * lcd)
{
	bool button1 = gamePad->GetRawButton(1);
	bool button2 = gamePad->GetRawButton(2);
	bool button3 = gamePad->GetRawButton(3);
	bool button4 = gamePad->GetRawButton(4);
	bool button5 = gamePad->GetRawButton(5);
	bool button6 = gamePad->GetRawButton(6);
	bool button7 = gamePad->GetRawButton(7);
	bool button8 = gamePad->GetRawButton(8);
	lcd->PrintfLine(DriverStationLCD::kUser_Line5, "gamepad = %c%c%c%c %c%c%c%c",
			button1 ? '1':'0',
			button2 ? '1':'0',
			button3 ? '1':'0',		
			button4 ? '1':'0',		
			button5 ? '1':'0',		
			button6 ? '1':'0',				
			button7 ? '1':'0',		
			button8 ? '1':'0');
}

TestMode::~TestMode(){
	
}
