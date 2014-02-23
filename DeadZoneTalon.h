#ifndef DEADZONETALON_H
#define DEADZONETALON_H
#include "wpilib.h"

class DeadZoneTalon : public Talon
{
public:
	DeadZoneTalon(uint32_t channel);
	DeadZoneTalon(uint8_t moduleNumber, uint32_t channel);
	
	~DeadZoneTalon();
private:
};
#endif
