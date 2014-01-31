#include "WPILib.h"
#include "Shooter.h"
#include "Grabber.h"
#include "TestMode.h"
#include "Phoenix2014.h"


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
	Joystick rightStick; // rightStick wired to port 1
	Joystick leftStick;  // leftStick wired to port 2
	Joystick gamePad;
	//Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistance;
	Encoder testEncoder;
	Talon elevatorMotor;
	DigitalInput testSwitch;
	Talon testTalons;
	Ultrasonic ultrasonicRangeFinder;
	DriverStationLCD * lcd;

	
public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		//myRobot(1, 2, 3, 4),	// lr, lf, rr, rf pwm channels,
		myRobot(PHOENIX2014_DRIVEMOTOR_LEFT_REAR,PHOENIX2014_DRIVEMOTOR_RIGHT_REAR), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		ballGrabber(), //Place holder for grabber.
		rightStick(2),// as they are declared above.
		leftStick(1),
		gamePad(3),
		//elevation(1,2),
		driveDistance(3,4),
		testEncoder(1,2),
		elevatorMotor(5),
		testSwitch(3),
		testTalons(2),
		ultrasonicRangeFinder(PHOENIX2014_ANALOG_ULTRASONIC_OUTPUT, PHOENIX2014_ANALOG_ULTRASONIC_INPUT),
	    lcd(DriverStationLCD::GetInstance())
	{
		myRobot.SetExpiration(0.1);
		myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}
	

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		myRobot.SetSafetyEnabled(false);
		bool checkBox1 = SmartDashboard::GetBoolean("Checkbox 1");
		int rangeToWall = 60;
		
		if(checkBox1 == true){
			SmartDashboard::PutNumber("Autonomous mode", 1);
			
			//Drive until Robot is within range to wall.
			while(ultrasonicRangeFinder.GetRangeInches() > rangeToWall){
				myRobot.Drive(-5, 0.0);
			}
			myRobot.Drive(0.0, 0.0); //Stop the Robot
		}
		if(checkBox1 == false){
			SmartDashboard::PutNumber("Autonomous mode", 2);
		}
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
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		//elevation.Reset();
		//elevation.Start();

		Shooter Shooter;
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			ultrasonicRangeFinder.Ping();
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "%f", ultrasonicRangeFinder.GetRangeInches());
			myRobot.TankDrive(rightStick, leftStick);
			
			//int rotation = elevation.Get();
			//the above is commented because we are not using it yet
			Shooter.OperateShooter(&gamePad); 
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
		while (IsTest()){
			tester.PerformTesting(&gamePad, &testEncoder, lcd, &rightStick, &leftStick, &testSwitch, &testTalons);
			lcd->UpdateLCD();
			Wait(0.1);
		}
		testEncoder.Stop();

	}
};

START_ROBOT_CLASS(RobotDemo);

