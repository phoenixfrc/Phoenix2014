/*
 * Phoenix2014.h
 *
 *  Created on: Jan 17, 2014
 *      Author: Ryan
 */
#include "Team.h"

#ifndef PHOENIX2014_H
#define PHOENIX2014_H

//define talon constant here

//using 1,2,3,4 for drivetrain
#define PHOENIX2014_DRIVEMOTOR_LEFT_REAR 1
#define PHOENIX2014_DRIVEMOTOR_LEFT_FRONT 2
#define PHOENIX2014_DRIVEMOTOR_RIGHT_REAR 3
#define PHOENIX2014_DRIVEMOTOR_RIGHT_FRONT 4
//talons needed for grabber
#define PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM 5
#define PHOENIX2014_GRABBER_CLAW_MOTOR_PWM 6
//Talons for shooter
#define PHOENIX2014_SHOOTER_LOAD_MOTOR 7
//unused pwm ports.
#define PHOENIX2014_CAMERA_SERVO_VERTICLE_PWM 8
#define PHOENIX2014_CAMERA_SERVO_HORIZONTAL_PWM 9
#define PHOENIX2014_PWM_UNUSED10 10
#define PHOENIX2014_PWM_UNUSED11 11

// *******************************************************************
//spikes needed for shooter
#define PHOENIX2014_SHOOTER_BRAKE_MOTOR_SPIKE 1
//Unused Spikes.
#define PHOENIX2014_RELAY_UNUSED2 2
#define PHOENIX2014_RELAY_UNUSED3 3
#define PHOENIX2014_RELAY_UNUSED4 4
#define PHOENIX2014_RELAY_UNUSED5 5
#define PHOENIX2014_RELAY_UNUSED6 6
#define PHOENIX2014_RELAY_UNUSED7 7
#define PHOENIX2014_RELAY_UNUSED8 8

// *******************************************************************
//Digital I/O
//Encoders 
#define PHOENIX2014_R_DRIVE_ENCODER_A 1
#define PHOENIX2014_R_DRIVE_ENCODER_B 2
#define PHOENIX2014_L_DRIVE_ENCODER_A 3
#define PHOENIX2014_L_DRIVE_ENCODER_B 4
#define PHOENIX2014_SHOOTER_ENCODER_A 5
#define PHOENIX2014_SHOOTER_ENCODER_B 6
//limit Switches
#define PHOENIX2014_ELEVATOR_TOP_LIMIT_SWITCH 7
#define PHOENIX2014_ELEVATOR_BOTTOM_LIMIT_SWITCH 8
#define PHOENIX2014_BRAKE_ENGAGED_LIMIT_SWITCH 9
#define PHOENIX2014_BRAKE_DISENGAGED_LIMIT_SWITCH 10
#define PHOENIX2014_GRABBER_CLOSE_LIMIT_SWITCH 11
#define PHOENIX2014_GRABBER_OPEN_LIMIT_SWITCH  12
#define PHOENIX2014_SHOOTER_LIMIT_SWITCH 13
#define PHOENIX2014_UNWINED_LIMIT_SWITCH 14
// *******************************************************************
//Analog IO
#define PHOENIX2014_ANALOG_ULTRASONIC_FRONT 1
#define PHOENIX2014_ANALOG_ULTRASONIC_BACK 2
#define PHOENIX2014_ANALOG_GRABBER_BALL_ULTRASONIC_SENSOR 3
#define PHOENIX2014_ANALOG_ELEVATOR_ANGLE 4
//Unused analog ports.
#define PHOENIX2014_ANALOG_UNUSED_5 5
#define PHOENIX2014_ANALOG_UNUSED_6 6
#define PHOENIX2014_ANALOG_UNUSED_7 7
#define PHOENIX2014_ANALOG_UNUSED_8 8
// *********************************************************************
// encoder calibrations
//drivetrain
//winch
//elevator
//ultrasonics
//***********************************************************************

//Test board module 1 is broken using module 2
//Constants for elevator
#define PHOENIX2014_POT_DEGREES_PER_VOLT 60.0
#define PHOENIX2014_UNITS_PER_DEGREE 3.2
#define PHOENIX2014_VOLTAGE_AT_VERTICAL 2.5
#define PHOENIX2014_VOLTAGE_AT_FRONT 1.0
#define PHOENIX2014_VOLTAGE_AT_BACK 4.0

#if TEAM_NUMBER == 2342
#define PHOENIX2014_ANALOG_MODULE_NUMBER 1
#else 
#define PHOENIX2014_ANALOG_MODULE_NUMBER 2
#endif	//TEAM NUMBER
#endif /* PHOENIX2014_H_ */
