/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "phil_pwm.h"
#include "phil_i2c.h"
#include "phil_rele.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Networking
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CanRxMsg RxMessage;
extern uint8_t ubKeyNumber;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

#ifdef USE_CAN1
/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{	
	uint32_t i, j;
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  
  if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD)) {

		
		uint16_t coordinateToSet = RxMessage.Data[0] + (RxMessage.Data[1]<<8);
		uint16_t finalCoord;
		uint8_t motorID = RxMessage.Data[2];
		uint8_t dir = RxMessage.Data[3];
		uint8_t steps2mm = RxMessage.Data[4];
		
		GPIOE->ODR ^= GPIO_Pin_12;
		
  	i = 0;

		finalCoord = Move(motorID, coordinateToSet*4096/2000, steps2mm);
//		Move1Unit(motorID, 0);
//		finalCoord = GetMotorCoordinate(motorID);
		
//		SetDirection(1, 0);
//		Delay(10000000);
//		PWM_start();

		SendEncoderOutput(encoderOutput, finalCoord / 2000, NUMBER_OF_BITS_FROM_ENCODER);
//		PrintCoordinate(coordinate);
	}
//	CAN_ClearFlag(CAN1, 
}
#endif  /* USE_CAN1 */

#ifdef USE_CAN2
/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
	int i, delay = 1000000;
	uint8_t pulses = 0;
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);

  if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD))
//	if (RxMessage.StdId == 0x321)
  {
//		if(RxMessage.Data[0] == 1) {
//			GPIOD->ODR ^= GPIO_Pin_13;
//			PWM_start();
//		} else if(RxMessage.Data[0] == 2) {
//			GPIOD->ODR ^= GPIO_Pin_14;
//			PWM_stop();
//		} else if(RxMessage.Data[0] == 3) {
//			GPIOD->ODR ^= GPIO_Pin_15;
//		} else if(RxMessage.Data[0] == 4) {
//			GPIOD->ODR ^= GPIO_Pin_12;
//		} else {
//			GPIOD->ODR ^= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//			PWM_start();
//			for(i=0;i<delay; i++);
//			PWM_stop();
//		}
		GPIOD->ODR ^= GPIO_Pin_12;
		SetPulsesToSend(RxMessage.Data[0]);
		PWM_start();
//		Move();
		
//		GetCoordinate(coordinate);
//		PrintCoordinate(coordinate);
		SendCoordinate(coordinate, NUMBER_OF_BITS_FROM_ENCODER);
  }
}
#endif  /* USE_CAN2 */

//void TIM5_IRQHandler(void)
//{
//	uint16_t pos = 0;
//	TIM_Cmd(TIM5, DISABLE);
//	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
//	GetCoordinate(coordinate);
//	pos = coordinate[0] + ((coordinate[1]>>4)<<8);
//	Check4Stop(pos);
//}

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
