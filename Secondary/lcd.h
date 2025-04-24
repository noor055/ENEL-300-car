/* 
 * File:   lcd.h
 * Author: youss
 *
 * Created on April 21, 2025, 7:53 PM
 */

#ifndef LCD_H
#define	LCD_H

// The I2C chip is a PCF8574 with secondary address indicated in Fig 9 of the 
// data sheet.  The last 3 address bits are open jumpers on the back of the 
// device (look closely at it).
#define DISPLAY_ADDR 0x27

void I2CWrite(uint8_t address, uint8_t data);
void LCDWrite(uint8_t nibble, uint8_t rs, uint8_t rw, uint8_t ledOn);
void write(char* string);
void lcdInit();
void clearScreen();

#endif	/* LCD_H */

