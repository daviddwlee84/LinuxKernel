#include "../header/seven_seg.h"

// Initialize the global variables

// GPIO pinout
const short SegmentPin[8] = {11, 4, 23, 8, 7, 10, 18, 25}; // a, b, c, d, e, f, g, decimal point
const short DigitPin[4] = {22, 27, 17, 24};                // digit 0~ 3

// Number Table
const short NumTable[10][7] = {{1, 1, 1, 1, 1, 1, 0},  // 0
                               {0, 1, 1, 0, 0, 0, 0},  // 1
                               {1, 1, 0, 1, 1, 0, 1},  // 2
                               {1, 1, 1, 1, 0, 0, 1},  // 3
                               {0, 1, 1, 0, 0, 1, 1},  // 4
                               {1, 0, 1, 1, 0, 1, 1},  // 5
                               {1, 0, 1, 1, 1, 1, 1},  // 6
                               {1, 1, 1, 0, 0, 0, 0},  // 7
                               {1, 1, 1, 1, 1, 1, 1},  // 8
                               {1, 1, 1, 1, 0, 1, 1}}; // 9