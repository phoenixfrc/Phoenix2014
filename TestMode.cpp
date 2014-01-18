#include "TestMode.h"

TestMode::TestMode(){
	m_mode = testGamepad;
	
	
}

void TestMode::PerformTesting(Joystick * gamePad, Encoder *encoder, DriverStationLCD * lcd)
{
	bool button1 = gamePad->GetRawButton(1);
	bool button2 = gamePad->GetRawButton(2);
	bool button3 = gamePad->GetRawButton(3);
	bool button4 = gamePad->GetRawButton(4);
	bool button5 = gamePad->GetRawButton(5);
	bool button6 = gamePad->GetRawButton(6);
	bool button7 = gamePad->GetRawButton(7);
	bool button8 = gamePad->GetRawButton(8);
	
	switch (m_mode) {
		case testGamepad:
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "gamepad = %c%c%c%c %c%c%c%c",
					button1 ? '1':'0',
					button2 ? '1':'0',
					button3 ? '1':'0',		
					button4 ? '1':'0',		
					button5 ? '1':'0',		
					button6 ? '1':'0',				
					button7 ? '1':'0',		
					button8 ? '1':'0');
			if(button2){
				m_mode = testJoystick;
			}
			break;
		case testJoystick:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Testing joystick");
			
			if(button2){
				m_mode = testTalon;
			}
			break;
		case testTalon:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Testing Talon");
			
			if(button2){
				m_mode = testIO;
			}
			break;
		case testIO:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "testingIO");
			if(button2){
				m_mode = testEncoder;
			}
			break;
		case testEncoder:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Testing Encoder");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", encoder->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", encoder->GetRate());
			
			if(button2){
				m_mode = testGamepad;
			}
			
			
			break;
		default:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "unknown mode");
			break;
	}
}

TestMode::~TestMode(){
	
}
