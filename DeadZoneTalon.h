#ifndef DEADZONETALON_H
#define DEADZONETALON_H
#include "wpilib.h"

class DeadZoneTalon : public Talon
{
public:
	DeadZoneTalon(uint32_t channel);
	DeadZoneTalon(uint8_t moduleNumber, uint32_t channel);
	//Run motor but protect with limit switches.
	void Set(float value, bool posLS, bool negLS, uint8_t syncGroup=0);
	void Set(float value, uint8_t syncGroup=0);
	virtual void PIDWrite(float output);
	float adjustValue(float value);
	~DeadZoneTalon();
private:
	float m_deadZone;
};
#endif
