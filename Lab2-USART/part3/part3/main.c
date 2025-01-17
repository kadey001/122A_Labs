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
<<<<<<< HEAD
	uc mode = 0;//1 = Leader | 0 = Follower
=======
	uc usartMode = 0;//1 = Leader | 0 = Follower
>>>>>>> 2eefe4420958c9dffbb5897152054d8ed630d1c7
	uc followerTick = 0;
	uc hasReceived = 0;
	
	while(1){
		if(usartMode == 1) {//Leader
			TimerSet(1000);
			PORTB = 0x01;//Leader LED enabled
			
			if(USART_IsSendReady(sendingUSART)){//Checks if USART is ready for to transmit
				USART_Send(localLED, sendingUSART);//Transmit data
			}
			PORTA = localLED;//Set local LED On/Off after data is transmitted
			localLED = (localLED == 0) ? 1 : 0;//Flip LED On/Off

			if(USART_HasReceived(receivingUSART)) {
				usartMode = 0;//Switch to follower
			}
		}
		else {//Follower
			TimerSet(50);
			PORTB = 0x00;//Leader LED disabled
			
			if(USART_HasReceived(receivingUSART)){//Checks if USART has received data
				hasReceived = 1;
				localLED = USART_Receive(receivingUSART); //store received data
			}
			PORTA = localLED;//Set local LED On/Off after data is transmitted

			if(followerTick == 60) {//If 3 seconds without any data
				usartMode = 1;//Switch to Leader
				followerTick = 0;
			} 
			else {
				//followerTick++;
				if(hasReceived) {//Checks if data has been received, if it has then ticker is reset, if not then ticker is incremented.
					hasReceived = 0;
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