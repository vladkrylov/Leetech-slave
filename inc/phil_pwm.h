#ifndef __PHIL_PWM_H
#define __PHIL_PWM_H

#include "stm32f4xx.h"
#include "phil_typedefs.h"

void Delay(uint32_t delay);

void NVIC_Config(void);
void CAN_Config(void);
void TIM_Config(void);

void UpdateTimers(uint8_t motorID, uint16_t pulseWidth, uint16_t pulsePeriod);
void UpdateTimersWidth(uint16_t pulseWidth);
void ApplyMotorSettings(uint8_t motorID);

void PWM_start(uint8_t motorID);
void PWM_stop(void);
void PWM_Run(uint8_t motorID, uint32_t duration);

uint8_t PrevInd(uint8_t i);
uint8_t PrevIndN(uint8_t i, uint8_t n);

void PWM_Run_test(uint32_t coordinate);

void SetDestinationCoordinate(uint16_t);
uint32_t GetCoordinateToSet(void);

void Init_RxMes(CanRxMsg *RxMessage);

void SendCoordinate(uint16_t coordindate, uint8_t steps2mm);
void SendEncoderOutput(uint8_t* data, uint8_t steps2mm, uint8_t length);
void SendArray8Bytes(uint8_t* data);

void Move1Unit(uint8_t motorID, direction_t direction);
uint16_t HotfixMove(uint8_t motorID, uint16_t coordToSet, uint8_t steps2mm, uint16_t precision);
uint16_t Move(uint8_t motorID, uint16_t coordToSet, uint8_t steps2mm);
uint16_t Reset(uint8_t motorID);

// Tests
void Test(uint8_t motorID);
void TestPulsesForOscilloscope();

uint32_t RExp(uint32_t x);
void Check4OverStep2mm(direction_t direction, uint16_t lastCoord, uint16_t* nextCoordptr, uint8_t* steps2mm);
uint8_t MotorStuck(uint16_t* coordArray, 
									 uint8_t lastReceivedCoordinateIndex,
									 uint8_t numOfRepeats);
uint8_t MotorStop(uint16_t coord, uint16_t coordToSet, uint16_t presicion);

#endif /* __PHIL_PWM_H */

