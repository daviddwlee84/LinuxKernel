#!/bin/bash
gcc test/test.c gpio/BCM2837_GPIO.c gpio/seven_seg.c -Iheader -o test.out
