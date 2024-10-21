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

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";
char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";

char* resStr = "<p>RESOLUTION Control:</p><form action=\"res8\"><input type=\"submit\" value=\"Display 8-bit resolution\"></form>\
	<form action=\"res9\"><input type=\"submit\" value=\"Display 9-bit resolution\"></form></form>\
        <form action=\"res10\"><input type=\"submit\" value=\"Display 10-bit resolution\"></form>\
        <form action=\"res11\"><input type=\"submit\" value=\"Display 11-bit resolution\"></form>\
	<form action=\"res12\"><input type=\"submit\" value=\"Display 12-bit resolution\"></form>";

char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[])
{
	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

int updateRESStatus(char request[])
{
	int res_status = 0;
	// The request has been received. now process to determine which temperature resolution will be shown
	if (inString(request, "res8")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		res_status = 8;
	} else if (inString(request, "res9")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		res_status = 9;
        } else if (inString(request, "res10")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		res_status = 10;
	} else if (inString(request, "res11")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		res_status = 11;
	} else if (inString(request, "res12")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		res_status = 12;
        }
	return res_status;
}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  // Designate the SPI pins
  pinMode(PA5, GPIO_ALT);    // SCK
  pinMode(PB5, GPIO_ALT);    // SDO
  pinMode(PA11, GPIO_ALT);   // SDI
  pinMode(PA8, GPIO_OUTPUT); // CE

  // Give SPI pins proper ALT functinos
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SCK PA5
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SDO PB5
  GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL11, 5); // SDI PA11
  
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  // TODO: Add SPI initialization code
  initSPI(4,0,1); // 8-bit width

  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */
/*
    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }
  */
    // Update string with current LED state
    //int led_status = updateLEDStatus(request);

    // Update string with current RESOLUTION state
   // float res_status = updateRESStatus(request);

    float temperature;

    // TODO: Add SPI code here for reading temperature
    digitalWrite(PA8, 1);
    //writeRes(res_status);
    writeRes(8);
    digitalWrite(PA8, 0);

    digitalWrite(PA8, 1);
    readTemp(8);
    //temperature = readTemp(res_status);
    digitalWrite(PA8, 0);

    /*
    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");
    */
    

    
/*
    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED

    sendString(USART, "<h2>LED Status</h2>");


    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");
    */
/*
    sendString(USART, webpageStart);
    sendString(USART, "<p>");
    sendString(USART, "Temperature: ");
    sendString(USART, temperature);
    sendString(USART, "</p>");

  
    sendString(USART, webpageEnd);
    */
  }
}