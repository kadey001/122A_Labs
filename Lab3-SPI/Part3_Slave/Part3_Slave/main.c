/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #3  Exercise #3
 *	Exercise Description: Updating speed from Master
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "bit.h"
#include "timer.h"
#include "scheduler.h"

#define uc unsigned char
#define ul unsigned long

uc receivedData;
uc pattern;
uc pattern3 = 0x80;

void SPI_ServantInit(void);
ul updateSpeed(uc);

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
					pattern3 = 0x01;
					state = phase2;
				} else {
					pattern3 = pattern3 >> 1;
				}
				PORTA = pattern3;
				break;
			case phase2:
				if(pattern3 == 0x40) {
					pattern3 = 0x80;
					state = phase1;
				} else {
					pattern3 = pattern3 << 1;
				}
				PORTA = pattern3;
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

int main(void)
{
	SPI_ServantInit();

	//Individual Periods
	//ul SMTick1_calc = 100;

	//Calculating GCD
	ul GCD = 500;
	//GCD = SMTick1_calc;

	//Task Periods
	ul period = 1;
	//ul period = SMTick1_calc/GCD;

	//Array of Tasks
	static task task1;
	static task task2;
	static task task3;
	static task task4;

	task *tasks[] = { &task1, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1;//Init state
	task1.period = period;
	task1.elapsedTime = period;
	task1.TickFct = &SMTick1;
	
	//Task 2
	task2.state = -1;//Init state
	task2.period = period;
	task2.elapsedTime = period;
	task2.TickFct = &SMTick2;
	
	//Task 3
	task3.state = -1;//Init state
	task3.period = period;
	task3.elapsedTime = period;
	task3.TickFct = &SMTick3;
	
	//Task 1
	task4.state = -1;//Init state
	task4.period = period;
	task4.elapsedTime = period;
	task4.TickFct = &SMTick4;

	//Initilization
	TimerSet(GCD);
	TimerOn();
	
	uc speed;//Speed setting for timer
	unsigned short i;//Iterator
	
	while (1)
	{
		speed = receivedData & 0x0F;
		pattern = receivedData & 0xF0;//Gets pattern on upper 4 bits
		pattern = pattern >> 4;//Shifts pattern to lower 4 bits
		TimerSet(updateSpeed(speed));

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
	//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK
	DDRB = (1<<DDB5);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<7);
	sei();
}

ISR(SPI_STC_vect) {
	receivedData = SPDR;
}

ul updateSpeed(uc selectedSpeed) {
	switch(selectedSpeed) {
		case 1: return 2000;
		case 2: return 1000;
		case 3: return 500;
		case 4: return 250;
		case 5: return 100;
		case 6: return 50;
		default: return 100;
	}
}
