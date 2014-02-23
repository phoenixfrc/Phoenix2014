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
void DeadZoneTalon::Set(float value, uint8_t syncGroup){
	if(value <= m_deadZone && value >= 0.01){
		value = value + m_deadZone;
	}
	else if(value >= -m_deadZone && value <= -0.01){
		value = value - m_deadZone;
	}
	
	Talon::Set(value, syncGroup);
}
DeadZoneTalon::~DeadZoneTalon(){
	
}
