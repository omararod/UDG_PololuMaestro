#include "UDG_PololuMaestro.h"
int ServoMotor::AngleToPulse(float angle)
{
	float correctedAngle = angle; 
	if(angle < degreeRange * -1)
		correctedAngle = degreeRange*-1;
	if(angle > degreeRange)
		correctedAngle = degreeRange;

	return (float)(((float)(correctedAngle+degreeRange)/(float)(2*degreeRange))*((float)maxPulse - (float)minPulse)+minPulse);
}

void ServoMotor::SetParameters(int _minPulse, int _maxPulse, int _neutralPulse, int _degreeRange)
{
	minPulse = _minPulse;
	maxPulse = _maxPulse;
	neutralPulse = _neutralPulse;
	degreeRange = _degreeRange;
}