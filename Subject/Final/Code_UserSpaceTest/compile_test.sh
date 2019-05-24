#!/bin/bash
gcc test/test.c gpio/BCM2837_GPIO.c gpio/seven_seg.c -Iheader -o test.out
# I've made a Makefile, so either using this script, you can just use `make` and `make clean`
