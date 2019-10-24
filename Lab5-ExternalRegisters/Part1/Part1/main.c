/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: Using shift registers to increment and decrement
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define uc unsigned char

#define SER 0x01
#define RCLK 0x02
#define SRCLK 0x04
#define SRCLR 0x08

void transmit_data(uc);
void clear_data(void);

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    
	uc shiftValue = 0x00;
    uc incrementButton = 0x00;
    uc decrementButton = 0x00;

    TimerSet(50);
	TimerOn();

    while (1) 
    {
		incrementButton = ~PINA & 0x01;//Increment Button
        decrementButton = ~PINA & 0x02;//Decrement Button

        if(incrementButton && shiftValue) {
            shiftValue = 0x00;
            transmit_data(shiftValue);
        }
		else if(incrementButton && shiftValue < 0xFF) {
			shiftValue += 1;
            transmit_data(shiftValue);
		}
        else if(decrementButton && shiftValue > 0x00) {
            shiftValue -= 1;
            transmit_data(shiftValue);
        }

        while(!TimerFlag);
        TimerFlag = 0;
    }
}

void transmit_data(uc data) {
    for(uc i = 0; i < 8; ++i) {
        //SRCLR set to 1 allowing for data to be sent
        //Also clears SRCLK in preparation for sending data
        PORTB = SRCLR;
        //SER = next bit of data to be sent
        uc sendBit = (SRCLR | (SER & data));
        //Shift data over
        data = data >> 1;
        //Set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTB = (sendBit | SRCLK);
    }
    //Set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
	PORTB = (SRCLR | RCLK);
	//Clear all lines in preparation of a new transmission
	PORTB = 0x00;
}

void clear_data() {
	PORTB = SRCLK;
	PORTB = RCLK;
}