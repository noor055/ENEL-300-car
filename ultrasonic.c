/*
 * File:   ultrasonic.c
 * Author: youss
 *
 * Created on February 25, 2025, 12:18 PM
 */


#include <avr/io.h>
#include <avr/interrupt.h>

double distance;
double ticks;
double time;

int main(void) {
    /* Replace with your application code */
    
    CCP = 0xd8;                             // Unlock protected I/O registers
    
    CLKCTRL.OSCHFCTRLA = 0x14;              // Clock set to 8MHz
    while (CLKCTRL.MCLKSTATUS & 0b00000001) {
        
    }
    
    PORTMUX.TCAROUTEA = 0x02;
    TCA0.SINGLE.CTRLA = (0x01)<<3 | 0x01;           // Divide by 16 and enable peripheral
    TCA0.SINGLE.CTRLB = 0x03;                       // Single slope PWM
    TCA0.SINGLE.CTRLB |= 0x01 << 4;                 // PA0 enabled as single slope PWM
    TCA0.SINGLE.PER = 100;                        // Frequency set to 1kHz
    TCA0.SINGLE.CMP0 = 5;                        // Set to 50% duty cycle
    
    PORTA.DIRSET = 0x01;
    PORTC.DIRSET = 0x01;

    EVSYS.CHANNEL0 = 0x41;          // PORTA Pin 1
    EVSYS.USERTCB2CAPT = 0x01;      // Choose channel 0 for input user
    TCB2.INTCTRL = 0x01;            // Capture interrupt enable
    TCB2.EVCTRL = 0x01;             // Enable input capture event
    TCB2.CTRLB = 0x04;              // Input capture PW measurement mode
    TCB2.CTRLA = 0x01;              // No clock division and TCB enabled
    
    
    sei();                          // Enable global interrupts

   
    
    while (1) {


        
    }
}

ISR(TCB2_INT_vect) 
{
    TCB2.INTFLAGS = 0x01;           // Clear interrupt flag
    ticks = TCB2.CCMP;
    time = ticks / 8000000;
    time /= 2;
    distance = time * 34300;

}