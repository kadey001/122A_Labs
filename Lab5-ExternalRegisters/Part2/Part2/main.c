/*	Author: Kelton
 *  Partner(s) Name: Andrew Lee
 *	Lab Section: 22
 *	Assignment: Lab #5  Exercise #2
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

uc shiftValue;
uc cycleState;
uc toggleSystem;
uc updateShiftRegister;
uc cycleButtonUp;
uc cycleButtonDown;

//State Machines
enum buttonState { CHECK, CYCLEUP, CYCLEDOWN, TOGGLE };

int SMTick1(int state) {
    switch(state) {
        case CHECK:
            cycleButtonUp = ~PINA & 0x01;
            cycleButtonDown = ~PINA & 0x02;
            break;
        case CYCLEUP: 
            if(cycleState < 3) {
                cycleState += 1;
            }
            break;
        case CYCLEDOWN: 
            if(cycleState > 0) {
                cycleState -= 1;
            }
            break;
        case TOGGLE: 
            toggleSystem = !toggleSystem;
            break;
        default: state = CHECK;
    }
    switch(state) {
        case CHECK:
            if(cycleButtonUp && cycleButtonDown) {
                state = TOGGLE;
            }
            else if(cycleButtonUp) {
                state = CYCLEUP
            }
            else if(cycleButtonDown) {
                state = CYCLEDOWN;
            }
            break;
        case CYCLEUP:
        case CYCLEDOWN:
        case TOGGLE: state = CHECK;
        default: state = CHECK;
    }
}

enum transmition { OFF, CYCLE0, CYCLE1, CYCLE2 }

//Transmition
int SMTick2(int state) {
    //Action
    switch(state) {
        case OFF:
            transmit_data(0x00);//Turn off all LEDs
            break;
        case CYCLE0: 
            if(shiftValue < 0xFF) {
                shiftValue = shiftValue << 1;
            } else {
                shiftValue = 0;
            }
            transmit_data(shiftValue);
            break;
        case CYCLE1: 
            if(shiftValue > 0x00) {
                shiftValue = shiftValue >> 1;
            } else {
                shiftValue = 0;
            }
            transmit_data(shiftValue);
            break;
        case CYCLE2: 
            if(shiftValue == 0xF0) {
                shiftValue = 0x0F;
            } else {
                shiftValue = 0xF0;
            }
            transmit_data(shiftValue);
            break;
        default: //Nothing
    }
    //State
    switch(state) {
        case OFF: 
            if(!toggleSystem) {
                switch(cycleState) {
                    case 0: state = CYCLE0; break;
                    case 1: state = CYCLE1; break;
                    case 2: state = CYCLE2; break;
                    default: state = CYCLE0; break;
                }
            }
            break;
        case CYCLE0:
        case CYCLE1:
        case CYCLE2: 
            if(toggleSystem) {
                state = OFF;
                break;
            }
            switch(cycleState) {
                case 0: state = CYCLE0; break;
                case 1: state = CYCLE1; break;
                case 2: state = CYCLE2; break;
                default: state = CYCLE0; break;
            }
            break;
        default: state = OFF;
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
	ul GCD = findGCD(SMTick1_calc, SMTick2_calc);
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