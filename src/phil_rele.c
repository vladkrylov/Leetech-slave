#include "phil_rele.h"

/* Delay -----------------------------------------------*/	
void rDelay(uint32_t delay)
{
	uint32_t i;
	for(i=0; i<delay; i++);
}

void MotorSwitchInit(void)
{	
	GPIO_InitTypeDef rele_1;
	GPIO_InitTypeDef rele_2_5;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE );
		
	rele_1.GPIO_Mode = GPIO_Mode_OUT;
	rele_1.GPIO_OType = GPIO_OType_PP ;
	rele_1.GPIO_Pin = GPIO_Pin_6;
	rele_1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	rele_1.GPIO_Speed = GPIO_Speed_2MHz;
		
	rele_2_5.GPIO_Mode = GPIO_Mode_OUT;
	rele_2_5.GPIO_OType = GPIO_OType_PP ;
	rele_2_5.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	rele_2_5.GPIO_PuPd = GPIO_PuPd_NOPULL;
	rele_2_5.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &rele_1);
	GPIO_Init(GPIOE, &rele_2_5);
}

void SetDirection(uint8_t motorID, direction_t direction)
{
	switch(motorID) {
		case 1:
			if (direction == FORWARD) {
				MOTOR_1_B
			} else {
				MOTOR_1_F
			}
			break;
		case 2:
			if (direction == FORWARD) {
				MOTOR_2_B
			} else {
				MOTOR_2_F
			}
			break;
		case 3:
			if (direction == FORWARD) {
				MOTOR_3_B
			} else {
				MOTOR_3_F
			}
			break;
		case 4:
			if (direction == FORWARD) {
				MOTOR_4_B
			} else {
				MOTOR_4_F
			}
			break;
		default:
			break;
	}
	// Wait until rele will switch
	// Don't remove this delay!
	// It causes a strong cross-talk on the 3.3V bus
	rDelay(500000);
}