#!/bin/bash

# 1. Build kernel module
make

# 2. Install kernel module
sudo insmod logadd2phyadd.ko

# 3. Compile logadd2phyadd_test.c
gcc logadd2phyadd_test.c -o logadd2phyadd_test

# 4. Run test program
./logadd2phyadd_test
