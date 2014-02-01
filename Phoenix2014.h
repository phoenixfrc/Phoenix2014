/*
 * Phoenix2014.h
 *
 *  Created on: Jan 17, 2014
 *      Author: Ryan
 */

#ifndef PHOENIX2014_H_
#define PHOENIX2014_H_

//define talon constant here

//using 1,2,3,4 for drivetrain
#define PHOENIX2014_DRIVEMOTOR_LEFT_REAR 1
#define PHOENIX2014_DRIVEMOTOR_LEFT_FRONT 2
#define PHOENIX2014_DRIVEMOTOR_RIGHT_REAR 3
#define PHOENIX2014_DRIVEMOTOR_RIGHT_FRONT 4
//talons needed for grabber
#define PHOENIX2014_GRABBER_ELEVATOR_PWM 5
#define PHOENIX2014_GRABBER_ACTUATOR_PWM 6
//unused pwm ports.
#define PHOENIX2014_PWM_UNUSED7 7
#define PHOENIX2014_PWM_UNUSED8 8
#define PHOENIX2014_PWM_UNUSED9 9
#define PHOENIX2014_PWM_UNUSED10 10

// *******************************************************************
#define PHOENIX2014_UNUSED_1 1
//spikes needed for shooter
#define PHOENIX2014_SHOOTER_RELEASE 2
#define PHOENIX2014_SHOOTER_LOAD 3
//Spikes used for elevator
#define PHOENIX2014_RELAY_ELEVATOR 4
//Unused Spikes.
#define PHOENIX2014_RELAY_UNUSED5 5
#define PHOENIX2014_RELAY_UNUSED6 6
#define PHOENIX2014_RELAY_UNUSED7 7
#define PHOENIX2014_RELAY_UNUSED8 8

// *******************************************************************
//Digital I/O
//Encoders 
#define PHOENIX2014_R_DRIVE_MOTOR_A 1
#define PHOENIX2014_R_DRIVE_MOTOR_B 2
#define PHOENIX2014_L_DRIVE_MOTOR_A 3
#define PHOENIX2014_L_DRIVE_MOTOR_B 4
#define PHOENIX2014_SHOOTER_ENCODER_A 5
#define PHOENIX2014_SHOOTER_ENCODER_B 6
#define PHOENIX2014_ELEVATOR_ENCODER_A 7
#define PHOENIX2014_ELEVATOR_ENCODER_B 8
#define PHOENIX2014_UNUSED_9 9
#define PHOENIX2014_UNUSED_10 10
#define PHOENIX2014_UNUSED_11 11
//limit Switch for Grabber
#define PHOENIX2014_GRABBER_BALL_SENSOR 12
#define PHOENIX2014_UNUSED_13 13
#define PHOENIX2014_UNUSED_14 14
// *******************************************************************
//Analog IO
//Grabber limit switches
#define PHOENIX2014_ANALOG_GRABBER_CLOSE_LIMIT_SWITCH 1
#define PHOENIX2014_ANALOG_GRABBER_OPEN_LIMIT_SWITCH 5 
//limit switch for shooter
#define PHOENIX2014_ANALOG_SHOOTER_LIMIT_SWITCH 3
//limit switch for elevator
#define PHOENIX2014_ANALOG_ELEVATOR_TOP_LIMIT_SWITCH 4
#define PHOENIX2014_ANALOG_ULTRASONIC 2
#define PHOENIX2014_ANALOG_UNUSED_6 6
#define PHOENIX2014_ANALOG_BOTTOM_LIMIT_SWITCH 7
#define PHOENIX2014_ANALOG_UNUSED_8 8

#endif /* PHOENIX2014_H_ */
