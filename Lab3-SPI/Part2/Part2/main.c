/*
 * Part2.c
 *
 * Created: 10/16/2019 4:16:54 PM
 * Author : Owner
 */ 

#include <avr/io.h>
#include "timer.h"
#include "keypad.h"

unsigned char LEDData;
unsigned char keypadInput;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
				 PORTB = 0x40;
	DDRC = 0x00; PORTC = 0xFF;
	
	TimerSet(10);
	TimerOn();
	
	SPI_MasterInit();
    
    while (1) 
    {
		keypadInput = GetKeypadKey();
		if(keypadInput >= '0' && keypadInput <= '9'){
			if(keypadInput > '4' || keypadInput == '0') {
				LEDData &= 0xF0;
				if(keypadInput == '5'){
					LEDData |= 0x01;
				}
				else if(keypadInput == '6'){
					LEDData |= 0x02;
				}
				else if(keypadInput == '7'){
					LEDData |= 0x03;
				}
				else if(keypadInput == '8'){
					LEDData |= 0x04;
				}
				else if(keypadInput == '9'){
					LEDData |= 0x05;
				}
				else {//if(keypadInput == '0'){
					LEDData |= 0x06;
				}
			}
			else if(keypadInput <= '4') {
				LEDData &= 0x0F;
				if(keypadInput == '1') {
					LEDData |= 0x10;
				}
				else if(keypadInput == '2'){
					LEDData |= 0x20;
				}
				else if(keypadInput == '3'){
					LEDData |= 0x30;
				}
				else { //if(keypadInput == '4'){
					LEDData |= 0x40;
				}
				
			}
		}
		
		SPI_MasterTransmit(LEDData);
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

void SPI_MasterInit(void) {
	
	DDRB =  (1 << DDB5) | (1 << DDB7) | (1 << DDB4);
	SPCR =  (1 << SPE)|(1 << MSTR)|(1 << SPR0);
	sei();
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;
	
	PORTB &= 0xEF;
	
	while(!(SPSR & (1 << SPIF)));
	
	PORTB |= 0x10;
}