#!/bin/bash

# The sample programs is reffered from https://www.geeksforgeeks.org/memory-layout-of-c-program/

# Initial version
Prog1="
#include <stdio.h>
  
int main(void)
{
    return 0;
}
"

# Add a global varialbe => bss
# * bss
Prog2="
#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    return 0;
}
"

# Add a static variable => bss
# * bss
Prog3="
#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    static int i; /* Uninitialized static variable stored in bss */
    return 0;
}
"

# Initialize the static variable => ds
# * bss
# * ds
Prog4="
#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    static int i = 100; /* Initialized static variable stored in DS*/
    return 0;
}
"

# Initialize the global variable => ds
# * ds
Prog5="
#include <stdio.h>

int global = 10; /* initialized global variable stored in DS*/

int main(void)
{
    static int i = 100; /* Initialized static variable stored in DS*/
    return 0;
}
"

Progs=( "$Prog1" "$Prog2" "$Prog3" "$Prog4" "$Prog5" )

for prog in "${Progs[@]}"
do
    echo "$prog" > prog_temp.c
    gcc prog_temp.c

    echo "===================="
    echo "The code:"
    echo "$prog"
    echo "The size of the program:"
    size a.out
done

rm prog_temp.c a.out