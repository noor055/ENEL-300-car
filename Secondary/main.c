/*
 * File:   main.c
 * Author: youss
 *
 */
#define F_CPU 8000000UL
#define TRUE 1
#define FALSE 0
#define ECHO_TIMEOUT 32000
#define DELAY 1000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "lcd.h"

/*
 The following variables are used to store and calculate
 the distance detected by the ultrasonic sensor in main file.
*/
double distance;
double ticks;
double time;
int distance_int;

/*
 This character array is used to store the string
 before being printed to the LCD display.
 */
char str[20];

int main(void) {
    
    CCP = 0xd8;
    CLKCTRL.OSCHFCTRLA = 0b00011100;
    while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
        ;
    }

   
          
    lcdInit();
    _delay_ms(50);
    
    PORTD.DIRSET = 0b00000010;
    PORTA.DIRCLR = 0b00000010;

    PORTD.OUT = 0b00000000;
   
    


    while (1) {
            
            // Sends a 10?s pulse to trigger ultrasonic transmitter
            PORTD.OUTCLR = 0b00000010;
            _delay_us(5);
            PORTD.OUTSET = 0b00000010;
            _delay_us(10);
            PORTD.OUTCLR = 0b00000010;
            
            uint16_t timeout = ECHO_TIMEOUT;
            while(!(PORTA.IN & 0b00000010) && --timeout); // Waits until pin for received signal goes high
            if(timeout == 0) { // Ensures code isn't stuck in loop in case of no returned signal
                _delay_ms(DELAY);
                continue;
            }
            
            // TCA0 is used to time duration from receiving pin high to low
            TCA0.SINGLE.CTRLA = 0x01;
            TCA0.SINGLE.CNT = 0;
            timeout = ECHO_TIMEOUT;
            while((PORTA.IN & 0b00000010) && --timeout); // Wait until pin goes low
            if(timeout == 0) { // Ensures code isn't stuck in loop in case of no low signal
                _delay_ms(DELAY);
                TCA0.SINGLE.CTRLA = 0;
                continue;
            }
            
            // Pulse duration in clock ticks is saved and counter is reset
            ticks = TCA0.SINGLE.CNT;
            TCA0.SINGLE.CNT = 0;
            TCA0.SINGLE.CTRLA = 0;
            
            // Distance is calculated from pulse duration in clock ticks
            time = ticks / 16000000;
            time /= 2;
            distance = time * 34300;
            distance_int = (int)distance;
            
            
            sprintf(str, "%d", distance_int); // Converts integer distance value to string with same digits
            write(str); // Prints distance to LCD
            _delay_ms(DELAY); // Delays are used for LCD to run more smoothly

}
   }
