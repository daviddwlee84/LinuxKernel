#!/bin/bash

# 1. Compile and Link singlefoo.c as shared library
gcc -o liblkpsinglefoo.so -O2 -fPIC -shared lkpsinglefoo.c

# 2. Copy the dynamic library to default library directory
sudo cp liblkpsinglefoo.so /usr/lib/

# 3. Compile test program lkpmem.c
gcc lkpmem.c liblkpsinglefoo.so -o lkpmem
