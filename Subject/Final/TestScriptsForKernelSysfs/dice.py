#!/usr/bin/python3
import random
import sys
import time

MAX_ROUND = 50

if len(sys.argv) < 2:
    print("Usage: dice.py max_num (this will show a random number between 1~max_num)")
elif int(sys.argv[1]) < 0:
    print("Please input positive number!")
else:
    max_num = int(sys.argv[1])

    with open('/sys/rpi_7seg/display', 'w') as rpi:
        rpi.write(str(-1))  # first clean the display

    for i in range(MAX_ROUND):
        number = random.randrange(1, 1 + max_num)
        with open('/sys/rpi_7seg/display', 'w') as rpi:
            rpi.write(str(number))
        time.sleep(i * 0.003)  # make the dice effect
