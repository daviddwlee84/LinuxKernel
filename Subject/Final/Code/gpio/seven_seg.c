#include "../header/BCM2837_GPIO.h"

// Initialize the global variables

// GPIO pinout
const short SegmentPin[TotalLED] = {21, 20, 16, 12, 25, 24, 23, 18}; // a, b, c, d, e, f, g, decimal point
const short DigitPin[TotalDigits] = {22, 27, 17, 4};                 // digit 0-3

// Number Table
const short NumTable[10][PinUsed] = {{1, 1, 1, 1, 1, 1, 0},  // 0
                                     {0, 1, 1, 0, 0, 0, 0},  // 1
                                     {1, 1, 0, 1, 1, 0, 1},  // 2
                                     {1, 1, 1, 1, 0, 0, 1},  // 3
                                     {0, 1, 1, 0, 0, 1, 1},  // 4
                                     {1, 0, 1, 1, 0, 1, 1},  // 5
                                     {1, 0, 1, 1, 1, 1, 1},  // 6
                                     {1, 1, 1, 0, 0, 0, 0},  // 7
                                     {1, 1, 1, 1, 1, 1, 1},  // 8
                                     {1, 1, 1, 1, 0, 1, 1}}; // 9

// Function
void init_7seg_gpio()
{
    int i;
    for (i = 0; i < PinUsed; i++)
    {
        INP_GPIO(SegmentPin[i]);
        OUT_GPIO(SegmentPin[i]);
    }
    for (i = 0; i < DigitUsed; i++)
    {
        INP_GPIO(DigitPin[i]);
        OUT_GPIO(DigitPin[i]);
    }
}

void clearNumber()
{
    for (int i = 0; i < PinUsed; i++)
    {
        GPIO_CLR = 1 << SegmentPin[i];
    }
}

void setDigit(int digit)
{
    clearNumber();
    for (int i = 0; i < PinUsed; i++)
    {
        GPIO_SET = NumTable[digit][i] << SegmentPin[i];
    }
}

void clearDisplay()
{
    for (int i = 0; i < DigitUsed; i++)
    {
        GPIO_SET = 1 << DigitPin[i];
    }
}

void showDigit(int pos)
{
    clearDisplay();
    GPIO_CLR = 1 << DigitPin[pos];
}
