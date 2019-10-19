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
enum LEDState2 {INIT2, OFF2, ON4} led_state2;
enum LEDState4 {INIT4, OFF4, ON4} led_state4;

void LED0_Init(){
	led0_state = INIT0;
}

void LED2_Init(){
	led2_state = INIT2;
}

void LED4_Init(){
	led4_state = INIT4;
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
		led_state = OFF0;
		break;
		case OFF0:
		led_state = ON0;
		break;
		case ON0:
		led_state = OFF0;
		break;
		default:
		led_state = INIT;
		break;
	}
}

void LedSecTask() {
	LEDS_Init();
	for(;;) {
		LEDS_Tick();
		vTaskDelay(1000);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,
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
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}