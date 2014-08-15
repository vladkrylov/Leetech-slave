#include "phil_typedefs.h"

commands_t WhatToDo(uint8_t symbol)
{
	switch (symbol) {
	case 'm':
		return MOVE;
	case 'r':
		return RESET_ONE;
	case 'a':
		return RESET_ALL;
	case 'g':
		return GET_COORDINATE;
	default:
		return NOTHING;
	}
}