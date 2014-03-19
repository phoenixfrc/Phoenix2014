#include "WPILib.h"
#include "Shooter.h"
#include "Grabber.h"
#include "TestMode.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"
#include "Preferences.h"



/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive driveTrain; // robot drive system
	Joystick rightJoyStick; // rightStick wired to port 1
	Joystick leftJoyStick;  // leftStick wired to port 2
	Joystick gamePad;
	Grabber ballGrabber;
	//Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistanceRight;
	Encoder driveDistanceLeft;
	//Encoder testEncoder;
	Shooter shooter;
	DigitalInput testSwitch;
	Talon testTalons;
	UltrasonicSensor frontUltrasonic;
	UltrasonicSensor backUltrasonic;
	AnalogTrigger analogTestSwitch;
	//RobotDrive speedLimiter;
	DriverStationLCD * lcd;
	Preferences * dashboardPreferences;
	bool m_display_page_1;
	
public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		//myRobot(1, 2, 3, 4),	// lr, lf, rr, rf pwm channels,
		driveTrain(PHOENIX2014_DRIVEMOTOR_LEFT_REAR,PHOENIX2014_DRIVEMOTOR_RIGHT_REAR), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		rightJoyStick(2),// as they are declared above.
		leftJoyStick(1),
		gamePad(3),
		ballGrabber(&gamePad),
		//elevation(1,2),
		driveDistanceRight(PHOENIX2014_R_DRIVE_ENCODER_A,PHOENIX2014_R_DRIVE_ENCODER_B ),
		driveDistanceLeft(PHOENIX2014_L_DRIVE_ENCODER_A, PHOENIX2014_L_DRIVE_ENCODER_B),
		//testEncoder(5,6),
		testSwitch(3),
		testTalons(PHOENIX2014_DRIVEMOTOR_LEFT_FRONT),
		frontUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
		backUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_BACK),
		analogTestSwitch(PHOENIX2014_ANALOG_MODULE_NUMBER, 5),
		//speedLimiter(1, 2),
	    lcd(DriverStationLCD::GetInstance()),
		dashboardPreferences(Preferences::GetInstance())
	{
		driveTrain.SetExpiration(0.1);
		driveTrain.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		driveTrain.SetInvertedMotor(RobotDrive::kRearRightMotor, false); 
	}
	
	float limitSpeed(float requestedSpeed)
	{
		float scaleSpeed = SmartDashboard::GetNumber("Slider 1");
		return requestedSpeed * 0.01 * scaleSpeed;
	}
	
	// Executed at Robot power-on
	void RobotInit(){
	//move initial code from inside operator controll
		m_display_page_1 = false;
		Wait(.5);
		double P = dashboardPreferences->GetDouble("P");
		double I = dashboardPreferences->GetDouble("I");
		double D = dashboardPreferences->GetDouble("D");
		double angleMeasure = dashboardPreferences->GetDouble("Angle");
		double distanceMeasure = dashboardPreferences->GetDouble("Distance");
		double grabberMeasure = dashboardPreferences->GetDouble("Grabber");
		double extra1 = dashboardPreferences->GetDouble("Extra1");
		double extra2 = dashboardPreferences->GetDouble("Extra2");
		double extra3 = dashboardPreferences->GetDouble("Extra3");
		double slider1 = dashboardPreferences->GetDouble("Slider 1");
		SmartDashboard::PutNumber("P", P);
		SmartDashboard::PutNumber("I", I);
		SmartDashboard::PutNumber("D", D);
		SmartDashboard::PutNumber("Angle", angleMeasure);
		SmartDashboard::PutNumber("Distance", distanceMeasure);
		SmartDashboard::PutNumber("Grabber", grabberMeasure);
		SmartDashboard::PutNumber("Extra1", extra1);
		SmartDashboard::PutNumber("Extra2", extra2);
		SmartDashboard::PutNumber("Extra3", extra3);
		SmartDashboard::PutNumber("Slider 1", slider1);
		/*double pFromDashboard = SmartDashboard::GetNumber("P");
		double iFromDashboard = SmartDashboard::GetNumber("I");
		double dFromDashboard = SmartDashboard::GetNumber("D");
		double angleFromDashboard = SmartDashboard::GetNumber("Angle");
		double distanceFromDashboard = SmartDashboard::GetNumber("Distance");
		double grabberFromDashboard = SmartDashboard::GetNumber("Grabber");
		double extra1FromDashboard = SmartDashboard::GetNumber("Extra1");
		double extra2FromDashboard = SmartDashboard::GetNumber("Extra2");
		double extra3FromDashboard = SmartDashboard::GetNumber("Extra3");
		double sliderFromDashboard = SmartDashboard::GetNumber("Slider 1");*/
	}
	
	//this called when the robot is enabled
	void init(){
		shooter.init();
		ballGrabber.init();
	}
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */ 
	void Autonomous()
	{
		init();
		lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Entered Autonomous");
		driveTrain.SetSafetyEnabled(false);
		bool checkBox1 = SmartDashboard::GetBoolean("Checkbox 1");
		//float rightDriveSpeed = -1.0;
		//float leftDriveSpeed = -1.0;
		//int rangeToWallClose = 60;
		//int rangeToWallFar = 120;
		//Initialize encoder.
		//int distanceToShoot = 133;
		//int shootDelay = 0;
		//ballGrabber.elevatorController.SetSetpoint(PHOENIX2014_INITIAL_AUTONOMOUS_ELEVATOR_ANGLE);
		driveDistanceRight.Reset();
		driveDistanceLeft.Reset();
		driveDistanceRight.SetDistancePerPulse(PHOENIX2014_DRIVE_DISTANCE_PER_PULSE_RIGHT);
		driveDistanceLeft.SetDistancePerPulse(PHOENIX2014_DRIVE_DISTANCE_PER_PULSE_LEFT);
		driveDistanceRight.Start();
		driveDistanceLeft.Start();
		//int printDelay = 0;
		float minDistance = 52.0;  // Closer to the wall than this is too close
		float maxDistance = 144.0; // Once at least this close, within shooting range
		float closeDistance = maxDistance + 24.0;
		float autoDriveSpeed = 0.55;
		float autoDriveSlowSpeed = 0.38;
		
		int maxDriveLoop = 5*200; // 5 seconds @200 times/sec
		if(checkBox1 == false){
			//Ultrasonic Autonomous
			bool isInRange = false;
			while(IsAutonomous() && IsEnabled())
			{
				float currentDistance = frontUltrasonic.GetAverageDistance();
				// Transition isInRange from false to true once
				if((minDistance < currentDistance) && (currentDistance < maxDistance)){
					isInRange = true;
				}
				maxDriveLoop --;
				bool motorDriveTimeExceeded = maxDriveLoop < 0;
				if(isInRange){
					driveTrain.TankDrive(0.0,0.0);
					if((ballGrabber.elevatorAngleSensor.GetVoltage() > 2.06) && (ballGrabber.elevatorAngleSensor.GetVoltage() < 2.10)){
						shooter.OperateShooter(true, false);
					}
				}
				else if(motorDriveTimeExceeded){
					driveTrain.TankDrive(0.0,0.0);
				}
				else{
					if(currentDistance < closeDistance){
						autoDriveSpeed = autoDriveSlowSpeed;
					}
					driveTrain.TankDrive(-1.0 * autoDriveSpeed - 0.03, -1.0 * autoDriveSpeed);
				}
				ballGrabber.RunElevatorAutonomous(2.08);
/****
				printDelay = printDelay++;
				if(printDelay >= 100){
					lcd->Clear();
					lcd->PrintfLine(DriverStationLCD::kUser_Line1, "In Autonomous");
					lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Ulra=%f", frontUltrasonic.GetAverageDistance());
					lcd->PrintfLine(DriverStationLCD::kUser_Line3, "CEV=%f", ballGrabber.elevatorAngleSensor.GetVoltage());
					lcd->PrintfLine(DriverStationLCD::kUser_Line4, "DEV=%f", ballGrabber.m_desiredElevatorVoltage);
					lcd->UpdateLCD();
					printDelay = 0;
				}
****/
				Wait(0.005);
			}
			lcd->Clear();
			lcd->UpdateLCD();
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Exiting Autonomous");
		} else {
			//Timer Autonomous
			driveTrain.TankDrive(-0.5,-0.5);
			ballGrabber.DriveElevatorTestMode(-1.0);
			lcd->Clear();
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Skip Auto");
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "CheckBox Checked");
			lcd->UpdateLCD();
			Wait(2.0);
			bool shooting = true;
			driveTrain.TankDrive(0.0,0.0);
			int counter = 0;
			while(counter < 600){
				shooter.OperateShooter(shooting, false);
				Wait(0.005);
			}
		}
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	/*void CameraMotor(){
		const float minOffSet = 0.05;
		if (gamePad.GetY() > minOffSet || gamePad.GetY() < -minOffSet){
			
		}
	}*/
	
	void OperatorControl()
	{
		init();
		
		driveTrain.SetSafetyEnabled(true);
		
		int printDelay = 0;
		int shootDelay = 0;
		bool SavePreferencesToFlash = false;
		
		while (IsOperatorControl() && IsEnabled())
		{
			bool SavePreferences = gamePad.GetRawButton(8);
			if (SavePreferences){
				double elevatorAngleValue = SmartDashboard::GetNumber("Angle");
				dashboardPreferences->PutDouble("Angle", elevatorAngleValue);
				SavePreferencesToFlash = true;
			}
			printDelay ++;
			
			float rJoyStick = limitSpeed(rightJoyStick.GetY());
			float lJoyStick = limitSpeed(leftJoyStick.GetY());
			bool button6 = gamePad.GetRawButton(6);
			
			//speedLimiter.SetMaxOutput(SmartDashboard::GetNumber("Slider 1"));
			driveTrain.TankDrive(lJoyStick, rJoyStick);
			
			//manual mode(no PID) for elevator
			//float dPadThumbstick = TestMode::GetThumbstickWithZero(&gamePad);
			//ballGrabber.DriveElevatorTestMode(dPadThumbstick);
			//Sets motor equal to the elevator sensor.

			ballGrabber.OperatePIDLoop();
			

		//organize lcd code limit to 2 times per second
			if(printDelay == 100){
				//float readings[100];
				//readings[loopCounter%100];
				//do average();
				lcd->Clear();
				if(m_display_page_1)
				{
					lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Teleop pg1");
					lcd->PrintfLine(DriverStationLCD::kUser_Line2, "FR %4.0f, BA %4.0f", frontUltrasonic.GetDistance(), backUltrasonic.GetDistance());
					shooter.PrintShooterState(DriverStationLCD::kUser_Line3, lcd);

					if(button6){
						m_display_page_1 = false;
					}
				}
				else{
					lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Teleop pg2 %c", button6 ? '1':'0');
					ballGrabber.DisplayDebugInfo(DriverStationLCD::kUser_Line2,lcd);
					//lcd->PrintfLine(DriverStationLCD::kUser_Line3, "G%f", ballGrabber.ballDetector.GetDistance());
					//ballGrabber.UpDateWithState(DriverStationLCD::kUser_Line3,lcd);
					shooter.PrintShooterState(DriverStationLCD::kUser_Line3, lcd);
					//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "EV%6.2f", ballGrabber.elevatorAngleSensor.GetVoltage());
					shooter.DisplayDebugInfo(DriverStationLCD::kUser_Line4, lcd);
					//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%5.3f %5.3f %5.3f", lJoyStick, rJoyStick, SmartDashboard::GetNumber("Slider 1"));
					lcd->PrintfLine(DriverStationLCD::kUser_Line5, "DEV=%6.3f", ballGrabber.m_desiredElevatorVoltage);
					lcd->PrintfLine(DriverStationLCD::kUser_Line6, "CEV=%5.2f",
									ballGrabber.elevatorAngleSensor.GetVoltage());
					if(button6){
						m_display_page_1 = true;
					}
				}
				
				lcd->UpdateLCD();
				printDelay = 0;
			}
			//int rotation = elevation.Get();
			//the above is commented because we are not using it yet
			bool shooterButton = gamePad.GetRawButton(7);//TODO make constants
			bool loadShooterButton = gamePad.GetRawButton(8);
			if (shooterButton && shootDelay == 0){
				shootDelay++;
			}
			if(shootDelay>0){
				shootDelay++;
			}
			bool ReadyToShoot = (shootDelay>PHOENIX2014_LOOP_COUNT_FOR_SHOOT_DELAY);
			shooter.OperateShooter(ReadyToShoot,loadShooterButton);
			if (ReadyToShoot){
				shootDelay = 0;
			}
			bool okToGrab = (shootDelay == 0);//Normaly 0 unless delaying
			ballGrabber.OperateGrabber(shooterButton, okToGrab);
			//Trying to make some things happen automatically during teleoperated
			
			Wait(0.005);// wait for a motor update time
		} // end of while enabled
		driveTrain.StopMotor();
		ballGrabber.StopPidLoop();	
		
		if(SavePreferencesToFlash){
			dashboardPreferences->Save();
			SavePreferencesToFlash = false;
		}
			
	} // end of OperatorControl()
		
	/**
	 * Runs during test mode	```````
	 */
	void Test() {
		TestMode tester(m_ds);
		driveDistanceRight.Reset();
		driveDistanceRight.Start();
		ballGrabber.resetSetPoint();
		while (IsTest() && IsEnabled()){
			lcd->Clear();
			tester.PerformTesting(&gamePad, &driveDistanceRight, lcd, &rightJoyStick, &leftJoyStick,
								  &testSwitch, &testTalons, &frontUltrasonic, &backUltrasonic,
								  &ballGrabber.ballDetector, &analogTestSwitch,
								  &shooter, &ballGrabber
								  );
			lcd->UpdateLCD();
			Wait(0.1);
			
		}
		driveDistanceRight.Stop();
		}

	
	
};

START_ROBOT_CLASS(RobotDemo);

