#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H
#include "WPILib.h"

class UltrasonicSensor : public AnalogChannel
{
public:
	UltrasonicSensor(int moduleNumber, int portNumber);
	float GetDistance();	
	float Range;
		
	~UltrasonicSensor();
private:
};
#endif
