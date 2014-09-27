#include "UDG_PololuMaestro.h"
#include <stdio.h>


#ifdef __linux
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
int OpenSerialPort(const char* device)
{
	int fd = open(device, O_RDWR | O_NOCTTY);
	struct termios options;
  tcgetattr(fd, &options);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  options.c_oflag &= ~(ONLCR | OCRNL);
  tcsetattr(fd, TCSANOW, &options);
  return fd;
}
#else
#pragma comment(lib,"UDG_PololuMaestro.lib")
HANDLE OpenSerialPort(LPWSTR name)
{
	HANDLE portDescriptor;
	DCB deviceControlBlock;
	portDescriptor = CreateFile(name,
				GENERIC_READ|GENERIC_WRITE,
				0,0,OPEN_EXISTING,0,0);
	if(portDescriptor == INVALID_HANDLE_VALUE)
	{
		printf("Failed opening %ls\n", name);
		return INVALID_HANDLE_VALUE;
	}

	FillMemory(&deviceControlBlock,sizeof(DCB),0);
	deviceControlBlock.DCBlength = sizeof(DCB);
	if(BuildCommDCB(L"57600,n,8,1",&deviceControlBlock))
	{
		printf("Successfully configured DCB!\n");
		
	}
	else
	{
		printf("Something went wrong while building the DCB\n");
		return INVALID_HANDLE_VALUE;
	}

	if (!SetCommState(portDescriptor,&deviceControlBlock))
	{
		printf("Error configuring port\n");
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		printf("Serial port ready\n");
	}
		
	return portDescriptor;
}
#endif

int main()
{
	HANDLE fd;
#ifdef __linux
	const char * device = "/dev/ttyACM0";  // Linux
  fd = OpenSerialPort(device);
	
#else
	LPCWSTR name = L"COM7";
	fd = OpenSerialPort(L"COM7");
#endif

	ServoMotor motor;
	motor.SetParameters(992,2000,1500,90);


	PololuMaestro maestro(fd,18);
	maestro.AssignChannel(0,motor);
	maestro.SetSpeed(0,18);
	maestro.SetTarget(0,motor.neutralPulse);
	while (maestro.IsMoving());
	printf("INITIAL angle: %f	pulse width: %u\n",maestro.GetAngle(0), maestro.GetPosition(0));
	
	float angle = 45;
	maestro.SetAngle(0,angle);
	while (maestro.IsMoving());
	printf("FINAL angle: %f	pulse width: %u\n",maestro.GetAngle(0), maestro.GetPosition(0));
	
	return 0;
}

