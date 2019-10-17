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
char SPI_ServantReceive(void);

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
    SPI_ServantInit();
    
	uc select;
    uc ledPattern;
    while (1) 
    {
		select = ~PINB & 0x10;//Get SS
		if(!select) {//If the Select line is low then it is selected
			ledPattern = receivedData;
		}
		//ledPattern = SPI_ServantReceive();
        PORTA = ledPattern;
    }
}

void SPI_ServantInit(void) {
    /* Set MISO output, all others input */
  /*  DDRB = 0x40;*/ PORTB = 0xBF;//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK
	
	DDRB = (1 << DDB5);
    /* Enable SPI */
    SPCR = (1<<SPE);
}

char SPI_ServantReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR & (1<<SPIF)));
    /* Return Data Register */
    return SPDR;
}

ISR(SPI_STC_vect) {
	receivedData = SPDR;
}

