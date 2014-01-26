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
//talons needed for shooter
#define PHOENIX2014_SHOOTER_LOAD_PWM 7
//unused pwm ports.
#define PHOENIX2014_PWM_UNUSED8 8
#define PHOENIX2014_PWM_UNUSED9 9
#define PHOENIX2014_PWM_UNUSED10 10

// *******************************************************************
//spike needed for grabber
#define PHOENIX2014_GRABBER_ACTUATOR 1
//spikes needed for shooter
#define PHOENIX2014_SHOOTER_RELEASE 2
#define PHOENIX2014_AIR_COMPRESSOR 3
//Unused Spikes.
#define PHOENIX2014_RELAY_UNUSED4 4
#define PHOENIX2014_RELAY_UNUSED5 5
#define PHOENIX2014_RELAY_UNUSED6 6
#define PHOENIX2014_RELAY_UNUSED7 7
#define PHOENIX2014_RELAY_UNUSED8 8

// *******************************************************************
//Encoders 
#define PHOENIX2014_R_DRIVE_MOTOR_A 1
#define PHOENIX2014_R_DRIVE_MOTOR_B 2
#define PHOENIX2014_L_DRIVE_MOTOR_A 4
#define PHOENIX2014_L_DRIVE_MOTOR_B 5
#define PHOENIX2014_SHOOTER_ENCODER_A 6
#define PHOENIX2014_SHOOTER_ENCODER_B 7
#define PHOENIX2014_ELEVATOR_ENCODER_A 8
#define PHOENIX2014_ELEVATOR_ENCODER_B 9
#define PHOENIX2014_ULTRASONIC_ENCODER_A 10
#define PHOENIX2014_ULTRASONIC_ENCODER_B 11
//Unused Digital IO.
#define PHOENIX2014_IO_UNUSED12 12
//limit switch for shooter
#define PHOENIX2014_SHOOTER_LIMIT_SWITCH 13
//limit switch for elevator
#define PHOENIX2014_ELEVATOR_LIMIT_SWITCH 14
//encoders

#endif /* PHOENIX2014_H_ */
