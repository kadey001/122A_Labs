/*
 * Part1.c
 *
 * Created: 10/11/2019 7:06:41 PM
 * Author : Owner
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xBF; PORTB = 0x40;
	DDRC = 0x00; PORTC = 0xFF;
	DDRB = 0x00; PORTD = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

