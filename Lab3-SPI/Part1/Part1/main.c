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
	DDRA = 0xFF; PORTA = 0x00;
				 PORTB = 0x40;
	TimerSet(1000);
	TimerOn();
	LEDValue = 0;
	SPI_MasterInit();
    while (1) 
    {
		SPI_MasterTransmit(LEDValue);
		
		LEDValue++; //will go back to zero if char overflows
		if(LEDValue > 200) {
			LEDValue = 0;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

void SPI_MasterInit(void) {
	
	DDRB =  (1 << DDB5) | (1 << DDB7) | (1 << DDB4);
	SPCR =  (1 << SPE)|(1 << MSTR)|(1 << SPR0);
	sei();//From <avr/interupt.h> in timer does the same thing as SREG |= (1 << 7);
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;
	
	PORTB &= 0xEF;
	
	while(!(SPSR & (1 << SPIF)));
	
	PORTB |= 0x10;
}
