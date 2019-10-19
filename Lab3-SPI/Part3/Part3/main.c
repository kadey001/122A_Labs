/*
 * Part3.c
 *
 * Created: 10/16/2019 5:05:53 PM
 * Author : Owner
 */ 

#include <avr/io.h>
#include "timer.h"
#include "keypad.h"
#include "lcd.h"

static unsigned char PATTERN[7] = "Ptrn: ";
static unsigned char SPEED[6] = "Spd: ";
static unsigned char CONTROLLER[6] = "uC: 1";

unsigned char LCDOutput[6] = "asdf ";
unsigned char LEDData;
unsigned char keypadInput;
unsigned char upper;
unsigned char lower;

unsigned char test;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
				 PORTB = 0x40;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(500);
	TimerOn();
	LCD_init();
	
	SPI_MasterInit();
	
	while (1)
	{
		keypadInput = GetKeypadKey();
		if(keypadInput != '\0') {
			switch(keypadInput) {
				//Speeds
				case '1': LEDData &= 0xF0; LEDData |= 0x01; break;
				case '2': LEDData &= 0xF0; LEDData |= 0x02; break;
				case '3': LEDData &= 0xF0; LEDData |= 0x03; break;
				case '4': LEDData &= 0xF0; LEDData |= 0x04; break;
				case '5': LEDData &= 0xF0; LEDData |= 0x05; break;
				case '6': LEDData &= 0xF0; LEDData |= 0x06; break;
				
				//Pattern
				case 'A': LEDData &= 0x0F; LEDData |= 0x10; break;
				case 'B': LEDData &= 0x0F; LEDData |= 0x20; break;
				case 'C': LEDData &= 0x0F; LEDData |= 0x30; break;
				case 'D': LEDData &= 0x0F; LEDData |= 0x40; break;
				
				default: //Nothing
			}
			
			upper = ((LEDData & 0xF0) >> 4);
			lower = LEDData & 0x0F;
		
			LCD_ClearScreen();
			LCD_DisplayString(1, PATTERN);
			LCD_WriteData(upper + '0');
			LCD_DisplayString(9, SPEED);
			LCD_WriteData(lower + '0');
			LCD_DisplayString(17, CONTROLLER);
		
			SPI_MasterTransmit(LEDData);
		}
		
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
