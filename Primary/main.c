/*
 * File:   main.c
 * Author: youss
 *
 * Created on April 1, 2025, 12:54 PM
 */


#include <avr/io.h>
#include "metaldetector.h"

int main(void) {
    /* Replace with your application code */
    CCP = 0xd8;
    CLKCTRL.OSCHFCTRLA = 0b00011100;
    while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
        ;
    }

    // Defining a variable for calibration voltage and starting metal detector initialization
    double vthreshold = 0;
    detectorInit(vthreshold);    
    
    // Enabling input pins for RF receiver and outputs for motor driver controls + headlight LED
    PORTA.DIRSET = 0b11111001;
    PORTD.DIRCLR = 0b01111000;
    PORTD.DIRSET = 0b10000000;
    
    PORTA.OUT = 0b00000000;
    PORTD.OUT = 0b00000000;
    
    while(1) {
        
        if((ADC0.RES < vthreshold - 20)) {
            PORTA.OUTSET = 0b10000000; // Turns on buzzer
            ADC0.INTFLAGS |= 0b00000001; // Clears Interrupt flag for ADC to start next check
        } else {
            // Will keep buzzer off if voltage is below threshold
            PORTA.OUTCLR = 0b10000000;
            ADC0.INTFLAGS |= 0b00000001;
        }
        
        
        if((PORTD.IN & 0b00001000) && (PORTD.IN & 0b00010000)) { // Toggle on headlight if two buttons are pressed
            PORTD.OUTTGL = 0b10000000;
        } else { // Forward or reverse bias certain motor pairs based on input command
            if(PORTD.IN & 0b00001000) {
                PORTA.OUTCLR = 0b01111000;
                PORTA.OUTSET = 0b00110000;
            } else if(PORTD.IN & 0b00010000) {
                PORTA.OUTCLR = 0b01111000;
                PORTA.OUTSET = 0b01001000;
            } else if(PORTD.IN & 0b00100000) {
                PORTA.OUTCLR = 0b01111000;
                PORTA.OUTSET = 0b01010000;
            } else if(PORTD.IN & 0b01000000) {
                PORTA.OUTCLR = 0b01111000;
                PORTA.OUTSET = 0b00101000;
            } else PORTA.OUTCLR = 0b01111000;
        
        }
    }
}
