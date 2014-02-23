#include "deadZoneTalon.h"

DeadZoneTalon::DeadZoneTalon(uint32_t channel):
Talon(channel)
{
	
}
DeadZoneTalon::DeadZoneTalon(uint8_t moduleNumber, uint32_t channel):
Talon(moduleNumber, channel)
{
		
	}
DeadZoneTalon::~DeadZoneTalon(){
	
}
