#include "WPILib.h"

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
	Encoder elevation;//we will use digital I/O port numbers 1 and 2
	Encoder driveDistance;
	Talon elevatorMotor;

public:
	RobotDemo()://This is the constructer function
		//myRobot(1, 2,3, 4),	// these must be initialized in the same order
		myRobot(1,3),
		rightStick(2),// as they are declared above.
		leftStick(1),
		gamePad(3),
		elevation(1,2),
		driveDistance(3,4),
		elevatorMotor(5)
	{
		myRobot.SetExpiration(0.1);
		//myRobot.SetInvertedMotor();
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
		elevation.Reset();
		elevation.Start();
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.TankDrive(rightStick, leftStick);
			//int rotation = elevation.Get();
			//the above is commented because we are not using it yet
			Wait(0.005);// wait for a motor update time
		}
		myRobot.StopMotor();
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

