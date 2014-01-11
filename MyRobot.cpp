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
	Joystick rightStick;
	Joystick leftStick;

public:
	RobotDemo()://This is the constructer function
		myRobot(1, 2, 3, 4),	// these must be initialized in the same order
		rightStick(1),// as they are declared above.
		leftStick(2)
	{
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.TankDrive(leftStick, rightStick);
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

