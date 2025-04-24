/*
 * File:   lcd.c
 * Author: youss
 *
 * Created on April 21, 2025, 7:53 PM
 */

// This needs to be equal to the CPU clock for the delay functions to work.
// It also needs to be defined before the include statements.
#define F_CPU 16000000UL  

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"



// Lines 22 - 87 are functions written by Geoffrey Messier for LCD tutorial

/* 
 * This function writes a single byte of data over the I2C bus.
 *   To follow along with this function, read the AVR data sheet sections
 *   29.2, 29.3.1, 29.3.2.1, 29.3.2.2.1 and the entries in 29.4 and 29.5
 *   that explain the SFRs I use below.
 */
void I2CWrite(uint8_t address, uint8_t data) {

  // Set secondary device address and trigger the bus start condition.
  TWI0.MADDR = address << 1;
  
    
  // Wait until the address has been transmitted out of the data register.
  while( !(TWI0.MSTATUS & 0b01000000) ){
    ;
  }
  
  // Write the data byte.
  TWI0.MDATA = data;
    
  // Wait until the address has been transmitted out of the data register.
  while( !(TWI0.MSTATUS & 0b01000000) ){
    ;
  }
  
  // Send stop condition
  TWI0.MCTRLB = 0x03;
  
  /*
   * Really not sure why I need this delay.  Without it, the display behaves
   * randomly, sometimes working sometimes not.  That *usually* means something
   * is being driven too fast but the data sheets say I'm easily within timing
   * requirements.  Possibly there's a more efficient way to write to the LCD
   * RAM that isn't as fussy but I couldn't find it. 
   */
  _delay_ms(1);
}


/*
 * This function sends a single nibble of data to the LCD in 4 bit mode,
 * sets the RS, R/W lines and turns the display back light on/off.
 *   To follow this section, have a look at the SPLC780D data sheet sections
 *   5.2 to get an idea of some LCD commands and section 6.5.6 to see the
 *   timing requirements for the LCD lines.
 */
void LCDWrite(uint8_t nibble, uint8_t rs, uint8_t rw, uint8_t ledOn){
  
  // Prepare the 8 bits that will be sent over I2C.  The wiring diagram on
  // page 2 of the LCD1602 tutorial shows how these bits are mapped to the 
  // LCD control lines.
  uint8_t data = nibble << 4;
  data |= ledOn << 3;
  data |= rw << 1;
  data |= rs;
  
  // Set the data lines, RS and RW.
  I2CWrite(DISPLAY_ADDR,data);
  
  // Pulse E to trigger the SPLC780D to read the data nibble.
  data |= 0b00000100;
  I2CWrite(DISPLAY_ADDR,data);  
  data &= 0b11111011;
  I2CWrite(DISPLAY_ADDR,data);
  
}

void write(char* string) { // Function receives a string pointer to be printed on LCD
    
    clearScreen();
    
    // For each character of string, print high and low bytes to LCD
    int i = 0;
    while(string[i]) {
        int data = (int)string[i];
        int high = data >> 4; // Isolate high byte
        int low = data & 0b00001111; // Isolate low byte
        
        LCDWrite(high, 1, 0, 1);
        LCDWrite(low, 1, 0, 1);
        _delay_ms(1);
        i++;
    }
}


/* 
   This function contains initialization code written
   by Geoffrey Messier that must be placed in the main 
   function of main file.
*/
void lcdInit() {
    
    // From the PCF8574 data sheet:
    //   Serial Clock Freq, fSCL = 100e3 (Hz)
    //   Bus Rise Time, Tr = 1e-6 (sec)
    // From the AVR data sheet:
    //   BAUD = fClk/(fSCL*2) - (5+fCLK*Tr/2)
    //        = 67
    
    
    TWI0.MBAUD = 67;
          
    // Enable TWI main.
    TWI0.MCTRLA |= 0x01;

    // Force bus state machine to idle.
    TWI0.MSTATUS = 0x01;


    /*
     * The following init sequence comes from page 16 of the LCD1602
     * data sheet.  Basically, you can't know for sure if the LCD controller
     * will boot up in 8 or 4 bit mode.  So, these three commands are meant to
     * switch it over to 8 bit mode first.  The delays are from the data sheet
     * (with some extra buffer time added).
     */

    LCDWrite(0b0011,0,0,1);
    _delay_ms(5);
    LCDWrite(0b0011,0,0,1);
    _delay_ms(5);
    LCDWrite(0b0011,0,0,1);
    _delay_ms(5);

    /*
     * The following init sequence is from section 5.5 from the SPLC780D data
     * sheet with an extra clear screen command thrown in.  Check out the 
     * earlier sections of that same data sheet to understand each command
     * in detail.
     */

    // Switch from 8 bit to 4 bit mode.
    LCDWrite(0b0010,0,0,1);

    // Set to 4 bit operation with 1 display line. 
    LCDWrite(0b0010,0,0,1);
    LCDWrite(0b0000,0,0,1);

    // Display on, cursor on, cursor flash off.
    LCDWrite(0b0000,0,0,1);
    LCDWrite(0b1110,0,0,1);

    // Cursor increment when write on, display shift off.
    LCDWrite(0b0000,0,0,1);
    LCDWrite(0b0110,0,0,1);

    // clear screen
    LCDWrite(0b0000,0,0,1);
    LCDWrite(0b0001,0,0,1);
}

void clearScreen() {

    LCDWrite(0b0000,0,0,1);
    LCDWrite(0b0001,0,0,1);
    
}