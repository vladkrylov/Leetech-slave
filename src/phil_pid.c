#include "phil_pid.h"
#include "phil_i2c.h"

void InitPID(SPid* s)
{
	s->numOfPoints = 0;
	
	s->iState = 0;
	s->dState = 0;
	
	s->pGain = 0.02;
	s->iGain = 1.45e-5;
	s->dGain = 0.01;
	
	s->pKGain = 1.;
	s->Kpk = 0.0;
	
	s->iMin = 0;
	s->iMax = 1000000;
	
	s->minOutputLimit = 5;
	s->maxOutputLimit = 120;
}

uint16_t UpdatePID(SPid* pid, int coord, int destination)
{
  int pTerm, iTerm, dTerm;
	int error;
	int result;
	
	pid->numOfPoints += 1;
	
	error = absI(destination, coord);
	
//	if ((error > 50) && ((coord - pid->dState) == 0) && (pid->numOfPoints > 20)) {
//		pid->pKGain = 1.2 + pid->Kpk * (444 - error);
//		if (pid->pKGain < 1.) {
//			pid->pKGain = 1.;
//		}
//	}
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

	result = pTerm + iTerm - dTerm;
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
