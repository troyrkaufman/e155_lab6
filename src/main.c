/*
File: Lab_6_JHB.c
Author: Josh Brake
Email: jbrake@hmc.edu
Date: 9/14/19
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "../lib/STM32L432KC_FLASH.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

int main(void) {
  configureFlash();
  configureClock();

  RCC -> CFGR |= (0b0000 << 4); // SYSCLK is divided by 4 from the AHB PRESC in clock tree (to get to 1MHz)
  RCC -> CFGR |= (0b000 << 8);  // HCLK (formerly SYSCLK)is not divided by APB1 PRESC (Setting this to 0b0xx avoids 2x multiplier)

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  // Designate the SPI pins
  pinMode(PA5, GPIO_ALT);    // SCK
  pinMode(PB5, GPIO_ALT);    // SDO
  pinMode(PA6, GPIO_ALT);   // SDI
  pinMode(PA8, GPIO_OUTPUT); // CE
  //pinMode(PB3, GPIO_OUTPUT); // LED

  // Give SPI pins proper ALT functinos
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SCK PA5
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SDO PB5
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5); // SDI PA6

  uint8_t hiByte = 0;

  // TODO: Add SPI initialization code
   // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, 0b0111);
    
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, 0b0);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, 0b1);
    
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_LSBFIRST, 0b0);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CRCEN, 0b0);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSM, 0b1);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSI, 0b1);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 0b1);

    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 0b1);
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRF, 0b0);
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0b1);
    
    SPI1->CR2 &= ~SPI_CR2_NSSP;

    SPI1->CR1 |= SPI_CR1_SPE;


  while(1) {

    digitalWrite(PA8, 0b1);
    spiSendReceive(0x80); // Write to configuration register setup
    spiSendReceive(0b11100000); // Write data
    digitalWrite(PA8, 0b0);

    digitalWrite(PA8, 0b1);
    spiSendReceive(0x00);          // Access Temperature MSB 
    hiByte = spiSendReceive(0x00); // Wait sometime to retrieve data
    digitalWrite(PA8, 0b0);
  }
}