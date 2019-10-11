/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description: Automatic Leader/Follower switching
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

#define uc unsigned char

//Leader Micro Controller Code
int main(void){
	DDRA = 0xFF; PORTA = 0x00;//A OUTPUT
	DDRB = 0xFF; PORTB = 0x00;//B Output
	DDRC = 0x00; PORTC = 0xFF;//C INPUT
	DDRD = 0xFF; PORTD = 0x00;//D OUTPUT

	initUSART(0);//Initialize USART 0 RECIEVING
	initUSART(1);//Initialize USART 1 SENDING

	unsigned long timeInterval = 1000;//1000 ms
	TimerSet(timeInterval);//1 second timer
	TimerOn();//Start timer

	uc localLED = 0;
	uc receivingUSART = 0;
	uc sendingUSART = 1;
	uc mode = 1;//1 = Leader | 0 = Follower
	uc followerTick = 0;
	
	while(1){
		if(mode == 1) {//Leader
			PORTB = 0x01;//Leader LED enabled
			
			if(USART_IsSendReady(sendingUSART)){//Checks if USART is ready for to transmit
				USART_Send(localLED, sendingUSART);//Transmit data
				//USART_Flush(sendingUSART);
			}
			PORTA = localLED;//Set local LED On/Off after data is transmitted
			localLED = (localLED == 0) ? 1 : 0;//Flip LED On/Off

			if(USART_HasReceived(receivingUSART)) {
				mode = 0;//Switch to follower
			}
		}
		else {//Follower
			PORTB = 0x00;//Leader LED disabled
			
			if(USART_HasReceived(receivingUSART)){//Checks if USART has received data
				localLED = USART_Receive(receivingUSART); //store received data
				//USART_Flush(receivingUSART);
			}
			PORTA = localLED;//Set local LED On/Off after data is transmitted

			if(followerTick == 5) {//If 3 seconds without any data
				mode = 1;//Switch to Leader
				followerTick = 0;
			} else {
				if(USART_HasReceived(1)) {//Checks if data has been received, if it has then ticker is reset, if not then ticker is incremented.
					followerTick = 0;
				} else {
					followerTick++;
				}
			}
		}
		while(!TimerFlag);//Wait 1 second
		TimerFlag = 0;
	}
	return 1;
}