#ifndef __PHIL_TYPEDEFS_H
#define __PHIL_TYPEDEFS_H

#include "stm32f4xx.h"

#define LSBYTE ((uint16_t)0x00FF)
#define MSBYTE ((uint16_t)0xFF00)

typedef enum {BACK, FORWARD} direction_t;
typedef enum {NOTHING, MOVE, RESET_ONE, RESET_ALL, GET_COORDINATE, TEST, TEST_OSCILLOSCOPE, SET_PULSES} commands_t;
typedef enum {SINGLE_COORDINALTE = 8,
							TIME_START,
							TIME,
							U_SIGNAL_START,
							U_SIGNAL,
							COOORDINATES_START,
							COOORDINATES,
							FINISH,
							TRAJECTORY_TRANSMITTED,
							UNKNOWN
						} can_flag;
typedef enum {SET_OR_GET, RESET_ORIGIN} action_performed_t;

commands_t WhatToDo(uint8_t symbol);

#endif // __PHIL_TYPEDEFS_H


