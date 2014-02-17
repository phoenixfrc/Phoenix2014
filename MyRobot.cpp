#include "WPILib.h"
#include "Shooter.h"
#include "Grabber.h"
#include "TestMode.h"
#include "Phoenix2014.h"
#include "UltrasonicSensor.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive driveTrain; // robot drive system
	Grabber ballGrabber;
	Joystick rightJoyStick; // rightStick wired to port 1
	Joystick leftJoyStick;  // leftStick wired to port 2
	Joystick gamePad;
	//Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistance;
	Encoder testEncoder;
	Talon elevatorMotor; //check if this is being used
	Shooter shooter;
	DigitalInput testSwitch;
	Talon testTalons;
	UltrasonicSensor frontUltrasonic;
	UltrasonicSensor backUltrasonic;
	AnalogTrigger analogTestSwitch;
	//RobotDrive speedLimiter;
	DriverStationLCD * lcd;
	
	
public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		//myRobot(1, 2, 3, 4),	// lr, lf, rr, rf pwm channels,
		driveTrain(PHOENIX2014_DRIVEMOTOR_LEFT_REAR,PHOENIX2014_DRIVEMOTOR_RIGHT_REAR), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		ballGrabber(), //Place holder for grabber.
		rightJoyStick(2),// as they are declared above.
		leftJoyStick(1),
		gamePad(3),
		//elevation(1,2),
		driveDistance(3,4),
		testEncoder(1,2),
		elevatorMotor(5),
		testSwitch(3),
		testTalons(PHOENIX2014_DRIVEMOTOR_LEFT_FRONT),
		frontUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
		backUltrasonic(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ULTRASONIC_BACK),
		analogTestSwitch(PHOENIX2014_ANALOG_MODULE_NUMBER, 5),
		//speedLimiter(1, 2),
	    lcd(DriverStationLCD::GetInstance())
	{
		driveTrain.SetExpiration(0.1);
		driveTrain.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		driveTrain.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}
	
	float limitSpeed(float requestedSpeed)
	{
		float scaleSpeed = SmartDashboard::GetNumber("Slider 1");
		return requestedSpeed * 0.01 * scaleSpeed;
	}
	void RobotInit(){
	//move initial code from inside operator controll
		ballGrabber.desiredElevatorVoltage = PHOENIX2014_VOLTAGE_AT_VERTICAL;
		ballGrabber.elevatorController.SetSetpoint(ballGrabber.desiredElevatorVoltage);
		ballGrabber.elevatorController.Enable();
	}
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */ 
	void Autonomous()
	{
	        lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Entered Autonomous");
		driveTrain.SetSafetyEnabled(false);
		bool checkBox1 = SmartDashboard::GetBoolean("Checkbox 1");
		int rangeToWallClose = 60;
		int rangeToWallFar = 120;
		
		while(IsAutonomous() && IsEnabled()){
		if(checkBox1 == true){
			SmartDashboard::PutNumber("Autonomous mode", 1);
			
			//Drive until Robot is within range to wall.
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "range%f", frontUltrasonic.GetDistance()

			
					       );
			float rangeToWall = frontUltrasonic.GetDistance();
			while(rangeToWall > rangeToWallClose){
				driveTrain.Drive(-5, 0.0);
				rangeToWall = frontUltrasonic.GetDistance();
				Wait(.001);
			}
			driveTrain.Drive(0.0, 0.0); //Stop the Robot
		}
		if(checkBox1 == false){
			SmartDashboard::PutNumber("Autonomous mode", 2);
			float rangeToWall = frontUltrasonic.GetDistance();
			
			while(rangeToWall > rangeToWallFar){
				driveTrain.Drive(-5, 0.0);
			}
			driveTrain.Drive(0.0, 0.0);
		}}
		lcd->Clear();
		lcd->UpdateLCD();
		//driveDistance.Reset();
		//driveDistance.Start();
		//myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		//if(driveDistance.Get()==1){
		    //myRobot.Drive(-5, 0.0);
		    //Wait(2.0);
		    //myRobot.Drive(0.0, 0.0);
		//}
		//Wait(2.0); 				//    for 2 seconds
		//myRobot.Drive(0.0, 0.0); 	// stop robot
		lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Exiting Autonomous");
	}
	

	/**
	 * Runs the motors with arcade steering. 
	 */
	
	void OperatorControl()
	{
		//elevation.Reset();
		//elevation.Start();
		driveTrain.SetSafetyEnabled(true);
		//ballGrabber.desiredElevatorVoltage = 90;
		int printDelay = 0;
		int shootDelay = 0;
		while (IsOperatorControl() && IsEnabled())
		{
			printDelay ++;
			
			float rJoyStick = limitSpeed(rightJoyStick.GetY());
			float lJoyStick = limitSpeed(leftJoyStick.GetY());
			
			//speedLimiter.SetMaxOutput(SmartDashboard::GetNumber("Slider 1"));
			driveTrain.TankDrive(rJoyStick, lJoyStick);
		//organize lcd code limit to 2 times per second
			if(printDelay == 100){
				//float readings[100];
				//readings[loopCounter%100];
				//do average();
				lcd->PrintfLine(DriverStationLCD::kUser_Line1, "F%6.2f B%6.2f", frontUltrasonic.GetDistance(), backUltrasonic.GetDistance());
				ballGrabber.DisplayDebugInfo(DriverStationLCD::kUser_Line2,lcd);
				//lcd->PrintfLine(DriverStationLCD::kUser_Line3, "G%f", ballGrabber.ballDetector.GetDistance());
				ballGrabber.UpDateWithState(DriverStationLCD::kUser_Line3,lcd);
				//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "EV%6.2f", ballGrabber.elevatorAngleSensor.GetVoltage());
				shooter.DisplayDebugInfo(DriverStationLCD::kUser_Line4, lcd);
				//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%5.3f %5.3f %5.3f", lJoyStick, rJoyStick, SmartDashboard::GetNumber("Slider 1"));
				lcd->PrintfLine(DriverStationLCD::kUser_Line5, "DEV=%6.2fSP=%6.2f", ballGrabber.desiredElevatorVoltage, ballGrabber.elevatorController.GetSetpoint());
				lcd->PrintfLine(DriverStationLCD::kUser_Line6, "CEV=%6.2fEE=%6.2f",
						ballGrabber.elevatorAngleSensor.PIDGet(),
						ballGrabber.elevatorController.GetError());
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
			bool ReadyToShoot = (shootDelay>20);
			shooter.OperateShooter(ReadyToShoot,loadShooterButton);
			if (ReadyToShoot){
				shootDelay = 0;
			}
			ballGrabber.OperateGrabber(ReadyToShoot, shooterButton , &gamePad);
			//Trying to make some things happen automatically during teleoperated
			
			
			
			Wait(0.005);// wait for a motor update time
		
	
		}
		driveTrain.StopMotor();
		ballGrabber.elevatorController.Disable();
		
		
	}
		
	/**
	 * Runs during test mode
	 */
	void Test() {
		TestMode tester(m_ds);
		testEncoder.Reset();
		testEncoder.Start();
		while (IsTest() && IsEnabled()){
			lcd->Clear();
			tester.PerformTesting(&gamePad, &testEncoder, lcd, &rightJoyStick, &leftJoyStick,
								  &testSwitch, &testTalons, &frontUltrasonic, &backUltrasonic,
								  &ballGrabber.ballDetector, &analogTestSwitch,
								  &shooter
								  );
			lcd->UpdateLCD();
			Wait(0.2);
		}
		testEncoder.Stop();

	}
};

START_ROBOT_CLASS(RobotDemo);

