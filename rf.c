/*
 * File:   rf.c
 * Author: youss
 *
 * Created on April 1, 2025, 12:54 PM
 */


#include <avr/io.h>

int main(void) {
    /* Replace with your application code */
    
    PORTA.DIRSET = 0b00111100;
    PORTD.DIRSET = 0b10000000;
    PORTD.DIRCLR = 0b01111000;
    
    PORTA.OUT = 0b00000000;
    PORTD.OUT = 0b00000000;
    
    while(1) {
        if((PORTD.IN & 0b00001000) && (PORTD.IN & 0b00010000)) {
            PORTD.OUTTGL = 0b10000000;
        } else {
            if(PORTD.IN & 0b00001000) {
                PORTA.OUTCLR = 0b00111100;
                PORTA.OUTSET = 0b00010100;
            } else if(PORTD.IN & 0b00010000) {
                PORTA.OUTCLR = 0b00111100;
                PORTA.OUTSET = 0b00101000;
            } else if(PORTD.IN & 0b00100000) {
                PORTA.OUTCLR = 0b00111100;
                PORTA.OUTSET = 0b00000100;
            } else if(PORTD.IN & 0b01000000) {
                PORTA.OUTCLR = 0b00111100;
                PORTA.OUTSET = 0b00010000;
            } else PORTA.OUTCLR = 0b00111100;
        
        }
    }
}
