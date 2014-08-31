#include "phil_pid.h"
#include "phil_i2c.h"

void InitPID(SPid* s)
{
	s->iState = 0;
	s->dState = 0;
	
	s->pGain = 0.02;
	s->iGain = 0.01;
	s->dGain = 0.1;
	
	s->iMin = 0;
	s->iMax = 100;
}

uint16_t UpdatePID(SPid* pid, int coord, int destination)
{
  int pTerm, iTerm, dTerm;
	int error;
	int minLimit = 0;
	int maxLimit = 120;
	
	int result;
	
	error = destination - coord;
	
  pTerm = (int)(pid->pGain * error + 0.5);    // calculate the proportional term
	
	pid->iState += error;          					 		// calculate the integral state with appropriate limiting
	if (pid->iState > pid->iMax) 
      pid->iState = pid->iMax;     
  else if (pid->iState < pid->iMin) 
      pid->iState = pid->iMin;
	
  iTerm = (int)(pid->iGain * pid->iState);    // calculate the integral term
//	iTerm = 0;
	
	dTerm = (int)(pid->dGain * (coord - pid->dState));
  pid->dState = coord;

	result = pTerm + iTerm - dTerm;
	// check if result is in valid range
	if (result < 0) result *= -1;
	if (result > maxLimit) {
		result = maxLimit;
	} else if (result < minLimit) {
		result = minLimit;
	}
	
  return (uint16_t)result;
}
