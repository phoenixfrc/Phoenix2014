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
	DriverStationLCD * lcd;
	Preferences * dashboardPreferences;
	RobotDrive driveTrain; // robot drive system
	Joystick rightJoyStick; // rightStick wired to port 1
	Joystick leftJoyStick;  // leftStick wired to port 2
	Joystick gamePad;
	Grabber ballGrabber;
	Encoder driveDistanceRight;
	Encoder driveDistanceLeft;
	Shooter shooter;
	DigitalInput testSwitch; //TODO get rid of testSwitch.
	Talon testTalons; //TODO get rid of testTalons.
	UltrasonicSensor frontUltrasonic;
	UltrasonicSensor backUltrasonic;
	AnalogTrigger analogTestSwitch;
	Relay lightBulb;
	bool m_display_page_1;
	bool m_FromAutonomous;
	
public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		lcd(DriverStationLCD::GetInstance()),
		dashboardPreferences(Preferences::GetInstance()),
		driveTrain(PHOENIX2014_DRIVEMOTOR_LEFT_REAR,PHOENIX2014_DRIVEMOTOR_RIGHT_REAR), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		rightJoyStick(2),// as they are declared above.
		leftJoyStick(1),
		gamePad(3),
		ballGrabber(&gamePad),
		driveDistanceRight(PHOENIX2014_R_DRIVE_ENCODER_A,PHOENIX2014_R_DRIVE_ENCODER_B ),
		driveDistanceLeft(PHOENIX2014_L_DRIVE_ENCODER_A, PHOENIX2014_L_DRIVE_ENCODER_B),
		testSwitch(3),
		testTalons(PHOENIX2014_DRIVEMOTOR_LEFT_FRONT),
		frontUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
		backUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_BACK),
		analogTestSwitch(PHOENIX2014_ANALOG_MODULE_NUMBER, 5),
		lightBulb(PHOENIX2014_RELAY_LIGHTBULB)
	{
		driveTrain.SetExpiration(0.1);
		driveTrain.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		driveTrain.SetInvertedMotor(RobotDrive::kRearRightMotor, false); 
		m_FromAutonomous = false;
	}
	
	float limitSpeed(float requestedSpeed)
	{
		float scaleSpeed = SmartDashboard::GetNumber("Extra1");
		if(scaleSpeed <= 0){
			scaleSpeed = 0;
		}
		if(scaleSpeed > 100){
			scaleSpeed = 100.0;
		}
		return requestedSpeed * 0.01 * scaleSpeed;
		
	}
	
	// Executed at Robot power-on
	void RobotInit(){
	//move initial code from inside operator controll
		m_display_page_1 = false;
		Wait(.5); //Give dashboard preferences time to load.
		double P = dashboardPreferences->GetDouble("P");
		double I = dashboardPreferences->GetDouble("I");
		double D = dashboardPreferences->GetDouble("D");
		double angleMeasure = dashboardPreferences->GetDouble("Angle");
		double length = dashboardPreferences->GetDouble("Length");
		double grabberMeasure = dashboardPreferences->GetDouble("Grabber");
		double extra1 = dashboardPreferences->GetDouble("Extra1");
		double extra2 = dashboardPreferences->GetDouble("Extra2");
		double extra3 = dashboardPreferences->GetDouble("Extra3");
		double slider1 = dashboardPreferences->GetDouble("Slider1");
		SmartDashboard::PutNumber("P", P);
		SmartDashboard::PutNumber("I", I);
		SmartDashboard::PutNumber("D", D);
		SmartDashboard::PutNumber("Angle", angleMeasure);
		SmartDashboard::PutNumber("Length", length);
		SmartDashboard::PutNumber("Grabber", grabberMeasure);
		SmartDashboard::PutNumber("Extra1", extra1);
		SmartDashboard::PutNumber("Extra2", extra2);
		SmartDashboard::PutNumber("Extra3", extra3);
		SmartDashboard::PutNumber("Slider 1", slider1);
		m_FromAutonomous = false;
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
		m_FromAutonomous = true;
		//float rightDriveSpeed = -1.0;
		//float leftDriveSpeed = -1.0;
		//int rangeToWallClose = 60;
		//int rangeToWallFar = 120;
		//Initialize encoder.
		//int distanceToShoot = 133;
		//int shootDelay = 0;
		//ballGrabber.elevatorController.SetSetpoint(PHOENIX2014_INITIAL_AUTONOMOUS_ELEVATOR_ANGLE);
		//TODO Remove encoders from code??
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
		int time = 0;
		
		//TODO Verify times are good for competition robot. "D.M"
		int maxDriveLoop = 4*200; // 4 seconds @200 times/sec
		int minDriveLoop = 1*200;
		bool shootingBall = false;
		bool wantFirstShot = true;

		if(checkBox1 == false){
			int printDelay = 0;
			//Ultrasonic Autonomous
			bool isInRange = false;
			while(IsAutonomous() && IsEnabled())
			{
				float currentDistance = frontUltrasonic.GetAverageDistance();
				// Transition isInRange from false to true once
				if((minDistance < currentDistance) && (currentDistance < maxDistance)){
					isInRange = true;
				}
				time++;
				bool motorDriveTimeExceeded = time > maxDriveLoop;
				bool motorMinMet = time > minDriveLoop;
				if(isInRange && motorMinMet){
					driveTrain.TankDrive(0.0,0.0);
					if((ballGrabber.elevatorAngleSensor.GetVoltage() > PHOENIX2014_AUTONOMOUS_ELEVATOR_ANGLE - 0.05) &&
							(ballGrabber.elevatorAngleSensor.GetVoltage() < PHOENIX2014_AUTONOMOUS_ELEVATOR_ANGLE + 0.05)){
						//Enough to cover break release and start winding again.
						
						shootingBall = shooter.OperateShooter(wantFirstShot);
					}
					if(shootingBall == true){
						wantFirstShot = false;
					}
				}
				else if(motorDriveTimeExceeded){
					driveTrain.TankDrive(0.0,0.0);
				}
				else{
					if((currentDistance < closeDistance) && motorMinMet){
						autoDriveSpeed = autoDriveSlowSpeed;
					}
					driveTrain.TankDrive(-0.95 * autoDriveSpeed, -1.0 * autoDriveSpeed);//the DriveTrain is BACKWARD
				}
				ballGrabber.RunElevatorAutonomous(PHOENIX2014_AUTONOMOUS_ELEVATOR_ANGLE);
				printDelay = printDelay++;
				if(printDelay >= 200){
					lcd->Clear();
					lcd->PrintfLine(DriverStationLCD::kUser_Line1, "In Autonomous");
					shooter.DisplayDebugInfo(DriverStationLCD::kUser_Line2, lcd);
					shooter.PrintShooterState(DriverStationLCD::kUser_Line3, lcd);
					lcd->PrintfLine(DriverStationLCD::kUser_Line4, "Ulra=%f", frontUltrasonic.GetAverageDistance());
					//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "CEV=%f", ballGrabber.elevatorAngleSensor.GetVoltage());
					//lcd->PrintfLine(DriverStationLCD::kUser_Line6, "DEV=%f", ballGrabber.m_desiredElevatorVoltage);
					lcd->UpdateLCD();
					printDelay = 0;
				}
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
				shooter.OperateShooter(shooting);
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
		if(!m_FromAutonomous){
			init();
		}
		m_FromAutonomous = false;
		driveTrain.SetSafetyEnabled(true);
		
		int printDelay = 0;
		int shootDelay = 0;
		//bool SavePreferencesToFlash = false;
		
		while (IsOperatorControl() && IsEnabled())
		{
			/*
		    bool SavePreferences = gamePad.GetRawButton(8);
			if (SavePreferences){
				double elevatorAngleValue = SmartDashboard::GetNumber("Angle");
				dashboardPreferences->PutDouble("Angle", elevatorAngleValue);
				SavePreferencesToFlash = true;
			}
			*/
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

			//TODO Probably don't need but want to test because called inside operate grabber.
			ballGrabber.OperatePIDLoop();

			if(printDelay == 100){
				lcd->Clear();
				if(m_display_page_1)
				{
					lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Teleop pg1");
					lcd->PrintfLine(DriverStationLCD::kUser_Line2, "FR %4.0f, BA %4.0f",
							        frontUltrasonic.GetAverageDistance(),
							        backUltrasonic.GetAverageDistance());
					shooter.PrintShooterState(DriverStationLCD::kUser_Line3, lcd);
					SmartDashboard::PutNumber("UltrasonicF", 1);
					SmartDashboard::PutNumber("UltrasonicB", 1);                                         
					SmartDashboard::PutNumber("ElvatorAngle", 2);//Change keyname to ElavatorAngle from (ElvatorAngle)
																										 //^^
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
			bool shooterButton = gamePad.GetRawButton(7) || gamePad.GetRawButton(8);//TODO make constants
			bool automaticAimButton = gamePad.GetRawButton(1);
			//float distanceToWall = frontUltrasonic.GetAverageDistance();
			//bool loadShooterButton = gamePad.GetRawButton(8);
			if (shooterButton && shootDelay == 0){
				shootDelay++;
			}
			if(shootDelay>0){
				shootDelay++;
			}
			bool ReadyToShoot = (shootDelay>PHOENIX2014_LOOP_COUNT_FOR_SHOOT_DELAY);
			shooter.OperateShooter(ReadyToShoot);
			if (ReadyToShoot){
				shootDelay = 0;
			}
			bool okToGrab = (shootDelay == 0);//Normaly 0 unless delaying
			ballGrabber.OperateGrabber(shooterButton, okToGrab);
			//Trying to make some things happen automatically during teleoperated
			if(automaticAimButton){
				ballGrabber.m_desiredElevatorVoltage = PHOENIX2014_AUTONOMOUS_ELEVATOR_ANGLE;
			}
			//((distanceToWall > (12.0*11.0)) && distanceToWall < (12.0*13.0)){
				//lightBulb.Set(Relay::kOn);
			//}
			//else{
			
#ifdef WANTWEIRDPULSING
			if (printDelay < 30) {
				lightBulb.Set(Relay::kForward);
			}
			else if (printDelay >= 30 && printDelay < 65) {
				lightBulb.Set(Relay::kReverse);
			}
			else {
				lightBulb.Set(Relay::kOff);
			}
#endif
			
			Wait(0.005);// wait for a motor update time
		} // end of while enabled
		driveTrain.StopMotor();
		ballGrabber.StopPidLoop();
		shooter.motorShutOff();
		
		/*
		if(SavePreferencesToFlash){
			dashboardPreferences->Save();
			SavePreferencesToFlash = false;
		}
		*/
			
	} // end of OperatorControl()
		
	/**
	 * Runs during test mode	```````
	 */
	void Test() {
		TestMode tester(m_ds);
		driveDistanceRight.Reset();
		driveDistanceRight.Start();
		ballGrabber.resetSetPoint();
		shooter.motorShutOff();
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

