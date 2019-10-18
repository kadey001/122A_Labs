/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: Init SPI and Update LED with data from Master
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

#define uc unsigned char

uc receivedData;

void SPI_ServantInit(void);

int main(void)
{
    SPI_ServantInit();
    
	uc select;
    uc ledPattern;
    while (1) 
    {
		select = ~PINB & 0x10;//Get SS
		if(!select) {//If the Select line is low then it is selected
			PORTA = recivedData;
    }
}

void SPI_ServantInit(void) {
	/* Set MISO output, all others input */
	DDRA = 0xFF; PORTA = 0x00;//LED Output
	DDRB = 0x40; 
	//PORTB = 0xBF;//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK

	/* Enable SPI */
	DDRB = (1<<DDB5);
	SPCR = (1<<SPE)|(1<<7);
}

ISR(SPI_STC_vect) {
	receivedData = SPDR;
}
