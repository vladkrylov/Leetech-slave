#ifndef __PHIL_PID_H
#define __PHIL_PID_H

#include "stm32f4xx.h"

typedef struct
{
	uint16_t numOfPoints;
	
  int dState;                  // Last position input
  int iState;                  // Integrator state
  int iMax, iMin;      
  // Maximum and minimum allowable integrator state
  float    iGain,        // integral gain
            pGain,        // proportional gain
            dGain;         // derivative gain
	float pKGain, Kpk;
	
	int minOutputLimit;
	int maxOutputLimit;
} SPid;

void InitPID(SPid* s);
uint16_t UpdatePID(SPid* pid, int coord, int destination);

#endif /* __PHIL_PID_H */
