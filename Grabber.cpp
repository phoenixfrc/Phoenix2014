#include "Grabber.h"
#include "TestMode.h"

Grabber::Grabber(Joystick * gamePad) :
	grabberActuator(PHOENIX2014_GRABBER_CLAW_MOTOR_PWM),
	//grabberElevator(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	grabberCloseLimit(PHOENIX2014_GRABBER_CLOSE_LIMIT_SWITCH),
	grabberOpenLimit(PHOENIX2014_GRABBER_OPEN_LIMIT_SWITCH),
	//forward is battery side
	forwardLimitSwitch(PHOENIX2014_ELEVATOR_FORWARD_LIMIT_SWITCH),
	backLimitSwitch(PHOENIX2014_ELEVATOR_BACK_LIMIT_SWITCH),
	elevatorMotor(PHOENIX2014_GRABBER_ELEVATOR_MOTOR_PWM),
	//the angle is 0 when shooter is backward (transmition side)
	elevatorAngleSensor(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_ELEVATOR_ANGLE),
	elevatorController(0.1*2000.0, 0.001*10.0, 0.0, &elevatorAngleSensor, &elevatorMotor),
	ballDetector(PHOENIX2014_ANALOG_MODULE_NUMBER, PHOENIX2014_ANALOG_GRABBER_BALL_ULTRASONIC_SENSOR)
	//lcd(DriverStationLCD::GetInstance())

{
	//initialize the grabber to trip the closed grabber switch
	m_grabberState = unknown;
	m_grabberPower = 1.0;
	m_elevatorPower = 2.0;
	//initialize elevator PiD loop
	elevatorController.SetContinuous(false);
	elevatorAngleSensor.SetVoltageForPID(true);
	elevatorController.SetOutputRange(-m_elevatorPower, m_elevatorPower);//motor run from -1 to 1
	elevatorController.SetInputRange(PHOENIX2014_VOLTAGE_AT_FRONT, PHOENIX2014_VOLTAGE_AT_BACK);
	elevatorController.SetAbsoluteTolerance(2.5);
	distanceToClose = 12;
	detectBall = true;
	m_stateString = "";
	m_gamePad = gamePad;
	multiplier = 0.0;
	boost = 0.0;
	m_motorOnTimeCount = 0;
}

void Grabber::init(){
	m_grabberState = unknown;
	this->resetSetPoint();
	multiplier = SmartDashboard::GetNumber("I");
	boost = SmartDashboard::GetNumber("D");
}

//Caller needs to enable elevator controller.
void Grabber::resetSetPoint(){
	m_desiredElevatorVoltage = elevatorAngleSensor.GetVoltage();
	//elevatorController.Reset();
	//elevatorController.SetSetpoint(m_desiredElevatorVoltage);
}


void Grabber::OperateGrabber(bool openToShoot, bool useBallSensor){
	//One button will toggle between open and closed grabber
	bool grabberButton = m_gamePad->GetRawButton(1);
	//float moveGrabberUpButton = gamePad->GetRawButton(2);
	//float moveGrabberDownButton = gamePad->GetRawButton(4);
	//bool ballPresent =  ballSensor.Get();
	bool reachedLimitForClosed = !grabberCloseLimit.Get();
	bool reachedLimitForOpen = !grabberOpenLimit.Get();
	//Ball detector.
	if(ballDetector.GetDistance() < distanceToClose){
		detectBall = true;
	}
	else{
		detectBall = false;
	}
	if(openToShoot){
			m_grabberState = opening;
		}
	//int currentElevatorAngle =(int) (elevatorAngleSensor.GetVoltage()*PHOENIX2014_POT_DEGREES_PER_VOLT);
	//This will 
	bool motorOnTooLong = (m_motorOnTimeCount > PHOENIX2014_MAX_GRAB_MOTOR_ON_CYCLES);
	switch(m_grabberState){
		case closing:
			m_stateString = "GS=closing";
			grabberActuator.Set(m_grabberPower*-1);
			m_motorOnTimeCount ++;
			if(reachedLimitForClosed || motorOnTooLong){
				m_grabberState = closed;
			}
			break;
		case closed:
			
			m_stateString = "GS=closed";
			grabberActuator.Set(0.0);
			m_motorOnTimeCount = 0;
			if(grabberButton){
				m_grabberState = opening;
			}
			break;
		case opening:
			m_stateString = "GS=opening";
			grabberActuator.Set(m_grabberPower);
			m_motorOnTimeCount ++;
			if(reachedLimitForOpen || motorOnTooLong){
				m_grabberState = open;
			}
			break;
		case open:
			m_stateString = "GS=open";
			grabberActuator.Set(0.0);//if button is pressed or ball is detected.
			m_motorOnTimeCount = 0;
			if (grabberButton /*|| (detectBall && useBallSensor)*/){
				m_grabberState = closing;
			}
			break;
		case unknown://unknown is the same as default
		default:
			if(reachedLimitForClosed){
				m_grabberState = closed;
			}
			else if(reachedLimitForOpen){
				m_grabberState = open;
			}
			else{
				m_grabberState = opening;
			}
			break;
	}
	//this->ButtonControledElevator();
	this->ThumbstickControledElevator(); //update desired position based on thumbstick and limit switches.
	this->DriveMotorWithPIDLoop();
	
}
//This function changes the setpoint of the PID loop via the A and Y buttons
float Grabber::ButtonControledElevator(){
	//PID Loop for the grabber elevator which controlls the elevator arm
	bool elevatorForwardRequest = m_gamePad->GetRawButton(4);
	bool elevatorBackwardRequest = m_gamePad->GetRawButton(2);
	bool forwardLimit = !forwardLimitSwitch.Get();
	bool backLimit = !backLimitSwitch.Get();
	float voltageIncrement = 0.001;

	if(elevatorForwardRequest && !elevatorBackwardRequest && !forwardLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage + voltageIncrement;
				elevatorMotor.Set(m_elevatorPower);
	}
	
	else if(elevatorBackwardRequest && !elevatorForwardRequest && !backLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage - voltageIncrement;
				elevatorMotor.Set(m_elevatorPower*-1);
		
		}
		
	return this->ElevatorLimitSwitchBehavior();
	
}


//This function changes the set point of the PID loop via the thumbstick
float Grabber::ThumbstickControledElevator(){
	float dPadThumbstick = TestMode::GetThumbstickWithZero(m_gamePad);
	
	if (dPadThumbstick != 0.0){
		float currentVoltage = elevatorAngleSensor.GetVoltage();
		m_desiredElevatorVoltage = currentVoltage + dPadThumbstick/5.0;
	}
	
	if(m_desiredElevatorVoltage > PHOENIX2014_VOLTAGE_AT_BACK){
		m_desiredElevatorVoltage = PHOENIX2014_VOLTAGE_AT_BACK;
	}
	if(m_desiredElevatorVoltage < PHOENIX2014_VOLTAGE_AT_FRONT){
		m_desiredElevatorVoltage = PHOENIX2014_VOLTAGE_AT_FRONT;
	}
	return this->ElevatorLimitSwitchBehavior();	
}
//this function will move the elevator to an angle based on the elevator voltage
void Grabber::RunElevatorAutonomous(float autoDesiredElevatorVoltage){
	m_desiredElevatorVoltage = autoDesiredElevatorVoltage;
	this->DriveMotorWithPIDLoop();
}

//This function makes sure that the arm does not go passed the limit switches
float Grabber::ElevatorLimitSwitchBehavior(){
	bool forwardLimit = !forwardLimitSwitch.Get();
	bool backLimit = !backLimitSwitch.Get();
	float voltageIncrement = 0.005;
	
	if(backLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage + voltageIncrement;
	}
	if(forwardLimit){
		m_desiredElevatorVoltage = m_desiredElevatorVoltage - voltageIncrement;
	}
	return m_desiredElevatorVoltage;
}




void Grabber::UpDateWithState(DriverStationLCD::Line line, DriverStationLCD * lcd){
	lcd->PrintfLine(line, m_stateString);
}
void Grabber::DisplayDebugInfo(DriverStationLCD::Line line, DriverStationLCD * lcd){
	bool reachedLimitForClosed = !grabberCloseLimit.Get();
	bool reachedLimitForOpen = !grabberOpenLimit.Get();
	bool bottomLimit = !forwardLimitSwitch.Get();
	bool topLimit = !backLimitSwitch.Get();

	lcd->PrintfLine(line, "M%8.5f gb=%c%c ev=%c%c", //prints the button values to LCD display
			            elevatorMotor.Get(),
						reachedLimitForOpen ? 'O':'o',
						reachedLimitForClosed ? 'C':'c',
						bottomLimit ? 'F':'f',
						topLimit ? 'B':'b'
						);
}
void Grabber::DriveElevatorTestMode(float value){
	bool bottomLimit = !forwardLimitSwitch.Get();
	bool topLimit = !backLimitSwitch.Get();

	/*if (bottomLimit){
		if(value<0){
			value = 0.0;
		}
	}
	else if (topLimit){
		if(value>0){
					value = 0.0;
				}
	}*/
	
	elevatorMotor.Set(value, topLimit, bottomLimit);
}
//Custom Pid will figure out the error between the current value and the desired value and set the motor accordingly.
float Grabber::OperatePIDLoop(){
	//Make constant for 5.0 and call it max voltage or somthing like that.
	float pidError = (m_desiredElevatorVoltage - elevatorAngleSensor.GetVoltage()) * multiplier;  //Changed multiplier to dashboard constant
	if((-PHOENIX2014_PID_THRESHOLD < pidError) && (pidError < PHOENIX2014_PID_THRESHOLD)){
		m_elevatorPower = 0.0;
	}
	else{
		// Ensure the motor power is between .15 and 1
		double motorPower = pidError + (pidError > 0 ? boost : -boost);  //Changed boost to dashboard constant
		if (motorPower < -1.0) {
			motorPower = -1.0;
		} else if (motorPower > 1.0) {
			motorPower = 1.0;
		}
		// Ensure we do not power the motors in the direction of a hit limit switch
		// Also, do not have the target farther in the direction of a set limit switch
		bool forwardLimit = !forwardLimitSwitch.Get();
		bool backLimit = !backLimitSwitch.Get();
		if(backLimit && motorPower > 0){
			motorPower = 0.0;
			m_desiredElevatorVoltage = elevatorAngleSensor.GetVoltage();
		}
		if(forwardLimit && motorPower < 0){
			motorPower = 0.0;
			m_desiredElevatorVoltage = elevatorAngleSensor.GetVoltage();
		}
		m_elevatorPower = motorPower;
	}
	return pidError;
}
void Grabber::DriveMotorWithPIDLoop(){
	this->OperatePIDLoop();
	elevatorMotor.Set(m_elevatorPower);
}
void Grabber::StopPidLoop(){
		elevatorMotor.Set(0.0);
		m_desiredElevatorVoltage = elevatorAngleSensor.GetVoltage();
		this->ElevatorLimitSwitchBehavior();

}


Grabber::~Grabber(){
	
}
