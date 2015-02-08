#include "main.h"
#include "phil_pwm.h"
#include "phil_rele.h"
#include "phil_i2c.h"
#include "phil_typedefs.h"
#include "phil_pid.h"

const uint8_t N_MOTORS = 4;

static uint16_t PWM_PERIODS[N_MOTORS] = {260, 248, 251, 251};
static uint16_t PWM_PERIOD = 260;
static uint16_t PWM_PULSE = 120;

static uint16_t coordinateToSet = 0;
const uint16_t sizeOfGlobalArrays = 200;

uint16_t pulseValues[sizeOfGlobalArrays] = {0};
uint16_t coordinates[sizeOfGlobalArrays] = {0};
uint16_t times[sizeOfGlobalArrays] = {0};

SPid pid;

CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;

/* Delay -----------------------------------------------*/	
void Delay(uint32_t delay)
{
	uint32_t i;
	for(i=0; i<delay; i++);
}

void SendCoordinate(uint16_t coordindate, uint8_t steps2mm)
{
	Convert16to2_8(coordindate, &TxMessage.Data[0], &TxMessage.Data[1]);
//	TxMessage.Data[0] = coordindate >> 8;
//	TxMessage.Data[1] = coordindate & LSBYTE;
	TxMessage.Data[2] = steps2mm;
	
	TxMessage.Data[5] = SINGLE_COORDINALTE;
	TxMessage.Data[6] = SINGLE_COORDINALTE;
	TxMessage.Data[7] = SINGLE_COORDINALTE;
	
	CAN_SafeTransmit(&TxMessage);
}

void SendEncoderOutput(uint8_t* data, uint8_t steps2mm, uint8_t length)
{
	uint8_t message_length;
	uint8_t i;
	
	if (length > 8) {
		message_length = 8;
	} else {
		message_length = length;
	}
	
	for (i=0; i<message_length; i++) {
//		Phil_TxMessage.Data[i] = i;
		TxMessage.Data[i] = data[i];
	}
	TxMessage.Data[5] = steps2mm;
//	Phil_TxMessage.Data[0] = '0';
	CAN_Transmit(CANx, &TxMessage);
}

void SendArray8Bytes(uint8_t* data)
{
	uint8_t message_length = 8;
	uint8_t i;
//	uint8_t arrayForDebug[8];
	
	for (i=0; i<message_length; i++) {
//		arrayForDebug[i] = data[i];
		TxMessage.Data[i] = data[i];
	}
	CAN_Transmit(CANx, &TxMessage);
}

void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  uint16_t LED_PINS;
	
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	LED_PINS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, LED_PINS);
  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 200;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x122;
  TxMessage.ExtId = 0x02;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}

void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
	TIM_Cmd(TIM5, DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
//	TIM_TimeBaseStructure.TIM_Period = 100;
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 1;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

//  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	
	// Master - TIM2 ----------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	// Auxilary - TIM5 ----------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 399;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	
	// Slaves - TIM3 and TIM4 -------------------------------
  /* TIM3 and TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOC and GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
  
	// ----------------------------------------------------
	// GPIOs
	// ----------------------------------------------------
  /* GPIOC Configuration: TIM3 CH1 (PC6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	/* GPIOC Configuration: TIM4 CH4 (PD15) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Connect TIM3 and TIM4 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	// ----------------------------------------------------
	// Timers
	// ----------------------------------------------------
	
	TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 and Channel4 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = PWM_PULSE;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, DISABLE);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, DISABLE);
	
	// Synchronization --------------------------------------------
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Enable);

	TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);

	TIM_SelectInputTrigger(TIM4, TIM_TS_ITR1);
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Gated);
	
	TIM3->CNT = PWM_PULSE + 2;
	TIM4->CNT = PWM_PULSE + 2;
}

void PWM_start(uint8_t motorID)
{
	ApplyMotorSettings(motorID);
	
	TIM3->CNT = PWM_PULSE;
	TIM4->CNT = TIM3->CNT + (PWM_PERIOD)/2 + 1;	
	
	// Enable slave timers
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	
	// Enable master timer
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_stop(void)
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
	
	TIM_Cmd(TIM2, DISABLE);
	
	TIM3->CNT = PWM_PULSE + 2;
	TIM4->CNT = PWM_PULSE + 2;	
}

void PWM_Run(uint8_t motorID, uint32_t duration)
{
	uint32_t i = 0;
	
	PWM_start(motorID);
	for(i=0; i<duration; i++);
	PWM_stop();
}

void SetDestinationCoordinate(uint16_t x)
{
	coordinateToSet = x;
}

uint32_t GetCoordinateToSet(void)
{
	return coordinateToSet;
}

direction_t DetermDirection(uint16_t coordToSet, uint16_t coordinate)
{
	direction_t direction;
	if (coordToSet > coordinate) {
		direction = FORWARD;
	} else {
		direction = BACK;
	}
	return direction;
}

uint16_t HotfixMove(uint8_t motorID, uint16_t coordToSet, uint8_t steps2mm, uint16_t precision)
{
	uint16_t coord;
	direction_t direction;

	uint16_t i = 1;
	uint16_t stopInd = 0;

	coordinates[0] = steps2mm * 4096 + GetMotorCoordinate(motorID);
	times[0] = 0;
	pulseValues[0] = PWM_PULSE;

	if (abs(coordinates[0], coordToSet) > precision) {
		direction = DetermDirection(coordToSet, coordinates[0]);
		SetDirection(motorID, direction);
		
		TIM5->CNT = 0;
		TIM5->CR1 |= TIM_CR1_CEN;
		PWM_start(motorID);

		while(1) {			
			if (MotorStuck(coordinates, PrevInd(i), 30)) {
				PWM_stop();
				break;
			}
			
			pulseValues[i] = PWM_PULSE;
			coordinates[i] = steps2mm * 4096 + GetMotorCoordinate(motorID);
			times[i] = TIM5->CNT;
			
			Check4OverStep2mm(coordinates[PrevInd(i)], &coordinates[i], &steps2mm);
			
			if ((abs(coordinates[i], coordToSet) < precision) || 
					((direction == FORWARD) && (coordinates[i] >= coordToSet)) ||
					((direction == BACK) && (coordinates[i] <= coordToSet))
				 ) {
				PWM_stop();
				break;
			}
			
			i++; i %= sizeOfGlobalArrays;
		}

		stopInd = i;
		
		while(1) {
			coordinates[i] = steps2mm * 4096 + GetMotorCoordinate(motorID);
			times[i] = TIM5->CNT;
			Check4OverStep2mm(coordinates[PrevInd(i)], &coordinates[i], &steps2mm);
			if (MotorStuck(coordinates, PrevInd(i), 5)) break;
			i++; i %= sizeOfGlobalArrays;
		}
		
		TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN;
		TIM5->CNT = 0;
	}

	coord = steps2mm * 4096 + GetMotorCoordinate(motorID);
	Check4OverStep2mm(coordinates[PrevInd(i)], &coord, &steps2mm);
		
	return coord;
}

void Check4OverStep2mm(uint16_t prevCoord, uint16_t* nextCoordptr, uint8_t* steps2mm)
{
	uint16_t nextCoord = *nextCoordptr;
	if (prevCoord > nextCoord + 2048) {
		*steps2mm += 1;
		nextCoord += 4096;
	}
	else if ((prevCoord < nextCoord) && abs(prevCoord, nextCoord) > 2048) {
		*steps2mm -= 1;
		nextCoord -= 4096;
	}
	*nextCoordptr = nextCoord;
}

uint8_t MotorStuck(uint16_t* coordArray, 
									 uint16_t lastReceivedCoordinateIndex,
									 uint8_t numOfRepeats)
{
	uint8_t i;
	uint8_t res = 1;
	
	if (lastReceivedCoordinateIndex < numOfRepeats) return 0;
	
	for(i=0; i < numOfRepeats-1; i++) {
		if (abs(coordArray[lastReceivedCoordinateIndex], 
						coordArray[PrevIndN(lastReceivedCoordinateIndex, i+1)]) > 4) 
		{
			res = 0;
			break;
		}
	}
	
	return res;
}

uint8_t MotorStop(uint16_t coord, uint16_t coordToSet, uint16_t presicion)
{
	if (abs(coord, coordToSet) <= presicion) {
		PWM_stop();
		return 1;
	}
	return 0;
}

uint16_t Reset(uint8_t motorID)
{
	uint16_t origin = UINT16_MAX;
	uint16_t coordinates[sizeOfGlobalArrays] = {0};
	uint32_t i = 1;
	
	SetDirection(motorID, BACK);
	coordinates[0] = GetMotorCoordinate(motorID);
	
	PWM_start(motorID);
	while(1) {
		if (MotorStuck(coordinates, PrevInd(i), 40)) {
			PWM_stop();
			
			Delay(100000);
			origin = GetMotorCoordinate(motorID);
			return origin;
		}
		coordinates[i] = GetMotorCoordinate(motorID);
		i++; i %= sizeOfGlobalArrays;
	}
}

void Test(uint8_t motorID)
{
	uint8_t i = 0;
	uint32_t moveTime = 10000000;
	
	UpdateTimers(motorID, PWM_PERIOD/2 - 5, PWM_PERIOD);
	for(i=0; i<1; i++) {
		SetDirection(motorID, FORWARD);
		PWM_Run(motorID, moveTime);
		Delay(moveTime/10);
		
		SetDirection(motorID, BACK);
		PWM_Run(motorID, moveTime);
		Delay(moveTime/10);
	}
}

void TestPulsesForOscilloscope()
{
		PWM_Run(1, 100000000);
}

uint16_t PrevInd(uint16_t i) 
{
	return PrevIndN(i, 1);
}

uint16_t PrevIndN(uint16_t i, uint16_t n) 
{
	return (sizeOfGlobalArrays + i - n) % sizeOfGlobalArrays;
}

// motorID = 0...N_MOTORS-1
void UpdateTimers(uint8_t motorID, uint16_t pulseWidth, uint16_t pulsePeriod)
{
	PWM_PULSE = pulseWidth;
  PWM_PERIODS[motorID] = pulsePeriod;
} 

void UpdateTimersWidth(uint16_t pulseWidth)
{
	PWM_PULSE = pulseWidth;

	TIM3->CCR1 = PWM_PULSE;
	TIM4->CCR4 = PWM_PULSE;
}

// This function must be called before any movings on motor applied
// motorID = 1...N_MOTORS
void ApplyMotorSettings(uint8_t motorID)
{
	PWM_PERIOD = PWM_PERIODS[motorID-1];
	
	TIM3->CCR1 = PWM_PULSE;
	TIM4->CCR4 = PWM_PULSE;
	TIM3->ARR = PWM_PERIOD;
	TIM4->ARR = PWM_PERIOD;

	TIM3->CNT = PWM_PULSE + 2;
	TIM4->CNT = PWM_PULSE + 2;
}

void Convert16to2_8(uint16_t val, uint8_t *part1, uint8_t *part2)
{
	*part1 = val >> 8;
	*part2 = val & LSBYTE;
}

void SendFlag(can_flag f)
{
	uint8_t i = 0;
	for (i=5; i<8; i++) {
		TxMessage.Data[i] = (uint8_t) f;
	}	
	CAN_SafeTransmit(&TxMessage);
}

uint16_t SendArrayFragment(uint16_t *array, uint16_t startIndex, uint16_t len)
{
	// CAN frame length is 8 bytes
	// we can put only 4 uint16_t array values to 1 CAN message
	uint16_t i = 0;
	uint16_t numberOfValuesTransmitted = 0;
	for(i=0; i<4; i++) {
		if (startIndex + i < len) {
			Convert16to2_8(array[startIndex + i], &TxMessage.Data[2*i], &TxMessage.Data[2*i+1]);
			numberOfValuesTransmitted++;
		} else {
			TxMessage.Data[2*i] = 0;
			TxMessage.Data[2*i+1] = 0;
		}
	}
	CAN_SafeTransmit(&TxMessage);
	return numberOfValuesTransmitted;
}

void SendArray(can_flag f, uint16_t *array, uint16_t len)
{
	uint16_t nextFragmentIndex = 0;
	uint16_t bytesSent = 0;
	SendFlag(f);
	while (nextFragmentIndex < len) {
		bytesSent = SendArrayFragment(array, nextFragmentIndex, len);
		if (bytesSent < 4) break;
		else
			nextFragmentIndex += bytesSent;
	}
	SendFlag(FINISH);
}

void Wait4TransmissionComplete(uint8_t TransmitMailbox)
{
	uint8_t status;
	uint32_t i = 0;
  while((status != CAN_TxStatus_Ok) && (i != 0x000FFFFF))               
  {
    status = CAN_TransmitStatus(CANx, TransmitMailbox);
    i++;
  }
}

void CAN_SafeTransmit(CanTxMsg* TxMessage)
{
	uint8_t TransmitMailbox;
	TransmitMailbox = CAN_Transmit(CANx, TxMessage);
	Wait4TransmissionComplete(TransmitMailbox);
}

void SendTimes(void)
{
	SendArray(TIME_START, times, sizeOfGlobalArrays);
}

void SendUSignal(void)
{
	SendArray(U_SIGNAL_START, pulseValues, sizeOfGlobalArrays);
}

void SendCoordinates(void)
{
	SendArray(COOORDINATES_START, coordinates, sizeOfGlobalArrays);
}

void SendTrajectory(void)
{
	SendTimes();
	SendUSignal();
	SendCoordinates();
	SendFlag(TRAJECTORY_TRANSMITTED);
}







