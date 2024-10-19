// STM32L432KC_SPI.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/18/24
// Description: This C file creates the initSPI and spiSendReceive functions

#include "STM32L432KC_SPI.h"

void initSPI(int br, int cpol, int cpha){
    
    // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // Set clock rate using baud divisor
    SPI1->CR1 |= (br<<3);
    // Set CPOL and CPHA to match slave. We need CHPA to be 1 FIX THIS
    SPI1->CR1 |= (cpol<<1) | (cpha<<0);
    // DFF to 8- or 16- bit data frame format
    SPI1->CR2 |= SPI_CR2_DS_3; // (0b0111 << 8);
    // Set threshold of RXFIFO that triggers an RXNE event
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // Set LSBFIRSt bit to set wheter lsb or msb is sent first (normally msb)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // Configure the NSS pin (can either use software management or a separate GPIO set as an output and manually toggle it)
    SPI1->CR2 &= ~SPI_CR2_NSSP;
    // Set master mode MSTR
    SPI1->CR1 |= SPI_CR1_MSTR;
    // Enable SPI - Set SPE bit to 1
    SPI1->CR1 |= SPI_CR1_SPE;
}

char spiSendReceive(char send){
    // As long as the TXFIFO level is less than half its capacity, send txdata
    while(!(SPI1->SR & SPI_SR_FTLVL_1)){ //or is it SPI_SR_TXE?
        SPI1->DR = send;
    }

    char rxdata;
    while(1){                           // or while (!(SPI1->SR & SPI_SR_RXNE))
        // Read data once
        rxdata = SPI1->DR;
        // If RXFIFO buffer is not empty
        if (SPI1->SR & SPI_SR_RXNE){
            return rxdata;
        }
    } 
}

