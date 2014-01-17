#include "WPILib.h"
#include "Shooter.h"
#include "TestMode.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick rightStick; // rightStick wired to port 1
	Joystick leftStick;  // leftStick wired to port 2
	Joystick gamePad;
	//Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistance;
	Encoder testEncoder;
	Talon elevatorMotor;
	DriverStationLCD * lcd;

public:
	// For the RobotDemo() constructor list the component constructors (myrobot, rightstick etc) in the order declared above.
	RobotDemo()://This is the constructer function
		//myRobot(1, 2, 3, 4),	// lr, lf, rr, rf pwm channels,
		myRobot(1,3), // rearleftmotor (pwm channel), rearrightmotor (pwm channel)
		rightStick(2),// as they are declared above.
		leftStick(1),
		gamePad(3),
		//elevation(1,2),
		driveDistance(3,4),
		testEncoder(1,2),
		elevatorMotor(5),
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
		driveDistance.Reset();
		driveDistance.Start();
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		//elevation.Reset();
		//elevation.Start();

		Shooter shooter;
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.TankDrive(rightStick, leftStick);
			//int rotation = elevation.Get();
			//the above is commented because we are not using it yet
			Wait(0.005);// wait for a motor update time
		}
		myRobot.StopMotor();
		
		shooter.operateShooter(); 
	}
		
	/**
	 * Runs during test mode
	 */
	void Test() {
		TestMode tester;
		testEncoder.Reset();
		testEncoder.Start();
		while (IsTest()){
			tester.PerformTesting(&gamePad, &testEncoder, lcd);
			lcd->UpdateLCD();
			Wait(0.1);
		}
		testEncoder.Stop();

	}
};

START_ROBOT_CLASS(RobotDemo);

