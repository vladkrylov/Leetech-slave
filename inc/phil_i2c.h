#ifndef __PHIL_I2C_H
#define __PHIL_I2C_H

#define SLAVE_ADDRESS_0 0x40
#define SLAVE_ADDRESS_1 0x41
#define NUMBER_OF_BITS_FROM_ENCODER 5

#include <stm32f4xx.h>

extern uint8_t encoderOutput[NUMBER_OF_BITS_FROM_ENCODER];
extern uint16_t origins[4];

void USART3_Init(void);
void Usart3_Send_symbol(uint8_t);
void Usart3_Send_String(char* str);

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C3_Init(void);
uint8_t I2C_read_ack(I2C_TypeDef* );
uint8_t I2C_read_nack(I2C_TypeDef* );
void I2C_stop(I2C_TypeDef* );
void GetEncoderOutput(uint8_t motorID);

uint16_t abs(uint16_t x, uint16_t y);
int absI(int x, int y);
uint16_t min(uint16_t x, uint16_t y);

uint16_t GetMotorCoordinate(uint8_t motorID);
uint16_t GetFullMotorCoordinate(uint16_t lastMeasuredCoordinate);

void PrintCoordinate(uint8_t* );

#endif // __PHIL_I2C_H

