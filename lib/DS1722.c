// DS1722.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/19/24
// Description: This C file creates the 2s complement to decimal calculation 

#include "DS1722.h"

/*
    Configuration status register setup*
    1 1 1 1SHOT R2 R1 R0 SD

    The R0-R2 defines the resolution
    SD is shutdown mode (0 for continuous values and 1 for one value then on low power)
*/

int16_t readTemp(int resolution){
    int16_t temp;
    uint8_t hiByte = 0;
    uint8_t loByte = 0;

    if (resolution <= 8){
        spiSendReceive(0x02);          // Access Temperature MSB
        temp = spiSendReceive(0x00);   // Wait sometime to retrieve data. 
    } else {
        spiSendReceive(0x02);          // Access Temperature MSB 
        hiByte = spiSendReceive(0x00); // Wait sometime to retrieve data
        spiSendReceive(0x01);          // Access Temperature LSB 
        loByte = spiSendReceive(0x00); // Wait sometime to retrieve data

    }
    int16_t data = (hiByte<<8) | loByte;   // Creates 16 bit piece of data representing temperature at requested resolution

    switch(resolution){
        case 8: 
            temp = (data>>8);
            break;
        case 9:
            temp = (data>>7); 
            break;
        case 10:
            temp = (data>>6); 
            break;
        case 11:
            temp = (data>>5); 
            break;
        case 12:
            temp = (data>>4); 
            break;
        default: 
            return 0;
    }

    if (temp & (1 << (resolution - 1))){  // Checks if MSB is 1. If so, sign extend with 1s
        temp |= ~((1<<resolution) - 1); // First set bit in MSB according to resolution, then sets lower bits get set to 1, then all bit are inverted
    }

    return (float)temp;
}

void writeRes(int resolution){
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

