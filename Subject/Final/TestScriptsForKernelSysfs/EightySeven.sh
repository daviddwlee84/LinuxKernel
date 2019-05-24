#!/bin/sh
echo 87 > /sys/rpi_7seg/display
sleep 3
echo -1 > /sys/rpi_7seg/display # clear the display