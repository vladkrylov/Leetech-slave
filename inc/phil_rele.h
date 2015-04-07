#ifndef __PHIL_RELE_H
#define __PHIL_RELE_H

#include <stm32f4xx.h>
#include "phil_typedefs.h"

void MotorSwitchInit(void);
void SetDirection(uint8_t motorID, direction_t direction);

#define RELE_PORT_1 			GPIOA
#define RELE_PORT_2 			GPIOE

#define RELE_1_PIN 			GPIO_Pin_6
#define RELE_2_PIN 			GPIO_Pin_7
#define RELE_3_PIN 			GPIO_Pin_9
#define RELE_4_PIN 			GPIO_Pin_8
#define RELE_5_PIN 			GPIO_Pin_10

#define RELE_1 			RELE_PORT_1, RELE_1_PIN
#define RELE_2 			RELE_PORT_2, RELE_2_PIN
#define RELE_3 			RELE_PORT_2, RELE_3_PIN
#define RELE_4 			RELE_PORT_2, RELE_4_PIN
#define RELE_5 			RELE_PORT_2, RELE_5_PIN

#define MOTOR_1_F GPIO_ResetBits(RELE_1);\
									/*GPIO_SetBits(RELE_1);*/\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									GPIO_ResetBits(RELE_3);\
									/*GPIO_SetBits(RELE_3);*/\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/
									
#define MOTOR_1_B GPIO_ResetBits(RELE_1);\
									/*GPIO_SetBits(RELE_1);*/\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									GPIO_ResetBits(RELE_3);\
									/*GPIO_SetBits(RELE_3);*/\
									\
									/*GPIO_ResetBits(RELE_4);*/\
									GPIO_SetBits(RELE_4);\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/

#define MOTOR_2_F GPIO_ResetBits(RELE_1);\
									/*GPIO_SetBits(RELE_1);*/\
									\
									/*GPIO_ResetBits(RELE_2);*/\
									GPIO_SetBits(RELE_2);\
									\
									/*GPIO_ResetBits(RELE_3);*/\
									GPIO_SetBits(RELE_3);\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/

#define MOTOR_2_B GPIO_ResetBits(RELE_1);\
									/*GPIO_SetBits(RELE_1);*/\
									\
									/*GPIO_ResetBits(RELE_2);*/\
									GPIO_SetBits(RELE_2);\
									\
									GPIO_ResetBits(RELE_3);\
									/*GPIO_SetBits(RELE_3);*/\
									\
									/*GPIO_ResetBits(RELE_4);*/\
									GPIO_SetBits(RELE_4);\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/
									
#define MOTOR_3_F /*GPIO_ResetBits(RELE_1);*/\
									GPIO_SetBits(RELE_1);\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									GPIO_ResetBits(RELE_3);\
									/*GPIO_SetBits(RELE_3);*/\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/

#define MOTOR_3_B /*GPIO_ResetBits(RELE_1);*/\
									GPIO_SetBits(RELE_1);\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									GPIO_ResetBits(RELE_3);\
									/*GPIO_SetBits(RELE_3);*/\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									/*GPIO_ResetBits(RELE_5);*/\
									GPIO_SetBits(RELE_5);

#define MOTOR_4_F /*GPIO_ResetBits(RELE_1);*/\
									GPIO_SetBits(RELE_1);\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									/*GPIO_ResetBits(RELE_3);*/\
									GPIO_SetBits(RELE_3);\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									GPIO_ResetBits(RELE_5);\
									/*GPIO_SetBits(RELE_5);*/

#define MOTOR_4_B /*GPIO_ResetBits(RELE_1);*/\
									GPIO_SetBits(RELE_1);\
									\
									GPIO_ResetBits(RELE_2);\
									/*GPIO_SetBits(RELE_2);*/\
									\
									/*GPIO_ResetBits(RELE_3);*/\
									GPIO_SetBits(RELE_3);\
									\
									GPIO_ResetBits(RELE_4);\
									/*GPIO_SetBits(RELE_4);*/\
									\
									/*GPIO_ResetBits(RELE_5);*/\
									GPIO_SetBits(RELE_5);

//void ReleConfigure(uint8_t Motor, uint8_t Direction);

#endif // __PHIL_RELE_H

