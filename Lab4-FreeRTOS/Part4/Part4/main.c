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
enum LEDState {INIT, RIGHT, LEFT} led_state;

void LEDS_Init(){
	led_state = INIT;
}

unsigned char currentLED;
unsigned char switched;

void LEDS_Tick(){
	//Actions
	switched = 0;
	switch(led_state){
		case INIT:
			currentLED = 0x80;
            PORTD = currentLED;
            break;
		case RIGHT:
			currentLED = currentLED >> 1;
			PORTD = currentLED;
			break;
		case LEFT:
			currentLED = currentLED << 1;
			PORTD = currentLED;
			break;
		default:
            PORTD = 0;
            break;
	}
	//Transitions
	switch(led_state){
		case INIT:
			led_state = RIGHT;
            break;
		case RIGHT:
			if(currentLED == 0x01) {
				led_state = LEFT;
			}
			break;
		case LEFT:
			if(currentLED == 0x80) {
				led_state = RIGHT;
			}
			break;
		default:
			led_state = INIT;
            break;
	}
}

unsigned char button;

void buttonTick() {
	button = ~PINA & 0x01;
	if(button && currentLED != 0x80 && currentLED != 0x01 && switched == 0) {
		switch(led_state) {
			case RIGHT: 
				led_state = LEFT;
				break;
			case LEFT: 
				led_state = RIGHT;
				break;
			default: 
				break;
		}
		switched = 1;
	}
	
}

void LedSecTask() {
    LEDS_Init();
    for(;;) { 	
        LEDS_Tick();
        vTaskDelay(250); 
    } 
}

void directionButtonTask() {
	for(;;) {
		buttonTick();
		vTaskDelay(25);
	}
}

void StartButtonSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(directionButtonTask, 
		(signed portCHAR *)"directionButtonTask", 
		configMINIMAL_STACK_SIZE, 
		NULL, 
		Priority, 
		NULL );
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
    StartButtonSecPulse(2);
    //RunSchedular 
    vTaskStartScheduler(); 
    return 0; 
}
