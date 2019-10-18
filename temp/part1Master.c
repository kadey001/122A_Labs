/*
 * Part1.c
 *
 * Created: 10/11/2019 7:06:41 PM
 * Author : Owner
 */ 

#include <avr/io.h>
#include "timer.h"

unsigned char LEDValue;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

int main(void)
{
	TimerSet(1000);
	TimerOn();
	LEDValue = 0;
	SPI_MasterInit();
    while (1) 
    {
		SPI_MasterTransmit(LEDValue);
		
		LEDValue++; //will go back to zero if char overflows
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

void SPI_MasterInit(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xBF; PORTB = 0x40;
	DDRC = 0x00; PORTC = 0xFF;
	DDRB = 0x00; PORTD = 0xFF;
	
	SPCR =  (1 << SPE)|(1 << MSTR)|(1 << SPR0);
	SREG |= (1 << 7);
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;
	
	PORTB |= 0x10;
	
	while(!(SPSR & (1 << SPIF))) {
		;
	}
	
	PORTB &= 0xEF;
}
