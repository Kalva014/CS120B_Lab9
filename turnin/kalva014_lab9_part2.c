/*	Author: kennethalvarez
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: When a button is pressed, the tone is scaled on the speaker 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum OnOff_States {Start1, Wait1, OnPress, OnRelease, OffPress, OffRelease} OnOff_state;
enum Output_States {Start3, Wait3, ScaleUpPress, ScaleUpRelease, ScaleDownPress, ScaleDownRelease} Output_state;

unsigned char tmpA;
unsigned char on;
unsigned char prevNote;


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


void OnOff_Tick() {
	switch(OnOff_state) { //Transitions
		case Start1:
			OnOff_state = Wait1;
			break;
		case Wait1:
			if((tmpA == 0x01) && (on == 0)) {
				OnOff_state = OnPress;
			}
			else if((tmpA == 0x01) && (on == 1)) {
                        	OnOff_state = OffPress;
			}
			break;
		case OnPress:
			if(tmpA == 0x01) {
				OnOff_state = OnPress;
			}
			else {
				OnOff_state = OnRelease;
			}
			break;
		case OnRelease:
			OnOff_state = Wait1;
			break;
		case OffPress:
			if(tmpA == 0x01) {
                                OnOff_state = OffPress;
                        }
                        else {
                                OnOff_state = OffRelease;
                        }
			 break;
		case OffRelease:
			OnOff_state = Wait1;
			break;
		default:
			break;
	}

	switch(OnOff_state) { //Actions
                case Start1:
                        break;
                case Wait1:
                        break;
                case OnPress:
			PWM_on();
			on = 1;
                        break;
		case OnRelease:
			break;
                case OffPress:
			PWM_off();
			on = 0;
                        break;
		case OffRelease:
			break;
                default:
                        break;
        }
}


void Output_Tick() {
	switch(Output_state) { //Transitions
		case Start3:
			Output_state = Wait3;
			break;
		case Wait3:
			if((tmpA == 0x04) && (on == 1)) {
				Output_state = ScaleUpPress;
			}
			else if((tmpA == 0x02) && (on == 1)) {
				Output_state = ScaleDownPress;
			}
			else {
				Output_state = Wait3;
			} 
			break;
		case ScaleUpPress:
			if(tmpA == 0x04) {
				Output_state = ScaleUpPress;	
			}
			else {	
				Output_state = ScaleUpRelease;
			}
			break;
		case ScaleUpRelease:
			Output_state = Wait3;
			break;
		case ScaleDownPress:
			if(tmpA == 0x02) {
				Output_state = ScaleDownPress;
			}
			else {
				Output_state = ScaleDownRelease;
			}
		case ScaleDownRelease:
			Output_state = Wait3;
			break;
		default:
			break;
	}

	switch(Output_state) { //Actions
		case Start3:
                        break;
                case Wait3:
                        break;
		case ScaleUpPress:
			break;
                case ScaleUpRelease:
			if(prevNote == 1) {
				set_PWM(261.63);
                        	//prevNote = 1; //"C4"
			}
                	else if(prevNote == 2) {
				set_PWM(293.66);
 				//prevNote = 2; //"D4"
			}
			else if(prevNote == 3) {
				set_PWM(329.63);
				//prevNote = 3; //"E4"
			}
			else if(prevNote == 4) {
				set_PWM(349.23);
				//prevNote = 4; //"F4"
			}
			else if(prevNote == 5) {
                                set_PWM(392.00);
                                //prevNote = 5; //"G4"
                        }
			else if(prevNote == 6) {
                                set_PWM(440.00);
                                //prevNote = 6; //"A4"
                        }
			else if(prevNote == 7) {
                                set_PWM(493.88);
                                //prevNote = 7; //"B4"
                        }		
			else if(prevNote == 8) {
                                set_PWM(523.25);
                                //prevNote = 8; //"C5"
                        }

			if(prevNote == 8) {
				prevNote = prevNote;
			}
			else {
				prevNote = prevNote + 1;
			}
                        break;       
		case ScaleDownPress:
			break;     
         	case ScaleDownRelease:
			if(prevNote == 1) {
                                set_PWM(261.63);
                                //prevNote = 1; //"C4"
                        }
                        else if(prevNote == 2) {
                                set_PWM(293.66);
                                //prevNote = 2; //"D4"
                        }
                        else if(prevNote == 3) {
                                set_PWM(329.63);
                                //prevNote = 3; //"E4"
                        }
                        else if(prevNote == 4) {
                                set_PWM(349.23);
                                //prevNote = 4; //"F4"
                        }
                        else if(prevNote == 5) {
                                set_PWM(392.00);
                                //prevNote = 5; //"G4"
                        }
                        else if(prevNote == 6) {
                                set_PWM(440.00);
                                //prevNote = 6; //"A4"
                        }
                        else if(prevNote == 7) {
                                set_PWM(493.88);
                                //prevNote = 7; //"B4"
                        }
                        else if(prevNote == 8) {
                                set_PWM(523.25);
                                //prevNote = 8; //"C5"
                        }
               
			if(prevNote == 1) {
                                prevNote = prevNote;
			}
			else {
				prevNote = prevNote - 1;
                        }

                        break;
                default:
                        break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	OnOff_state = Start1;
	Output_state = Start3;

	on = 0; //Initialize outputs
	prevNote = 1; //"none"

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	
	OnOff_Tick();
	Output_Tick();
    }
    return 1;
}

