#ifndef __PHIL_TYPEDEFS_H
#define __PHIL_TYPEDEFS_H

#include "stm32f4xx.h"

typedef enum {BACK, FORWARD} direction_t;
typedef enum {NOTHING, MOVE, RESET_ONE, RESET_ALL, GET_COORDINATE, TEST, TEST_OSCILLOSCOPE} commands_t;

commands_t WhatToDo(uint8_t symbol);

#endif // __PHIL_TYPEDEFS_H