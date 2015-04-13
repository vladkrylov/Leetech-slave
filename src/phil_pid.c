#include "phil_pid.h"
#include "phil_i2c.h"

void InitPID(SPid* s, double Kp, double Ki, double Kd)
{
	s->numOfPoints = 0;
	
	s->baseline = 30;
	s->iState = 0;
	s->dState = 0;
	
	s->pGain = Kp;
	s->iGain = Ki;
	s->dGain = Kd;
	
	s->pKGain = 1.;
	s->Kpk = 0.0;
	
	s->iMin = 0;
	s->iMax = 1000000;
	
	s->minOutputLimit = 0;
	s->maxOutputLimit = 120;
}

uint16_t UpdatePID(SPid* pid, int coord, int destination)
{
  int pTerm, iTerm, dTerm;
	int error;
	int result;
	
	pid->numOfPoints += 1;
	
	error = absI(destination, coord);
	
  pTerm = (int)(pid->pGain * error);    // calculate the proportional term
	
	pid->iState += error;          					 		// calculate the integral state with appropriate limiting
	if (pid->iState > pid->iMax) 
      pid->iState = pid->iMax;     
  else if (pid->iState < pid->iMin) 
      pid->iState = pid->iMin;
	
  iTerm = (int)(pid->iGain * pid->iState);    // calculate the integral term
//	iTerm = 0;
	
	if (pid->numOfPoints > 2) {
		dTerm = (int)(pid->dGain * absI(coord, pid->dState));
	} else {
		dTerm = 0;
	}
  pid->dState = coord;

	result = pid->baseline + pTerm + iTerm - dTerm;
	// check if result is in valid range
	if (result > pid->maxOutputLimit) {
		result = pid->maxOutputLimit;
	} else if (result < pid->minOutputLimit) {
		result = pid->minOutputLimit;
	}
	
  return (uint16_t)result;
}

//uint16_t Homut(int coord, int destination)
//{
//	return (uint16_t)0;
//}
