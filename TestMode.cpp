#include "TestMode.h"
#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"


TestMode::TestMode(DriverStation * theDriverStation):
	testUltrasonic1(2, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
	testUltrasonic2(2, 3),
	testUltrasonic3(2, 4),
	dashboardPreferences(Preferences::GetInstance()),
   m_dsIO(theDriverStation->GetEnhancedIO())
{
	m_mode = testGamepad;
	m_ds = theDriverStation;
	m_driving_motor = true;
	m_savePreferencesToFlash = false;

}

void TestMode::PerformTesting(Joystick * gamePad,Encoder *encoder, DriverStationLCD * lcd, 
		                      Joystick * rightJoyStick, Joystick * leftJoyStick, DigitalInput * testSwitch, 
		                      Talon * testTalons, UltrasonicSensor * frontUltrasonic, UltrasonicSensor * backUltrasonic,
		                      UltrasonicSensor * grabberUltrasonic, AnalogTrigger * analogTestSwitch, 
		                      Shooter * theShooter, Grabber * theElevatorAndGrabber)
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
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "Tx=%5.2f, Ty=%5.2f", gamePad->GetX(), gamePad->GetY());
			// SmartDashboard::PutNumber("Team Number", 2342);
			if(button2){
				m_mode = testLimitSwitches;  //Changeds to testShooter
			}
			if(button3){
				m_mode = savePreferences;
			}
			
			break;
		case testLimitSwitches:  //tests all the limit switches and potentiometer.

			theElevatorAndGrabber->DisplayDebugInfo(DriverStationLCD::kUser_Line1,lcd);
			theShooter->DisplayDebugInfo(DriverStationLCD::kUser_Line2, lcd);
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "CEV=%6.2f",
					theElevatorAndGrabber->elevatorAngleSensor.GetVoltage());
			if(button2){m_mode = testShooter;  //Changeds to testShooter
		
			}
			if(button3){
				m_mode = testGamepad;
			}
			break;
		case testShooter:
			float dPadThumbstick = GetThumbstickWithZero(gamePad);
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
			if(button3){
				m_mode = testLimitSwitches;
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
			if(button3){
				m_mode = testLimitSwitches;
			}
			
			break;
		case testGrabber:
			dPadThumbstick = GetThumbstickWithZero(gamePad);
			theElevatorAndGrabber->grabberActuator.Set(dPadThumbstick);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Grabber NI");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Press Green Button");
			theElevatorAndGrabber->DisplayDebugInfo(DriverStationLCD::kUser_Line3,lcd);
			
			if(button2){
				theElevatorAndGrabber->elevatorController.Disable();
				m_mode = testElevator;  //Changes mode to testElevator
			
			}
			if(button3){
				m_mode = testShooter;
			}
			break;
		case testElevator:
			dPadThumbstick = GetThumbstickWithZero(gamePad);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Elevator Test");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Press Green Button");
			theElevatorAndGrabber->DisplayDebugInfo(DriverStationLCD::kUser_Line3, lcd);
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Ts = %f", dPadThumbstick);
			theElevatorAndGrabber->DriveElevatorTestMode(dPadThumbstick);
			
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "CEV=%6.2f",
							theElevatorAndGrabber->elevatorAngleSensor.GetVoltage());
			
			if(button2){
				m_mode = testJoystick;  //Changes mode to Test Joystick
				theElevatorAndGrabber->resetSetPoint();
				// theElevatorAndGrabber->elevatorController.Enable();
				
			}
			if(button3){
				m_mode = testGrabber;
			}
			break;
		case testJoystick:  //Tests the Joysticks
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "LJS=%f,RJS=%f", //prints out Joystick values to LCD Display
					leftJoyStick->GetAxis(Joystick::kYAxis),		
					rightJoyStick->GetAxis(Joystick::kYAxis)
					
							);
			
			
			if(button2){
				m_mode = testEncoder; //Changes mode to test Talon
			}
			if(button3){
				m_mode = testElevator;
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
			if(button3){
				m_mode = testJoystick;
			}
			break;
		case ultrasonicTestMode:
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "%f", frontUltrasonic->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", frontUltrasonic->GetAverageDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", backUltrasonic->GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%f", backUltrasonic->GetAverageDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line5, "ultrasonicTestMode");
			//testSwitch = new AnalogIOButton(5);
			//lcd->PrintfLine(DriverStationLCD::kUser_Line1, "us%f", ultrasonic->GetVoltage());
			//lcd->PrintfLine(DriverStationLCD::kUser_Line1, "AB%d", testSwitch->Get());
			
			
			if(button2){
				m_mode = analogSwitchMode;
			}
			if(button3){
				m_mode = testEncoder;
			}
			break;
		case analogSwitchMode:
			analogTestSwitch->SetLimitsVoltage(1.5, 3);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "AB%c", 
			                analogTestSwitch->GetTriggerState() ? '1':'0'
			                );
			if(button2){
				m_mode = savePreferences;
			}
			if(button3){
				m_mode = ultrasonicTestMode;
			}
            break;
		case savePreferences:
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Save Preferences case");
			bool savePreferences = button8;
			
			if(savePreferences){
				double pValue = SmartDashboard::GetNumber("P");
				double iValue = SmartDashboard::GetNumber("I");
				double dValue = SmartDashboard::GetNumber("D");
				double elevatorAngleValue = SmartDashboard::GetNumber("Angle");
				double distanceValue = SmartDashboard::GetNumber("Length");
				double grabberValue = SmartDashboard::GetNumber("Grabber");
				double extra1Value = SmartDashboard::GetNumber("Extra1");
				double extra2Value = SmartDashboard::GetNumber("Extra2");
				double extra3Value = SmartDashboard::GetNumber("Extra3");
				double slider1Value = SmartDashboard::GetNumber("Slider 1");
				dashboardPreferences->PutDouble("P", pValue);
				dashboardPreferences->PutDouble("I", iValue);
				dashboardPreferences->PutDouble("D", dValue);
				dashboardPreferences->PutDouble("Angle", elevatorAngleValue);
				dashboardPreferences->PutDouble("Length", distanceValue);
				dashboardPreferences->PutDouble("Grabber", grabberValue);
				dashboardPreferences->PutDouble("Extra1", extra1Value);
				dashboardPreferences->PutDouble("Extra2", extra2Value);
				dashboardPreferences->PutDouble("Extra3", extra3Value);
				dashboardPreferences->PutDouble("Slider1", slider1Value);
				m_savePreferencesToFlash = true;
				lcd->PrintfLine(DriverStationLCD::kUser_Line2, "added pref to be saved");
				lcd->PrintfLine(DriverStationLCD::kUser_Line3, "angle=%f", elevatorAngleValue);
				lcd->UpdateLCD();
				Wait(1);
			}
			
			if(button2){
				m_mode = testGamepad;
				if(m_savePreferencesToFlash){
					dashboardPreferences->Save();
					m_savePreferencesToFlash = false;
					lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Saved");
					lcd->UpdateLCD();
					Wait(5);
			}
				if(button3){
					m_mode = analogSwitchMode;
				}
		break;
		default:
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "unknown mode");
			break;
	}
	
	}
}
float TestMode::GetThumbstickWithZero(Joystick * gamePad){
	float rawValue = gamePad->GetY();
	if(rawValue < 0.07 && rawValue > -0.07){
		return 0.0;
	}
	return rawValue;
}

float TestMode::GetTwistWithZero(Joystick * gamePad){
	float rawValue = gamePad->GetTwist();
	if(rawValue < 0.07 && rawValue > -0.07){
		return 0.0;
	}
	return rawValue;
}

TestMode::~TestMode(){
	
}
