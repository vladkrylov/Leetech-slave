#ifndef __PHIL_PWM_H
#define __PHIL_PWM_H

#include "stm32f4xx.h"

#define PWM_PERIOD 258
#define PWM_PULSE 120

extern uint32_t pulsesToSend;

void Delay(uint32_t delay);

void NVIC_Config(void);
void CAN_Config(void);

void TIM_Config(void);
void PWM_start(void);
void PWM_stop(void);
void PWM_Run(uint32_t duration);
void PWM_Run_test(uint32_t coordinate);
void SetDestinationCoordinate(uint16_t);
uint32_t GetCoordinateToSet(void);

void Init_RxMes(CanRxMsg *RxMessage);

void SendEncoderOutput(uint8_t* data, uint8_t steps2mm, uint8_t length);

void Move1Unit(uint8_t motorID, uint8_t direction);
void PreseciousMove(uint8_t motorID, uint16_t* coordToReturn, uint16_t coordToSet, uint8_t* steps2mm);
void PreseciousMoveTest(uint8_t motorID, uint16_t* coordToReturn, uint16_t coordToSet, uint8_t* steps2mm);
uint16_t Move(uint8_t motorID, uint16_t coordToSet, uint8_t steps2mm);

uint32_t RExp(uint32_t x);
void Check4OverStep2mm(uint8_t direction, uint16_t lastCoord, uint16_t* nextCoordptr, uint8_t* steps2mm);
uint8_t MotorStuck(uint16_t* coordArray, 
									 uint16_t lastReceivedCoordinateIndex, 
									 uint8_t coordArraySize, 
									 uint8_t numOfRepeats);
uint8_t MotorStop(uint16_t coord, uint16_t coordToSet, uint16_t presicion);

#endif /* __PHIL_PWM_H */

