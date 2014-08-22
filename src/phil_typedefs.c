#include "phil_typedefs.h"

commands_t WhatToDo(uint8_t symbol)
{
	switch (symbol) {
	case 'm':
		return MOVE;
	case 'r':
		return RESET_ONE;
	case 'R':
		return RESET_ALL;
	case 'g':
		return GET_COORDINATE;
	case 't':
		return TEST;
	case 'z':
		return TEST_OSCILLOSCOPE;
	case 's':
		return SET_PULSES;
	default:
		return NOTHING;
	}
}