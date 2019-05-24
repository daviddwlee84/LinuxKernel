#include "seven_seg.h"
#include <linux/gpio.h>
#include <linux/delay.h>

// Initialize the global variables

// GPIO pinout
const short SegmentPin[TotalLED] = {21, 20, 16, 12, 25, 24, 23, 18}; // a, b, c, d, e, f, g, decimal point
const short DigitPin[TotalDigits] = {22, 27, 17, 4};                 // digit 0-3

// Number Table
const short NumTable[11][PinUsed] = {{1, 1, 1, 1, 1, 1, 0},  // 0
                                     {0, 1, 1, 0, 0, 0, 0},  // 1
                                     {1, 1, 0, 1, 1, 0, 1},  // 2
                                     {1, 1, 1, 1, 0, 0, 1},  // 3
                                     {0, 1, 1, 0, 0, 1, 1},  // 4
                                     {1, 0, 1, 1, 0, 1, 1},  // 5
                                     {1, 0, 1, 1, 1, 1, 1},  // 6
                                     {1, 1, 1, 0, 0, 0, 0},  // 7
                                     {1, 1, 1, 1, 1, 1, 1},  // 8
                                     {1, 1, 1, 1, 0, 1, 1},  // 9
                                     {0, 0, 0, 0, 0, 0, 0}}; // Empty

short NumToShow[TotalDigits] = {10, 10, 10, 10}; // unit, ten, hundred, thousand

#define GPIO_REQUEST_LABEL "rpi_7seg"

// Function
void init_7seg_gpio(void)
{
    int i;
    for (i = 0; i < PinUsed; i++)
    {
        gpio_request(SegmentPin[i], GPIO_REQUEST_LABEL);
        gpio_direction_output(SegmentPin[i], 0);
    }
    for (i = 0; i < DigitUsed; i++)
    {
        gpio_request(DigitPin[i], GPIO_REQUEST_LABEL);
        gpio_direction_output(DigitPin[i], 1);
    }
}

void free_7seg_gpio(void)
{
    int i;
    for (i = 0; i < PinUsed; i++)
    {
        gpio_free(SegmentPin[i]);
    }
    for (i = 0; i < DigitUsed; i++)
    {
        gpio_free(DigitPin[i]);
    }
}

void setDigit(int digit)
{
    int i;
    for (i = 0; i < PinUsed; i++)
    {
        gpio_set_value(SegmentPin[i], NumTable[digit][i]);
    }
}

void showDigit(int pos)
{
    int i;
    for (i = 0; i < DigitUsed; i++)
    {
        gpio_set_value(DigitPin[i], (pos != i));
    }
}

void clearDisplay(void)
{
    int pos;
    for (pos = 0; pos < DigitUsed; pos++)
    {
        NumToShow[pos] = 10;
    }
}

void setNumber(int number)
{
    int pos;
    if (number < 0)
    {
        // If receive negative number, then clear the display.
        clearDisplay();
        return;
    }
    for (pos = 0; pos < DigitUsed; pos++)
    {
        // If the number position greater than 10 is 0 then don't display it!
        if (number == 0 && pos > 0)
            NumToShow[pos] = 10;
        else
        {
            NumToShow[pos] = number % 10;
            number /= 10;
        }
    }
}

void showAllDigits(unsigned int duration)
{
    int t, pos;
    for (t = 0; t < duration; t += DISP_DELAY_US)
    {
        for (pos = 0; pos < DigitUsed; pos++)
        {
            showDigit(pos);
            setDigit(NumToShow[pos]);
            usleep_range(DISP_DELAY_US, DISP_DELAY_US);
        }
    }
}