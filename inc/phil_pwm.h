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

void ClearGlobalArrays(void);
uint16_t PrevInd(uint16_t i);
uint16_t PrevIndN(uint16_t i, uint16_t n);

void PWM_Run_test(uint32_t coordinate);

void SetDestinationCoordinate(uint16_t);
uint32_t GetCoordinateToSet(void);

void Init_RxMes(CanRxMsg *RxMessage);

void SendCoordinate(uint16_t coordindate, action_performed_t act);
void SendEncoderOutput(uint8_t* data, uint8_t steps2mm, uint8_t length);
void SendArray8Bytes(uint8_t* data);

void Move1Unit(uint8_t motorID, direction_t direction);
uint16_t Move(uint8_t motorID, uint16_t coordToSet, uint8_t steps2mm, uint16_t precision);
uint16_t Reset(uint8_t motorID);

// Tests
void Test(uint8_t motorID);
//void TestPulsesForOscilloscope();

uint32_t RExp(uint32_t x);
void Check4OverStep2mm(uint16_t prevCoord, uint16_t* nextCoordptr, uint8_t* steps2mm);
uint8_t MotorStuck(uint16_t* coordArray, 
									 uint16_t lastReceivedCoordinateIndex,
									 uint8_t numOfRepeats);
uint8_t MotorStop(uint16_t coord, uint16_t coordToSet, uint16_t presicion);

// Trajectory transmit routine
void SendFlag(can_flag f);
uint16_t SendArrayFragment(uint16_t *array, uint16_t startIndex, uint16_t len);
void SendArray(can_flag f, uint16_t *array, uint16_t len);

void SendTimes(uint16_t len);
void SendUSignal(uint16_t len);
void SendCoordinates(uint16_t len);
void SendTrajectory(void);

void Convert16to2_8(uint16_t val, uint8_t *part1, uint8_t *part2);
void Wait4TransmissionComplete(uint8_t TransmitMailbox);
void CAN_SafeTransmit(CanTxMsg* TxMessage);

#endif /* __PHIL_PWM_H */

