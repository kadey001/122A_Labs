/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #2  Exercise #2
 *	Exercise Description: USART transmission to another MC to blink LED in sync
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "USART_ATmega1284.h"
#include "timer.h"

#define uc unsigned char

void transmitUSART(unsigned char targetUSART, unsigned char data);

//Leader Micro Controller Code
int main(void){
	DDRA = 0xFF; PORTA = 0x00;//A OUTPUT
	DDRC = 0x00; PORTC = 0xFF;//C INPUT
	DDRD = 0xFF; PORTD = 0x00;//D OUTPUT

	initUSART(0);//Initialize USART 0 RECIEVING
	initUSART(1);//Initialize USART 1 SENDING

	unsigned long timeInterval = 1000;//1000 ms
	TimerSet(timeInterval);//1 second timer
	TimerOn();//Start timer

	uc localLED = 0;
	uc targetUSART = 0;
	uc receivingUSART = 0;
	uc modeSwitch = 0;
	
	while(1){
		modeSwitch = ~PINC & 0x0C;//Gets state of the switch  0x08 is Leader and 0x04 is Follower
		
		if(modeSwitch == 0x08) {//Leader
			PORTC = 0x01;//Leader LED enabled
			targetUSART = 1;
			if(USART_IsSendReady(targetUSART)){//Checks if USART is ready for to transmit
				transmitUSART(targetUSART, localLED);//Transmit data
			}
			PORTA = localLED;//Set local LED On/Off after data is transmitted

			localLED = (localLED == 0) ? 1 : 0;//Flip LED On/Off
		} else {//Follower
			PORTC = 0x00;//Leader LED disabled
			receivingUSART = 0;
			if(USART_HasReceived(receivingUSART)){//Checks if USART has received data
				localLED = USART_Receive(receivingUSART); //store received data
			}
		}
		
		while(!TimerFlag);//Wait 1 second
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
