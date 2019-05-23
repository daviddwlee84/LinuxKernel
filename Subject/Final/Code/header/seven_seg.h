#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include <stdbool.h>

// Global define
#define TotalLED 8
#define PinUsed 7
#define TotalDigits 4
#define DigitUsed 2
#define DISP_DELAY_US 10000 // for visual persistence

// GPIO pinout
extern const short SegmentPin[TotalLED];  // a, b, c, d, e, f, g, decimal point
extern const short DigitPin[TotalDigits]; // digit 0~3

// Number Table
extern const short NumTable[10 + 1][PinUsed]; // from 0 to 9 + a None

// Basic Function
void init_7seg_gpio();    // Initialize the GPIOs used by 7-seg display
void setDigit(int digit); // Set the digit to display
void showDigit(int pos);  // Power the digit position to show
void clearNumber();       // Clean the digits

// Higher Level Funct
void setNumberPad(int number);            // Set the number to display (pad with zero)
void setNumber(int number, bool padZero); // Set the number to display
void showAllDigits(double duration);      // Show a number for a duration (sec)

#endif // SEVEN_SEG_H