/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: Using shift registers to increment and decrement
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define uc unsigned char
#define ul unsigned long
#define us unsigned short

#define SER 0x01
#define RCLK 0x02
#define SRCLK 0x04
#define SRCLR 0x08

void transmit_data(uc);
void clear_data(void);

uc shiftValue = 0x00;
uc incrementButton;
uc decrementButton;
uc updateShiftRegister;

//State Machines
enum buttonState { CHECK };

int SMTick1(int state) {
    switch(state) {
        case CHECK:
            incrementButton = ~PINA & 0x01;
            decrementButton = ~PINA & 0x02;
            if(incrementButton && decrementButton) {
                shiftValue = 0x00;
                updateShiftRegister = 1;
            }
            else if(incrementButton && shiftValue < 0xFF) {
                shiftValue += 1;
                updateShiftRegister = 1;
            }
            else if(decrementButton && shiftValue > 0x00) {
                shiftValue -= 1;
                updateShiftRegister = 1;
            }
            break;
    }
}

enum transmition { WAIT, TRANSMIT }

//Transmition
int SMTick2(int state) {
    //Action
    switch(state) {
        case WAIT: break;
        case TRANSMIT: transmit_data(shiftValue); break;
        default: //Nothing
    }
    //State
    switch(state) {
        case WAIT:
            if(updateShiftRegister == 1) {
                state = TRANSMIT;
                updateShiftRegister = 0;
            }
            break;
        case TRANSMIT:
            state = WAIT;
            break;
        default: 
            state = WAIT;
    }
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    //Tasks Period
	ul int SMTick1_calc = 50;
	ul int SMTick2_calc = 100;
	//ul int SMTick3_calc = 100;
	
	//Calculating GCD
	GCD = findGCD(SMTick1_calc, SMTick2_calc);
	//GCD = findGCD(GCD, SMTick3_calc);
	
	//Recalculate GCD periods for scheduler
	ul int SMTick1_period = SMTick1_calc/GCD;
	ul int SMTick2_period = SMTick2_calc/GCD;
	//ul int SMTick3_period = SMTick3_calc/GCD;
	
	//Array of Tasks
	static task task1;
	static task task2;
	//static task task3;
	task *tasks[] = { &task1, &task2 };
	const us numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1; //Init
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;

    //Task 2
	task2.state = -1; //Init
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &SMTick2;

    TimerSet(GCD);
    TimerOn();

    shiftValue = 0x00;
    us i;//Loop iterator
    while (1) 
    {
        //Scheduler code
		for (i = 0; i < numTasks; i++) {
			//Task ready to tick
			if (tasks[i]->elapsedTime >= tasks[i]->period) {
				//Setup next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				//Reset elapsed time for next tick
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime +=1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

void transmit_data(uc data) {
    for(uc i = 0; i < 8; ++i) {
        //SRCLR set to 1 allowing for data to be sent
        //Also clears SRCLK in preparation for sending data
        PORTB = SRCLR;
        //SER = next bit of data to be sent
        uc sendBit = (SRCLR | (SER & data));
        //Shift data over
        data = data >> 1;
        //Set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTB = (sendBit | SRCLK);
    }
    //Set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
	PORTB = (SRCLR | RCLK);
	//Clear all lines in preparation of a new transmission
	PORTB = 0x00;
}

void clear_data() {
	PORTB = SRCLK;
	PORTB = RCLK;
}