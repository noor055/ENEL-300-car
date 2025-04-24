/*
 * File:   metaldetector.c
 * Author: youss
 *
 * Created on April 22, 2025, 8:44 AM
 */


#include <avr/io.h>
#include "metaldetector.h"

void detectorInit(double vthreshold) {
    
    // Lines 16 - 29 is ADC initialization code taken from an ENCM369 lab
    
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
    
    // Next 3 lines enable input and output pins for detector voltage and buzzer
    PORTA.DIRSET = 0b00000001;
    PORTA.OUTCLR = 0b00000001; 
    PORTD.DIRCLR = 0b00000100; 

    
    // vthreshold contains the average detected voltage with no metal present
    vthreshold = 0;

    // An average calibration voltage is found from a number of samples
    for(double i = 0; i < SAMPLE_SIZE; i++) {
        vthreshold += ADC0.RES;
    }
    vthreshold /= SAMPLE_SIZE;
}
