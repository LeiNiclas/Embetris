#pragma once

#include <stdint.h>

class Touchscreen
{
	public:
		void init();
		void readValues(uint16_t &x, uint16_t &y, bool &touched);
};
