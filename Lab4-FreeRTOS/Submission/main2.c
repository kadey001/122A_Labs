/*
 * Part2.c
 *
 * Created: 10/18/2019 6:25:00 PM
 * Author : Andrew Lee (862039008) alee182@ucr.edu
 */ 
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum LEDState0 {INIT0, OFF0, ON0} led_state0;
enum LEDState2 {INIT2, OFF2, ON2} led_state2;
enum LEDState4 {INIT4, OFF4, ON4} led_state4;

void LED0_Init(){
	led_state0 = INIT0;
}

void LED2_Init(){
	led_state2 = INIT2;
}

void LED4_Init(){
	led_state4 = INIT4;
}

void LED0_Tick(){
	//Actions
	switch(led_state0){
		case INIT0:
		//do nothing
		break;
		case OFF0:
		PORTD &= 0xFE;
		break;
		case ON0:
		PORTD |= 0x01;
		break;
		default:
		//do nothing
		break;
	}
	//Transitions
	switch(led_state0){
		case INIT0:
		led_state0 = OFF0;
		break;
		case OFF0:
		led_state0 = ON0;
		break;
		case ON0:
		led_state0 = OFF0;
		break;
		default:
		led_state0 = INIT0;
		break;
	}
}

void LED2_Tick(){
	//Actions
	switch(led_state2){
		case INIT2:
		//do nothing
		break;
		case OFF2:
		PORTD &= 0xFD;
		break;
		case ON2:
		PORTD |= 0x02;
		break;
		default:
		//do nothing
		break;
	}
	//Transitions
	switch(led_state2){
		case INIT2:
		led_state2 = OFF2;
		break;
		case OFF2:
		led_state2 = ON2;
		break;
		case ON2:
		led_state2 = OFF2;
		break;
		default:
		led_state2 = INIT2;
		break;
	}
}

void LED4_Tick(){
	//Actions
	switch(led_state4){
		case INIT4:
		//do nothing
		break;
		case OFF4:
		PORTD &= 0xFB;
		break;
		case ON4:
		PORTD |= 0x04;
		break;
		default:
		//do nothing
		break;
	}
	//Transitions
	switch(led_state4){
		case INIT4:
		led_state4 = OFF4;
		break;
		case OFF4:
		led_state4 = ON4;
		break;
		case ON4:
		led_state4 = OFF4;
		break;
		default:
		led_state4 = INIT4;
		break;
	}
}


void Led0SecTask() {
	LED0_Init();
	for(;;) {
		LED0_Tick();
		vTaskDelay(500);
	}
}

void Led2SecTask() {
	LED2_Init();
	for(;;) {
		LED2_Tick();
		vTaskDelay(1000);
	}
}

void Led4SecTask() {
	LED4_Init();
	for(;;) {
		LED4_Tick();
		vTaskDelay(2500);
	}
}

void StartSecPulse0(unsigned portBASE_TYPE Priority) {
	xTaskCreate(Led0SecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

void StartSecPulse2(unsigned portBASE_TYPE Priority) {
	xTaskCreate(Led2SecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

void StartSecPulse4(unsigned portBASE_TYPE Priority) {
	xTaskCreate(Led4SecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

int main(void) {
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF;
	//Start Tasks
	StartSecPulse0(1);
	StartSecPulse2(2);
	StartSecPulse4(3);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}