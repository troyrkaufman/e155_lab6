// DS1722.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/19/24
// Description: This C file parses writing resolution information to configure the DS1722 temperature sensor
//              in addition to reading and parsing the real temperature data in different resolutions. 

#include "DS1722.h"
#include "STM32L432KC_GPIO.h"

/*
    Configuration status register setup*
    1 1 1 1SHOT R2 R1 R0 SD

    The R0-R2 defines the resolution
    SD is shutdown mode (0 for continuous values and 1 for one value then on low power)
*/

float readTemp(int resolution){  
    double binary;
    double decimal;
    double big;
    double small;
    uint8_t hiByte = 0;
    uint8_t loByte = 0;

    spiSendReceive(0x02);          // Access Temperature MSB 
    hiByte = spiSendReceive(0x00); // Wait sometime to retrieve data
    digitalWrite(PA8, 0);
    digitalWrite(PA8, 1);
    spiSendReceive(0x01);          // Access Temperature LSB 
    loByte = spiSendReceive(0x00); // Wait sometime to retrieve data

    float temperature;

    int16_t data = (hiByte<<8) | loByte;   // Creates 16 bit piece of data representing temperature at requested resolution

    return (float)data/256;

}

void writeRes(int resolution){ //writing address and data to sensor works as expected
    uint8_t data = 0xe0;

    switch(resolution){
        case 8: break;
        case 9: data |= (0b001<<1); break;
        case 10: data |= (0b010<<1); break;
        case 11: data |= (0b011<<1); break;
        case 12: data |= (0b111<<1); break;
        default: data = 0;
    }

    spiSendReceive(0x80); // Write to configuration register setup
    spiSendReceive(data); // Write data
}

