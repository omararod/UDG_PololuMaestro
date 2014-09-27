#ifndef UDG_POLOLUMAESTRO_H
#define UDG_POLOLUMAESTRO_H

#define DEFAULT_CHANNELS 18


#ifdef __linux
#include <unistd.h>

typedef int HANDLE;
#else
#include <Windows.h>
#endif

class ServoMotor
{
	public:
			
			int minPulse;
			int maxPulse;
			int neutralPulse;
			int degreeRange;

			int AngleToPulse(float angle);
			void SetParameters(int _minPulse, int _maxPulse, int _neutralPulse, int _degreeRange);

};


class PololuMaestro
{
	public:
	
	
	PololuMaestro(HANDLE fd);
	PololuMaestro(HANDLE fd,int nchannels);
	~PololuMaestro();
	bool SetAngle(int channel, float angle);
	float GetAngle(int channel);
	bool SetTarget(int channel,unsigned short targetMs);
	bool SetSpeed(int channel,unsigned short speed);
	bool SetAcceleration(int channel,unsigned short acceleration);
	unsigned short GetPosition(int channel);
	bool AssignChannel(int channel, ServoMotor motor);
	bool IsMoving();
	

	private:
	HANDLE deviceDescriptor;
	ServoMotor* channels;

	int numberOfChannels;
	bool WriteToDevice(unsigned char* buffer, int nBytes);
	bool ReadFromDevice(unsigned char* buffer, int nBytes);
	
};


#endif