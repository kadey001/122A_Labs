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
	DDRB = 0xFF; PORTB = 0x00;
	/* Insert your solution below */
	TimerSet(500);
	
	uc output = 0x00;
	uc mainSequence = 0x01;
	uc flashingLED = 0x00;
	uc flashingLEDTick = 0x00;

	while (1) {
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
		}
		output = output | flashingLED;
		PORTB = output;
		flashingLEDTick++;
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
