#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

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
extern void init_7seg_gpio(void); // Initialize the GPIOs used by 7-seg display
extern void free_7seg_gpio(void); // Free the GPIOs used by 7-seg display
extern void clearDisplay(void);   // Set the digits to "not display"

// Higher Level Function
extern void setNumber(int number);                // Set the number to display
extern void showAllDigits(unsigned int duration); // Show a number for a duration (usec)

#endif // SEVEN_SEG_H