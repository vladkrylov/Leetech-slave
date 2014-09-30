#include "phil_i2c.h"
 
void USART3_Init()
{
	USART_InitTypeDef    USART_InitStruct;
	GPIO_InitTypeDef    GPIO_InitStruct;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStruct);

  USART_Cmd(USART3, ENABLE);
}

void Usart3_Send_symbol(uint8_t data)
{
  while(!(USART3->SR & USART_FLAG_TC));
  USART3->DR = data;
}

void Usart3_Send_String(char* str)
{
  uint8_t i=0;
  while(str[i])
  {
    Usart3_Send_symbol(str[i]);
    i++;
  }
  Usart3_Send_symbol('\n');
  Usart3_Send_symbol('\r');
}

void I2C3_Init(void)
{        
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	// enable APB1 peripheral clock for I2C3 and I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* setup SCL and SDA pins*/
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8; 												// we are going to use PA8
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                     // set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                // set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;                   // set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                     // enable pull up resistors
	GPIO_Init(GPIOA, &GPIO_InitStruct);                           // init GPIOA

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 												// we are going to use PC9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                     // set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                // set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;                   // set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                     // enable pull up resistors
	GPIO_Init(GPIOC, &GPIO_InitStruct);                           // init GPIOC
	
	// Connect I2C3 pins to AF
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3); // SCL
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3); // SDA
	
	// configure I2C3
	I2C_InitStruct.I2C_ClockSpeed = 100000;                // 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                // I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;        // 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;                 // own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;              // disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C3, &I2C_InitStruct);                       // init I2C3
	
	// enable I2C3
	I2C_Cmd(I2C3, ENABLE);
	
	// -------------------------------------------
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	// Connect I2C2 pins to AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2); // SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2); // SDA
	// configure I2C2
	I2C_InitStruct.I2C_ClockSpeed = 100000;                // 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                // I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;        // 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;                 // own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;              // disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStruct);                       // init I2C3
	
	// enable I2C2
	I2C_Cmd(I2C2, ENABLE);
}

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
	// wait until I2C1 is not busy anymore
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	// Send I2C1 START condition
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
					
	// Send slave Address for write
	I2C_Send7bitAddress(I2Cx, address, direction);
	
	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(direction == I2C_Direction_Transmitter){
					while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
					while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/* This function reads one byte from the slave device
* and acknowledges the byte (requests another byte)
*/
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx)
{
	uint8_t data;
	// enable acknowledge of recieved data
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	data = I2C_ReceiveData(I2Cx);
	return data;
}

/* This function reads one byte from the slave device
* and doesn't acknowledge the recieved data
*/
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx)
{
	uint8_t data;
	// disabe acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	data = I2C_ReceiveData(I2Cx);
	return data;
}

/* This funtion issues a stop condition and therefore
* releases the bus
*/
void I2C_stop(I2C_TypeDef* I2Cx)
{
        // Send I2Cx STOP Condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
}

void GetEncoderOutput(uint8_t motorID)
{
	uint8_t i;
	uint8_t address;
	I2C_TypeDef* I2Cphil;
	if ((motorID == 1) || (motorID == 2)) {
		I2Cphil = I2C3;
	} else {
		I2Cphil = I2C2;
	}
	
	if ((motorID % 2) == 0) {
		address = SLAVE_ADDRESS_0;
	} else {
		address = SLAVE_ADDRESS_1;
	}
	
	I2C_start(I2Cphil, address<<1, I2C_Direction_Receiver); // start a transmission in Master transmitter mode
	
	for(i=0; i<NUMBER_OF_BITS_FROM_ENCODER; i++) {
		I2C_AcknowledgeConfig(I2Cphil, ENABLE);
		while( !I2C_CheckEvent(I2Cphil, I2C_EVENT_MASTER_BYTE_RECEIVED) );
		
		encoderOutput[i] = I2C_ReceiveData(I2Cphil);
	}
	
	I2C_AcknowledgeConfig(I2Cphil, DISABLE);
	I2C_GenerateSTOP(I2Cphil, ENABLE);
}

uint16_t abs(uint16_t x, uint16_t y)
{
	if (x < y) {
		return y - x;
	} else {
		return x - y;
	}
}

int absI(int x, int y)
{
	if (x < y) {
		return y - x;
	} else {
		return x - y;
	}
}

uint16_t GetMotorCoordinate(uint8_t motorID)
{
	uint16_t x;
	GetEncoderOutput(motorID);
	x = encoderOutput[0]<<4 | encoderOutput[1]>>4;
	return x;
}


void PrintCoordinate(uint8_t* encoderOutpt)
{
	uint8_t i;
	for(i=0; i<NUMBER_OF_BITS_FROM_ENCODER; i++) {
//	for(i=0; i<2; i++) {
//		Usart3_Send_symbol(encoderOutpt[i]);
//		Usart3_Send_symbol('a');
	}
}


