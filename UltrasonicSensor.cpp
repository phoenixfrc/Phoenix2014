#include "UltrasonicSensor.h"
#include "Phoenix2014.h"
UltrasonicSensor::UltrasonicSensor(int moduleNumber, int portNumber):
AnalogChannel(moduleNumber, portNumber)
{
	
}

float UltrasonicSensor::GetDistance(){
	
	return GetVoltage()*100;
}

float UltrasonicSensor::GetAverageDistance(){
	
	return GetAverageVoltage()*100;
}
UltrasonicSensor::~UltrasonicSensor(){
	
}
