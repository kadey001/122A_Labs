/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: Update speed and patterns from Master
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "timer.h"

#define uc unsigned char
#define ul unsigned long

uc pattern;
uc pattern3 = 0x80;

void SPI_ServantInit(void);
char SPI_ServantReceive(void);

//FSMs
enum phase {phase1, phase2};

int SMTick1(int state) {
	if(pattern == 1) {
		switch(state) {
			case phase1:
				PORTA = 0xF0;
				state = phase2;
				break;
			case phase2:
				PORTA = 0x0F;
				state = phase1;
				break;
			default:
				state = phase1;
				break;
		}
	}
	
	return state;
}

int SMTick2(int state) {
	if(pattern == 2) {
		switch(state) {
			case phase1:
				PORTA = 0xAA;
				state = phase2;
				break;
			case phase2:
				PORTA = 0x55;
				state = phase1;
				break;
			default:
				state = phase1;
				break;
		}
	}
	
	return state;
}

int SMTick3(int state) {
	if(pattern == 3) {
		switch(state) {
			case phase1:
				if(pattern3 == 0x02) {
					PORTA = 0x01;
					state = phase2;
				} else {
					pattern3 >> 1;
					PORTA = pattern3;
				}
				break;
			case phase2:
				if(pattern3 == 0x40) {
					PORTA = 0x80;
					state = phase1;
				} else {
					pattern3 << 1;
					PORTA = pattern3;
				}
				break;
			default:
				state = phase1;
				break;
		}
	}
	
	return state;
}

int SMTick4(int state) {
	if(pattern == 4) {
		switch(state) {
			case phase1:
				PORTA = 0x3C;
				state = phase2;
				break;
			case phase2:
				PORTA = 0xC3;
				state = phase1;
				break;
			default:
				state = phase1;
				break;
		}
	}
	
	return state;
}

ul updateSpeed(uc speed) {
	switch(speed) {
		case 0x01:
			return ;
		case 0x02:
			return ;
		default:
			return 50;//50ms
	}
}

int main(void)
{
	SPI_ServantInit();

	//Individual Periods
	//ul SMTick1_calc = 100;

	//Calculating GCD
	ul GCD = 50;
	//GCD = SMTick1_calc;

	//Task Periods
	ul period = 1;
	//ul SMTick1_period = SMTick1_calc/GCD;

	//Array of Tasks
	static task task1;
	static task task2;
	static task task3;
	static task task4;

	task *tasks[] = { &task1, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*)
	
	//Task 1
	task1.state = -1;//Init state
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;
	
	//Task 2
	task2.state = -1;//Init state
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &SMTick2;
	
	//Task 3
	task3.state = -1;//Init state
	task3.period = SMTick3_period;
	task3.elapsedTime = SMTick3_period;
	task3.TickFct = &SMTick3;
	
	//Task 1
	task4.state = -1;//Init state
	task4.period = SMTick4_period;
	task4.elapsedTime = SMTick4_period;
	task4.TickFct = &SMTick4;

	//Initilization
	TimerSet(GCD);
	TimerOn();

	uc receivedData;
	uc speed;
	uc currentPeriod;
	unsigned short i;//Iterator
	while (1)
	{
		receivedData = SPI_ServantReceive();
		speed = receivedData & 0x0F;
		receivedData >> 4;
		pattern = receivedData & 0x0F;//Gets pattern on lower 4 bits
		currentPeriod = updateSpeed(speed)/GCD;

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
	DDRA = 0xFF; PORTA = 0x00;//LED Output
	//DDRB = 0x40; 
	PORTB = 0xBF;//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK

	/* Enable SPI */
	DDRB = (1<<DDB5);
	SPCR = (1<<SPE);
}

char SPI_ServantReceive(void) {
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}
