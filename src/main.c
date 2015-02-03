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

int main(void)
{
  /* NVIC configuration */
  NVIC_Config();
   
  /* CAN configuration */
  CAN_Config();
	
	I2C3_Init();
	
//	USART3_Init();
	
	TIM_Config();
	MotorSwitchInit();
	
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


