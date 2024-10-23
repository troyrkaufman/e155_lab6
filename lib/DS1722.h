// DS1722.h
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/19/24
// Description: This header file creates the 2s complement to decimal calculation prototype


#include <stdint.h>
#include <stm32l432xx.h>
#include "STM32L432KC_SPI.h"

double readTemp(int resolution);

void writeRes(int resolution);




