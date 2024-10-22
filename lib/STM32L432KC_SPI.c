// STM32L432KC_SPI.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/18/24
// Description: This C file creates the initSPI and spiSendReceive functions

#include "STM32L432KC_SPI.h"

void initSPI(int br, int cpol, int cpha){
    /*
    // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // Set clock rate using baud divisor. Clock will now run at 2.5 MHz
    //SPI1->CR1 = 0;
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BR, br);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);
    // Set CPOL and CPHA to match slave. We need CHPA to be 1 FIX THIS
    //SPI1->CR1 |= (cpol<<1) | (cpha<<0);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
     SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
    // DFF to 8- or 16- bit data frame format
    //SPI1->CR2 = 0;
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
    //SPI1->CR2 |= SPI_CR2_DS_3; // (0b0111 << 8);
    // Set threshold of RXFIFO that triggers an RXNE event
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // Set LSBFIRSt bit to set wheter lsb or msb is sent first (normally msb)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // SSOE stuff
    SPI1->CR2 |= SPI_CR2_SSOE;
    // Configure the NSS pin (can either use software management or a separate GPIO set as an output and manually toggle it)
    SPI1->CR2 &= ~SPI_CR2_NSSP;
    // Set master mode MSTR
    SPI1->CR1 |= SPI_CR1_MSTR;
    // Enable SPI - Set SPE bit to 1
    SPI1->CR1 |= SPI_CR1_SPE;
    */

    // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // Set clock rate using baud divisor. Clock will now run at 78 KHz
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BR, br);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);
    // Set CPOL and CPHA to match slave. 
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
    // Set LSBFIRSt bit to set wheter lsb or msb is sent first (normally msb)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // Set master mode MSTR
    SPI1->CR1 |= SPI_CR1_MSTR;
    // Motorola mode
    SPI1->CR2 &= ~SPI_CR2_FRF;
    // DFF to 8- or 16- bit data frame format
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
    // Set threshold of RXFIFO that triggers an RXNE event
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // SSOE stuff
    SPI1->CR2 |= SPI_CR2_SSOE;
    // Configure the NSS pin (can either use software management or a separate GPIO set as an output and manually toggle it)
    SPI1->CR2 &= ~SPI_CR2_NSSP;
    
    // Enable SPI - Set SPE bit to 1
    SPI1->CR1 |= SPI_CR1_SPE;
}


char spiSendReceive(char send){
    // Wait until the TXFIFO level is less than half of its capacity to send data
    while(!(SPI1->SR & SPI_SR_TXE)); 
    
    // Send byte once, however, we run into a slight problem since we set our bit width to 8 bits in SPI
    // 1) First capture address of the 16 bit DR
    // 2) Cast this 16 bit value as a volatile 8 bit integer pointer
    // 3) Then dereference this value
    volatile uint8_t* dr = (volatile uint8_t*)&SPI1->DR;
    *dr = send;          

    // Wait until there is data in the RXFIFO
    while((!(SPI1->SR & SPI_SR_RXNE)));
    
    // Read byte once
    return *dr;
}
/*
char spiSendReceive(char send){
    // Wait until the TXFIFO level is less than half of its capacity to send data
    while(!(SPI1->SR & SPI_SR_TXE)); 
    
    // Send byte once, however, we run into a slight problem since we set our bit width to 8 bits in SPI
    // 1) First capture address of the 16 bit DR
    // 2) Cast this 16 bit value as a volatile 8 bit integer pointer
    // 3) Then dereference this value
    *((volatile uint8_t *) &SPI1->DR) = send;             

    // Wait until there is data in the RXFIFO
    while((!(SPI1->SR & SPI_SR_RXNE)));
    
    // Read byte once
    return SPI1->DR;
}
*/