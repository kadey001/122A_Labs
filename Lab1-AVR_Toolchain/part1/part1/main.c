/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #1  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"

#define uc unsigned char

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;//Output
	DDRC = 0x00; PORTC = 0xFF;//Input
	/* Insert your solution below */
	TimerSet(50);
	TimerOn();
	
	uc output = 0x00;
	uc mainSequence = 0x01;
	uc flashingLED = 0x08;
	uc flashingLEDTick = 0x00;
	uc pausePressed = 0x00;
	uc pausedSequence = 0x00;
	uc paused = 0x00;

	while (1) {
		pausePressed = ~PINC & 0x01;
		if(pausePressed) {
			paused = !paused;
		}
		
		if(!paused) {
			output = mainSequence;
			switch(mainSequence) {
				case 0x01: mainSequence = 0x02; break;
				case 0x02: mainSequence = 0x04; break;
				case 0x04: mainSequence = 0x01; break;
				default: mainSequence = 0x01; break;
			}
		
			if(flashingLEDTick == 2) {
				switch(flashingLED) {
					case 0x00: flashingLED = 0x08; break;
					case 0x08: flashingLED = 0x00; break;
					default: flashingLED = 0x00; break;
				}
				flashingLEDTick = 0;
			}
			
			flashingLEDTick++;
			
			output |= flashingLED;
			pausedSequence = output;
			PORTB = output;
		} else {
			PORTB = pausedSequence;
		}
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
