#include "TestMode.h"
#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"

TestMode::TestMode(DriverStation * theDriverStation):
	testUltrasonic1(2, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
	testUltrasonic2(2, 3),
	testUltrasonic3(2, 4),
   m_dsIO(theDriverStation->GetEnhancedIO())
{
	m_mode = testGamepad;
	m_ds = theDriverStation;
}

void TestMode::PerformTesting(Joystick * gamePad,Encoder *encoder, DriverStationLCD * lcd, 
		                      Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch, 
		                      Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic,
		                      UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch)
{
	bool button1 = gamePad->GetRawButton(1); //Gets button one (Blue X)
	bool button2 = gamePad->GetRawButton(2); //Gets button two (Green A)
	bool button3 = gamePad->GetRawButton(3); //Gets button three (Red B)
	bool button4 = gamePad->GetRawButton(4); //Gets button four (Yellow Y)
	bool button5 = gamePad->GetRawButton(5); //Gets button five (LB = top left trigger)
	bool button6 = gamePad->GetRawButton(6); //Gets button six (RB = top right trigger)
	bool button7 = gamePad->GetRawButton(7); //Gets button seven (LT = bottom left trigger)
	bool button8 = gamePad->GetRawButton(8); //Gets button eight (RT = bottom right trigger)
	
	bool checkBox1 = SmartDashboard::GetBoolean("Checkbox 1");
	double slider1 = SmartDashboard::GetNumber("Slider 1");
	
	switch (m_mode) {
		case testGamepad:  //Tests the Gamepad
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "gamepad=%c%c%c%c %c%c%c%c:%c", //prints the button values to LCD display
						    button1 ? '1':'0', 
						    button2 ? '1':'0',
					        button3 ? '1':'0',		
					        button4 ? '1':'0',		
					        button5 ? '1':'0',		
					        button6 ? '1':'0',				
					        button7 ? '1':'0',		
							button8 ? '1':'0',
							checkBox1 ? '1':'0'
							);
			lcd->PrintfLine(DriverStationLCD::kUser_Line6, "Thumstick=%f", gamePad->GetX());
			// SmartDashboard::PutNumber("Team Number", 2342);
			if(button2){
				m_mode = testJoystick;  //Changes mode to Test Joystick
			}
			break;
		case testJoystick:  //Tests the Joysticks
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "LJS = %f, RJS = %f", //prints out Joystick values to LCD Display
					leftJoyStick->GetAxis(Joystick::kXAxis),		
					rightJoyStick->GetAxis(Joystick::kXAxis)
					
							);
			
			
			if(button2){
				m_mode = testTalon; //Changes mode to test Talon
			}
			break;
		case testTalon:  //Tests the Talons
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Testing Talons, %d", testTalons->Get());
			
			
			if(button2){
				m_mode = testIO;  //Changes mode to test IO
				
			}
			break;
		case testIO:  //Tests the Input and Outputs
			
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "testingIO, %d", testSwitch->Get());
			if(button2){
				m_mode = testEncoder; //changes mode to test Encoder
			}
			break;
		case testEncoder:  //Tests the Encoders
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Testing Encoder");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", encoder->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", encoder->GetRate());
			
			if(button2){
				m_mode = ultrasonicTestMode; //Changes mode to Test Ultrasonic
			}
			break;
		case ultrasonicTestMode:
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "%f", frontUltrasonic->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", backUltrasonic->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", grabberUltrasonic->GetDistance());
			//testSwitch = new AnalogIOButton(5);
			//lcd->PrintfLine(DriverStationLCD::kUser_Line1, "us%f", ultrasonic->GetVoltage());
			//lcd->PrintfLine(DriverStationLCD::kUser_Line1, "AB%d", testSwitch->Get());
			
			
			if(button2){
				m_mode = analogSwitchMode;
			}
			break;
		case analogSwitchMode:
			analogTestSwitch->SetLimitsVoltage(1.5, 3);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "AB%c", 
			                analogTestSwitch->GetTriggerState() ? '1':'0'
			                );
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
