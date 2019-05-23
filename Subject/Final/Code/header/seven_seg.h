#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

// Global define
#define TotalLED 8
#define PinUsed 7
#define TotalDigits 4
#define DigitUsed 2

// GPIO pinout
extern const short SegmentPin[TotalLED];  // a, b, c, d, e, f, g, decimal point
extern const short DigitPin[TotalDigits]; // digit 0~3

// Number Table
extern const short NumTable[10][PinUsed]; // from 0 to 9

// Function
void init_7seg_gpio();    // Initialize the GPIOs used by 7-seg display
void setDigit(int digit); // Set the digit to display
void showDigit(int pos);  // Power the digit position to show
void clearNumber();       // Clean the digits

#endif // SEVEN_SEG_H