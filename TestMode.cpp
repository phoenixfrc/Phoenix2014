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
	m_driving_motor = true;
}

void TestMode::PerformTesting(Joystick * gamePad,Encoder *encoder, DriverStationLCD * lcd, 
		                      Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch, 
		                      Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic,
		                      UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch, 
		                      Shooter * theShooter)
{
	bool button1 = gamePad->GetRawButton(1); //Gets button one (Blue X)
	bool button2 = gamePad->GetRawButton(2); //Gets button two (Green A)
	bool button3 = gamePad->GetRawButton(3); //Gets button three (Red B)
	bool button4 = gamePad->GetRawButton(4); //Gets button four (Yellow Y)
	bool button5 = gamePad->GetRawButton(5); //Gets button five (LB = top left trigger)
	bool button6 = gamePad->GetRawButton(6); //Gets button six (RB = top right trigger)
	bool button7 = gamePad->GetRawButton(7); //Gets button seven (LT = bottom left trigger)
	bool button8 = gamePad->GetRawButton(8); //Gets button eight (RT = bottom right trigger)
	encoder->SetDistancePerPulse(PHOENIX2014_DRIVE_DISTANCE_PER_PULSE_LEFT);
	
	bool checkBox1 = SmartDashboard::GetBoolean("Checkbox 1");
//	double slider1 = SmartDashboard::GetNumber("Slider 1");
	
	switch (m_mode) {
		case testGamepad:  //Tests the Gamepad
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "GamePad Test");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Press Green Button");
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
				m_mode = testShooter;  //Changeds to testShooter
			}
			break;
		case testShooter:
			float dPadThumbstick = gamePad->GetY();
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Shooter Test");
			theShooter->DisplayDebugInfo(DriverStationLCD::kUser_Line3, lcd);
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Ts = %f", dPadThumbstick);
			
			if(m_driving_motor){
				//if true drive motor 
				theShooter->TestShooter(0.0, dPadThumbstick);
				lcd->PrintfLine(DriverStationLCD::kUser_Line5, "Drive winch");
				if(button4){
					m_driving_motor = false;
					//turn off driving motor
					theShooter->TestShooter(0.0, 0.0);			
				}
			}
			
			else{
				//drive brake
				theShooter->TestShooter(dPadThumbstick, 0.0);
				lcd->PrintfLine(DriverStationLCD::kUser_Line5, "Drive brake");
				if(button4) {
					m_driving_motor = true; 
					//stop the brake motor
					theShooter->TestShooter(0.0, 0.0);
				}
			}
			if(button2){
				m_mode = testGrabber;  //Changes mode to testGrabber
			//turn both motors off
				theShooter->TestShooter(0.0, 0.0);
			}
			
			break;
		case testGrabber:
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Grabber NI");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Press Green Button");
			if(button2){
				m_mode = testElevator;  //Changes mode to testElevator
			}
			break;
		case testElevator:
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Elevator NI");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Press Green Button");
			
			if(button2){
				m_mode = testJoystick;  //Changes mode to Test Joystick
			}
			break;
		case testJoystick:  //Tests the Joysticks
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "LJS=%f,RJS=%f", //prints out Joystick values to LCD Display
					leftJoyStick->GetAxis(Joystick::kYAxis),		
					rightJoyStick->GetAxis(Joystick::kYAxis)
					
							);
			
			
			if(button2){
				m_mode = testTalon; //Changes mode to test Talon
			}
			break;
		case testTalon:  //Tests the Talons
			float desiredValue = rightJoyStick->GetAxis(Joystick::kYAxis);
			testTalons->Set(desiredValue);
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "J%5.3,T%5.3f",desiredValue, testTalons->Get());
			
			//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%5.3f %5.3f %5.3f", lJoyStick, rJoyStick, SmartDashboard::GetNumber("Slider 1"));
							//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "DEV=%6.2fSP=%6.2f", ballGrabber.desiredElevatorVoltage, ballGrabber.elevatorController.GetSetpoint());
							//lcd->PrintfLine(DriverStationLCD::kUser_Line6, "CEV=%6.2fEE=%6.2f",
							//	ballGrabber.elevatorAngleSensor.PIDGet(),
							//	ballGrabber.elevatorController.GetError());
			
			
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
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%d", encoder->Get());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", encoder->GetRate());
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "%f", encoder->GetDistance());
			
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
