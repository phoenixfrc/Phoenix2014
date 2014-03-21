#include "DeadZoneTalon.h"

DeadZoneTalon::DeadZoneTalon(uint32_t channel):
Talon(channel)
{
	m_deadZone = 0.1;
}
DeadZoneTalon::DeadZoneTalon(uint8_t moduleNumber, uint32_t channel):
Talon(moduleNumber, channel)
{
	m_deadZone = 0.1;
}

float DeadZoneTalon::adjustValue(float value){
	if(value <= m_deadZone && value >= 0.01){
		value = value + m_deadZone;
	}
	else if(value >= -m_deadZone && value <= -0.01){
		value = value - m_deadZone;
	}
	return value;
}
void DeadZoneTalon::Set(float value, bool posLS, bool negLS, uint8_t syncGroup){
	if(posLS && value > 0.0){
		value = 0.0;
	}
	if(negLS && value < 0.0){
		value = 0.0;
	}
	this->Set(value, syncGroup);
}

void DeadZoneTalon::Set(float value, uint8_t syncGroup){
	value = adjustValue(value);
	Talon::Set(value, syncGroup);
}
void DeadZoneTalon::PIDWrite(float output){
	output = adjustValue(output);
	Talon::PIDWrite(output);
}
DeadZoneTalon::~DeadZoneTalon(){
	
}
