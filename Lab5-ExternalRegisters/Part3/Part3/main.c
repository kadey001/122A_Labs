/*
 * Part3.c
 *
 * Created: 10/23/2019 5:09:04 PM
 * Author : Owner
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

void transmit_dataC(uc data) {
	PORTC |= SRCLR;
	PORTC &= 0xFD;
	uc sendBit = 0x00;
	for(uc i = 0; i < 8; ++i) {
		//SRCLR set to 1 allowing for data to be sent
		//Also clears SRCLK in preparation for sending data
		PORTC &= 0xFB;
		//SER = next bit of data to be sent
		sendBit = 0x01 & data;
		//Shift data over
		data = data >> 1;
		//Set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		if(sendBit) {
			PORTC |= 0x01;
		}
		else {
			PORTC &= 0xFE;
		}
		
		PORTC |= (SRCLK);
	}
	//Set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
	PORTC |= (SRCLR | RCLK);
	//Clear all lines in preparation of a new transmission
	PORTC = 0x00;
}

void transmit_dataD(uc data) {
	PORTD |= SRCLR;
	PORTD &= 0xFD;
	uc sendBit = 0x00;
	for(uc i = 0; i < 8; ++i) {
		//SRCLR set to 1 allowing for data to be sent
		//Also clears SRCLK in preparation for sending data
		PORTD &= 0xFB;
		//SER = next bit of data to be sent
		sendBit = 0x01 & data;
		//Shift data over
		data = data >> 1;
		//Set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		if(sendBit) {
			PORTD |= 0x01;
		}
		else {
			PORTD &= 0xFE;
		}
		
		PORTD |= (SRCLK);
	}
	//Set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
	PORTD |= (SRCLR | RCLK);
	//Clear all lines in preparation of a new transmission
	PORTD = 0x00;
}

