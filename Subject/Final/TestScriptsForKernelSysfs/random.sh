#!/bin/sh
# https://unix.stackexchange.com/questions/140750/generate-random-numbers-in-specific-range
MIN=1
MAX=100
random_number=`shuf -i $MIN-$MAX -n 1`
echo $random_number > /sys/rpi_7seg/display
sleep 1
python -c "import random; print random.randint($MIN, $MAX - 1)" > /sys/rpi_7seg/display
sleep 1
echo $(( RANDOM % ($MAX - $MIN + 1 ) + 1 )) > /sys/rpi_7seg/display
sleep 1
echo $RANDOM > /sys/rpi_7seg/display
sleep 1
cat /dev/urandom | tr -dc '0-9' | fold -w 2 | head -n 1 > /sys/rpi_7seg/display
