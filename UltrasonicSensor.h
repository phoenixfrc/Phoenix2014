#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

class UltrasonicSensor{
public:
	UltrasonicSensor(int moduleNumber, int portNumber);
	void UseUltrasonic();	
	//analogChannel(moduleNumber, portNumber){}
	//float Range(){
		//return analogChannel.GetVoltage()*100;
	~UltrasonicSensor();
private:
	AnalogChannel m_analogChannel;
};
#endif
