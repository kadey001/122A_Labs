/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #1  Exercise #2
 *	Exercise Description: When middle button is pressed score is incremented but hit any other time goes down to lowest of 0.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "io.h"

#define uc unsigned char

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00; //LCD Control Lines
	DDRB = 0xFF; PORTB = 0x00; //LED output
	DDRC = 0x00; PORTC = 0xFF; //Button Input
	DDRD = 0xFF; PORTD = 0x00; //LCD Data Lines
	/* Insert your solution below */
	TimerSet(50);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	LCD_DisplayString(1, '0');
	
	uc output = 0x00;
	uc mainSequence = 0x01;
	uc flashingLED = 0x08;
	uc flashingLEDTick = 0x00;
	uc buttonC1 = 0x00;
	uc pausePressed = 0x00;
	uc pausedSequence = 0x00;
	uc paused = 0x00;
	uc score = 0x00;

	while (1) {
		pausePressed = ~PINC & 0x01;
		buttonC1 = ~PINC & 0x02;
		if(pausePressed) {
			paused = !paused;
		}
		
		if(!paused) {
			if(buttonC1) {
				if(output == 0x02 || output == 0x0A) {
					score++;
				} else {
					if(score > 0) {
						score--;
					} else {
						//Nothing
					}
				}
				char *scoreString[8];
				sprintf(scoreString, "%d", score);
				unsigned char * scoreStringFinal = (unsigned char *)scoreString;
				LCD_DisplayString(1, scoreStringFinal);
			}
			
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
