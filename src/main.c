#include "phil_pwm.h"
#include "phil_i2c.h"
#include "phil_nvic.h"
#include "phil_rele.h"

uint8_t encoderOutput[NUMBER_OF_BITS_FROM_ENCODER];
uint16_t origins[4] = {0};

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void HotfixTest()
{
	uint8_t i;
	uint8_t motorID = 1;
	uint16_t fineBalanceMax = 100;
	uint16_t coarseBalanceMax = 500;
	uint16_t coord;
	uint16_t coordToSet = 10000;
	
//		Reset(motorID);
	coord = HotfixMove(motorID, coordToSet, 0, coarseBalanceMax);
	for(i=0; i<4; i++) {
		if (abs(coord, coordToSet) < fineBalanceMax)
			break;
		else
			coord = HotfixMove(motorID, coordToSet, coord / 4096, fineBalanceMax);
	}
	Delay(10000000);
}

int main(void)
{
  /* NVIC configuration */
  NVIC_Config();
   
  /* CAN configuration */
  CAN_Config();
	
	I2C3_Init();
	
//	USART3_Init();

//	EXTILine0_Config();
	
	TIM_Config();
	MotorSwitchInit();
	
//	HotfixTest();
	
  while(1)
  {

  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif


