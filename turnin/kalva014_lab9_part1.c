/*	Author: kennethalvarez
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: When a button is pressed, the tone mapped to it is generated on the speaker. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, C4Pressed, D4Pressed, E4Pressed} state;
unsigned char tmpA;


void Tick() {
	switch(state) { //Transitions
		case Start:
			state = Wait;
			break;
		case Wait:
			if(tmpA == 0x01) {
				PWM_on();
				state = C4Pressed;
			}
			else if(tmpA == 0x02) {
				PWM_on();
				state = D4Pressed;
			}
			else if(tmpA == 0x04) {
				PWM_on();
				state = E4Pressed;
			}
			else {
				state = Wait;
			} 
			break;
		case C4Pressed:
			if(tmpA == 0x01) {
				state = C4Pressed;
			}
			else {
				state = Wait;
			}
			break;
		case D4Pressed:
			if(tmpA == 0x02) {
				state = D4Pressed;
			}
			else {
				state = Wait;
			}
			break;
		case E4Pressed:
			if(tmpA == 0x04) {
                                state = E4Pressed;
                        }
                        else {
                                state = Wait;
                        }  
			break;
		default:
			break;
	}

	switch(state) { //Actions
		case Start:
                        break;
                case Wait:
			PWM_off();
                        break;
                case C4Pressed:
			set_PWM(261.63);
			//PWM_on();
                        break;
                case D4Pressed:
			set_PWM(293.66);
                        //PWM_on();
                        break;
                case E4Pressed:
			set_PWM(329.63);
                        //PWM_on();
                        break;
                default:
                        break;
	}
}


void set_PWM(double frequency) {
	static double current_frequency;
	
	if(frequency != current_frequency) {
		if(!frequency) {
			TCCR3B &= 0x08;
		}
		else {
			TCCR3B |= 0x03;
		}

		if(frequency < 0.954) {
			OCR3A = 0xFFFF;
		}
		else if(frequency > 31250) {
			OCR3A = 0x0000;
		}
		else {
			OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		}
	
		TCNT3 = 0;
		current_frequency = frequency;
	}
}


void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}


void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	
	Tick();
    }
    return 1;
}
