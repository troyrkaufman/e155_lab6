// DS1722.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/19/24
// Description: This C file creates the 2s complement to decimal calculation 

#include "DS1722.h"
//#include "STM32L432KC.h"
#include "STM32L432KC_SPI.h"

/*
    Configuration status register setup*
    1 1 1 1SHOT R2 R1 R0 SD

    The R0-R2 defines the resolution
    SD is shutdown mode (0 for continuous values and 1 for one value then on low power)
*/

int16_t readTemp(int resolution){ //Faulty 
    int16_t binary;
    int16_t decimal;
    uint8_t hiByte = 0;
    uint8_t loByte = 0;

    /* 
    if (resolution <= 8){
        spiSendReceive(0x02);          // Access Temperature MSB
        temp = spiSendReceive(0x00);   // Wait sometime to retrieve data. 
    } else {
        spiSendReceive(0x02);          // Access Temperature MSB 
        hiByte = spiSendReceive(0x00); // Wait sometime to retrieve data
        spiSendReceive(0x01);          // Access Temperature LSB 
        loByte = spiSendReceive(0x00); // Wait sometime to retrieve data
    }
    */
    spiSendReceive(0x02);          // Access Temperature MSB 
    hiByte = spiSendReceive(0x00); // Wait sometime to retrieve data

    //digitalWrite(PA8, 0);
    //digitalWrite(PA8, 1);

    spiSendReceive(0x01);          // Access Temperature LSB 
    loByte = spiSendReceive(0x00); // Wait sometime to retrieve data

    int16_t data = (hiByte<<8) | loByte;   // Creates 16 bit piece of data representing temperature at requested resolution

    switch(resolution){
        case 8: 
            binary = (data>>8);
            break;
        case 9:
            binary = (data>>7); 
            break;
        case 10:
            binary = (data>>6); 
            break;
        case 11:
            binary = (data>>5); 
            break;
        case 12:
            binary = (data>>4); 
            break;
        default: 
            return 0;
    }

    return binary;
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

