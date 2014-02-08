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
	RobotDrive myRobot; // robot drive system
	Grabber ballGrabber;
	Joystick rightJoyStick; // rightStick wired to port 1
	Joystick leftJoyStick;  // leftStick wired to port 2
	Joystick gamePad;
	//Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistance;
	Encoder testEncoder;
	Talon elevatorMotor;
	DigitalInput testSwitch;
	Talon testTalons;
	UltrasonicSensor frontUltrasonic;
	UltrasonicSensor backUltrasonic;
	UltrasonicSensor grabberUltrasonic;
	AnalogTrigger analogTestSwitch;
	//RobotDrive speedLimiter;
	DriverStationLCD * lcd;
	
	
public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		//myRobot(1, 2, 3, 4),	// lr, lf, rr, rf pwm channels,
		myRobot(PHOENIX2014_DRIVEMOTOR_LEFT_REAR,PHOENIX2014_DRIVEMOTOR_RIGHT_REAR), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		ballGrabber(), //Place holder for grabber.
		rightJoyStick(2),// as they are declared above.
		leftJoyStick(1),
		gamePad(3),
		//elevation(1,2),
		driveDistance(3,4),
		testEncoder(1,2),
		elevatorMotor(5),
		testSwitch(3),
		testTalons(2),
		frontUltrasonic(2, PHOENIX2014_ANALOG_ULTRASONIC_FRONT),
		backUltrasonic(2, 3),
		grabberUltrasonic(2, 4),
		analogTestSwitch(2, 5),
		//speedLimiter(1, 2),
	    lcd(DriverStationLCD::GetInstance())
	{
		myRobot.SetExpiration(0.1);
		myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}
	
	float limitSpeed(float requestedSpeed)
	{
		float scaleSpeed = SmartDashboard::GetNumber("Slider 1");
		return requestedSpeed * 0.01 * scaleSpeed;
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
	        lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Entered Autonomous");
		myRobot.SetSafetyEnabled(false);
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
				myRobot.Drive(-5, 0.0);
				rangeToWall = frontUltrasonic.GetDistance();
				Wait(.001);
			}
			myRobot.Drive(0.0, 0.0); //Stop the Robot
		}
		if(checkBox1 == false){
			SmartDashboard::PutNumber("Autonomous mode", 2);
			float rangeToWall = frontUltrasonic.GetDistance();
			
			while(rangeToWall > rangeToWallFar){
				myRobot.Drive(-5, 0.0);
			}
			myRobot.Drive(0.0, 0.0);
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
		lcd->PrintfLine(DriverStationLCD::kUser_Line2, "Exeting Autonomous");
	}
	

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		//elevation.Reset();
		//elevation.Start();
		
		ballGrabber.elevatorController.Enable();
		Shooter Shooter;
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			lcd->UpdateLCD();
			float rJoyStick = limitSpeed(rightJoyStick.GetY());
			float lJoyStick = limitSpeed(leftJoyStick.GetY());
			lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%f %f %f", lJoyStick, rJoyStick, SmartDashboard::GetNumber("Slider 1"));
			//speedLimiter.SetMaxOutput(SmartDashboard::GetNumber("Slider 1"));
			myRobot.TankDrive(rJoyStick, lJoyStick);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "%f", frontUltrasonic.GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", backUltrasonic.GetDistance());
			lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", grabberUltrasonic.GetDistance());
			
			//int rotation = elevation.Get();
			//the above is commented because we are not using it yet
			Shooter.OperateShooter(&gamePad); 
			ballGrabber.OperateGrabber(&gamePad);
			//Trying to make some things happen automatically during teleoperated
		 
			
			
			Wait(0.005);// wait for a motor update time
		
	
		}
		myRobot.StopMotor();
		
		
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
			tester.PerformTesting(&gamePad, &testEncoder, lcd, &rightJoyStick, &leftJoyStick, &testSwitch, &testTalons, &frontUltrasonic, &backUltrasonic, &grabberUltrasonic, &analogTestSwitch);
			lcd->UpdateLCD();
			Wait(0.2);
		}
		testEncoder.Stop();

	}
};

START_ROBOT_CLASS(RobotDemo);

