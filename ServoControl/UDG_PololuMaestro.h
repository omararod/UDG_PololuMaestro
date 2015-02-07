#ifndef UDG_POLOLUMAESTRO_H
#define UDG_POLOLUMAESTRO_H

#define DEFAULT_CHANNELS 18

/*error code definitions*/
#define ERROR_MAESTRO_SERIAL_SIGNAL 0x0001
#define ERROR_MAESTRO_SERIAL_OVERRUN 0x0002
#define ERROR_MAESTRO_SERIAL_RX_BUFFER_FULL 0x0004
#define ERROR_MAESTRO_SERIAL_CRC_ERROR 0x0008
#define ERROR_MAESTRO_SERIAL_PROTOCOL 0x0010
#define ERROR_MAESTRO_SERIAL_TIMEOUT 0x0020
#define ERROR_MAESTRO_SCRIPT_STACK 0x0040
#define ERROR_MAESTRO_SCRIPT_CALL_STACK 0x0080
#define ERROR_MAESTRO_SCRIPT_PROGRAM_COUNTER 0x0100





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
	short GetErrors();
	bool GoHome();
	

	private:
	HANDLE deviceDescriptor;
	ServoMotor* channels;

	int numberOfChannels;
	bool WriteToDevice(unsigned char* buffer, int nBytes);
	bool ReadFromDevice(unsigned char* buffer, int nBytes);
	
};


#endif