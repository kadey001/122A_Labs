/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: USART transmission to another MC to blink LED in sync
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

#define uc unsigned char

void transmitUSART(unsigned char targetUSART, unsigned char data);

//Leader Micro Controller Code
int main(void){
	DDRA = 0xFF; PORTA = 0x00;//A OUTPUT
	DDRD = 0xFF; PORTD = 0x00;//D OUTPUT

    //initUSART(0);//Initialize USART 0
    initUSART(1);//Initialize USART 1
	
	unsigned long timeInterval = 500;//500 ms
	TimerSet(timeInterval);//1 second timer
	TimerOn();//Start timer

	uc localLED = 0;
	uc receivingUSART = 1;
	while(1){
		if(USART_HasReceived(receivingUSART)){//Checks if USART has received data
			localLED = USART_Receive(receivingUSART); //store received data
		}
		PORTA = localLED;//Set local LED On/Off after data is received
		
		while(!TimerFlag);//Wait .5 seconds
		TimerFlag = 0;
	}
	return 1;
}

void transmitUSART(unsigned char targetUSART, unsigned char data){
	USART_Send(data, targetUSART);//Send data
	while(!USART_HasTransmitted(targetUSART));//Wait for transmission to complete
	USART_Flush(targetUSART);//Flushing data registers
	return;
}
