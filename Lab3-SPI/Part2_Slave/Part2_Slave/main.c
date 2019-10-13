/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: Update LCD with data from Master
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "lcd.h"
#include "bit.h"
#include "timer.h"

#define uc unsigned char
#define ul unsigned long

void SPI_ServantInit(void);
char SPI_ServantReceive(void);

//FSMs
enum task1State {};

int SMTick1(int state) {

}

int main(void)
{
	SPI_ServantInit();

	//Individual Periods
	ul SMTick1_calc = 100;

	//Calculating GCD
	ul GCD;
	GCD = SMTick1_calc;

	//Task Periods
	ul SMTick1_period = SMTick1_calc/GCD;

	//Array of Tasks
	static task task1;

	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*)
	
	//Task 1
	task1.state = -1;//Init state
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;

	//Initilization
	LCD_init();
	TimerSet(GCD);
	TimerOn();

	uc receivedData;
	uc pattern;
	uc speed;
	while (1)
	{
		receivedData = SPI_ServantReceive();
		speed = receivedData & 0x0F;
		receivedData >> 4;
		pattern = receivedData & 0x0F;

		//Scheduler
		for(i = 0; i < numTasks; i++) {
			//Task ready to tick
			if(tasks[i]->elapsedTime >= tasks[i]->period) {
				//Setup next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				//Reset elapsed time
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

void SPI_ServantInit(void) {
	/* Set MISO output, all others input */
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x40; PORTB = 0xBF;//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK

	/* Enable SPI */
	DDR_SPI = (1<<DD_MISO);
	SPCR = (1<<SPE);
}

char SPI_ServantReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}

