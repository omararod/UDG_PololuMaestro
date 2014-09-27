#include "UDG_PololuMaestro.h"
#include <stdio.h>

#ifdef __linux
#pragma GCC diagnostic  ignored "-Wnarrowing"
#endif
PololuMaestro::PololuMaestro(HANDLE fd)
{
	channels = new ServoMotor[DEFAULT_CHANNELS];
	deviceDescriptor = fd;
	numberOfChannels = DEFAULT_CHANNELS;
}
PololuMaestro::~PololuMaestro()
{
	delete(channels);
#ifdef __linux
	close(deviceDescriptor);
#else
	CloseHandle(deviceDescriptor);
#endif
}
PololuMaestro::PololuMaestro(HANDLE fd,int nchannels)
{
	//No channel number validation is performed
	channels = new ServoMotor[nchannels];
	deviceDescriptor = fd;
	numberOfChannels = nchannels;

}
bool PololuMaestro::AssignChannel(int channel, ServoMotor motor)
{
	if(channel >= 0 && channel < numberOfChannels)
	{
		channels[channel] = motor;
		return true;
	}

	else
	{
		return false;
	}
}
bool PololuMaestro::SetAngle(int channel, float angle)
{
	unsigned short target = channels[channel].AngleToPulse(angle);
	return SetTarget(channel,target);
	
}

float PololuMaestro::GetAngle(int channel)
{
	unsigned short position = GetPosition(channel);

	return  ((((float)position -(float)channels[channel].minPulse)/((float)(channels[channel].maxPulse-(float)channels[channel].minPulse)))*(float)(channels[channel].degreeRange*2)) - (float)channels[channel].degreeRange;
}
/*channel: the number of the channel which will be set
targetUs: the desired pulse in micro seconds
returns false in case of error
*/
bool PololuMaestro::SetTarget(int channel,unsigned short targetUs)
{
	unsigned short target = targetUs;
	if(target > channels[channel].maxPulse)
	target = channels[channel].maxPulse;
	if(target < channels[channel].minPulse)
	target = channels[channel].minPulse;
	//the serial command receives the target in quarters of millisecond
	
	target*=4;
	// [0x84, channel number, target low bits, target high bits]
	unsigned char serialCommand[] = {0x84,channel,(target & 0x7F),(target >> 7) & 0x7F};
	return WriteToDevice(serialCommand,sizeof(serialCommand));
	
}
/*Sets the speed of 'channel' to 'speed' in units  of (0.25 μs)/(10 ms)*/
bool PololuMaestro::SetSpeed(int channel,unsigned short speed)
{
	//[0x87, channel number, speed low bits, speed high bits]	
	unsigned char serialCommand[] = {0x87,channel,(speed & 0x7F),(speed >> 7) & 0x7F};
	return WriteToDevice(serialCommand,sizeof(serialCommand));
}
/*Sets the acceleration of 'channel' to 'acceleration' in units  of (0.25 μs)/(10 ms)/(80 ms)*/
bool PololuMaestro::SetAcceleration(int channel,unsigned short acceleration)
{
	//[0x89, channel number, acceleration low bits, acceleration high bits]	
	unsigned char serialCommand[] = {0x89,channel,(acceleration & 0x7F),(acceleration >> 7) & 0x7F};
	return WriteToDevice(serialCommand,sizeof(serialCommand));
}
/*Gets the position of 'channel'*/
unsigned short PololuMaestro::GetPosition(int channel)
{
	//[0x90, channel number]	
	unsigned char serialCommand[] = {0x90,channel};
	if(! WriteToDevice(serialCommand,sizeof(serialCommand)))
	{
	}
	unsigned char response[2];
	if(!ReadFromDevice(response,2))
	{
	}
	return (response[0] + (response[1]*256))/4;
	
}

/*Gets the position of 'channel'*/
bool PololuMaestro::IsMoving()
{
	//[0x90, channel number]	
	unsigned char serialCommand[] = {0x93};
	if(! WriteToDevice(serialCommand,sizeof(serialCommand)))
	{
	}
	unsigned char response[1];
	if(!ReadFromDevice(response,1))
	{
	}
	return (bool)response[0];
	
}
/*Write nBytes of buffer to serial device*/
bool PololuMaestro::WriteToDevice(unsigned char* buffer, int nBytes)
{
#ifdef __linux
	if(write(deviceDescriptor, buffer, nBytes)==-1)
	{return false;}

	return true;
#else
	return WriteFile(deviceDescriptor,buffer,nBytes,NULL,NULL);
#endif
}
/*Read nBytes to buffer from serial device*/
bool PololuMaestro::ReadFromDevice(unsigned char* buffer, int nBytes)
{
#ifdef __linux
	if(read(deviceDescriptor, buffer, nBytes)!=nBytes)
	{return false;}
	return true;
#else
	if(ReadFile(deviceDescriptor, buffer, nBytes,NULL,NULL))
	{return false;}
	return true;
#endif
}
