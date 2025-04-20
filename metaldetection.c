/*
 * File:   adc.c
 * Author: gmessier
 *
 * Created on February 25, 2022, 3:14 PM
 */


#include <avr/io.h>
int main(void) {
// Enable global interrupts.
    SREG = 0b10000000;
// Set the ADC reference level to VDD.
    VREF.ADC0REF = 0b10000101; //Ref is VDD = 5V
// Enable the ADC interrupt.
    ADC0.INTCTRL = 0b00000001;
// Select PD2 (AIN2) as the ADC input.
    ADC0.MUXPOS = 0x02;
// Select minimum clock divide.
    ADC0.CTRLC = 0x00;
// Select single ended mode, 12 bit resolution and free-running modes.
    ADC0.CTRLA = 0b00000011;
// Start conversion.
    ADC0.COMMAND = 0x01;
    PORTA.DIRSET = 0b00000001; //Sets PA 0
    PORTA.OUT = 0b00000000; // Initializes buzzer off
    PORTD.DIRCLR = 0b00000100; // sets PD2 to input
    int i = 0;
    int vthreshold = 0;
    while(i<5000){
        if(i == 0){
            vthreshold = ADC0.RES;
        }
        else{
            vthreshold = (vthreshold)(i) + ADC0.RES;
            vthreshold = vthreshold/(i+1);
        }
        i++;
    }
    
    while (1) {
        if ((ADC0.RES > vthreshold - 50))
            {
            PORTA.OUT = 0b00000001; // Turns on buzzer
            ADC0.INTFLAGS |= 0b00000001; // Clears Interrupt flag for ADC to start next check
            }
        else
            {
// Will keep buzzer off if voltage is below threshold
            PORTA.OUT = 0b00000000;
            ADC0.INTFLAGS |= 0b00000001;
            }
}
}