# GNU Debugger

## Overview

## Preparation

GDB only debug the *executable file* (i.e. the binary), rather than the sorce code.

Compile the program with `-g` flag to generate **symbols** during compilation.

## Debugging

Tips:

* press enter will continue using the same last command
* you can `make` while staying in the gdb

Begin

* `gdb program`
* `gdb ./program`

### Start

* **run** (r)
* **start**: begin executing
* **quit**: leave the debugger

### Look around

* **list** (l): shows the 10 lines surrounding the current line
  * keep calling it will go through the rest of the codes
  * `list line`: goto the specific line
  * `list function`
* **print** (p)
  * `print variable`
    * You can only print the variable on the same [*frame*](#Frame)
  * `print variable = value`. You can set the variable value as you want.
* **x**: examine the memory directly
* **display**
* **whatis**

### Navigate

* **step** (s): step into
  * this will go into the subroutine (function).
* **next** (n): step through
  * this will stay in the same level.
  * treat the entire subroutine as a single line.
* **until**: run until exit the loop
  * `until line`: run until specific line
* **finish**
* **call**

### Breakpoints

* **break** (b)
  * `break function`
  * `break file:line`
* **continue** (c, cont): continue the program until reach the next breakpoint

#### Watch variable

* **watch**: set a watch for when a variable is written or read (then return to the debugger once this happens)
  * **rwatch**
  * **awatch**

#### Disable/Enable

* **delete**: delete watchpoint or break point
  * `delete number`
* **disable**
* **enable**
* **clear**

### Show status

* **info**
  * **info watch**: show watchpoints
  * **info break**: show breakpoints
  * **info locals**: show all the values of the local variables
  * `info function`
  * `info program`

### Assign value

* **set**
  * `set variable name=value`
* **show**

### Frame

* **where**
* **bt**: show the call frames for your program
  * **bt backtrace**
* **frame**
* **up**/**down**

## Resources

### Tutorial

* [**Youtube - Quick Intro to gdb**](https://youtu.be/xQ0ONbt-qPs)
* [Youtube - introduction to GDB a tutorial - Harvard CS50](https://youtu.be/sCtY--xRUyI)

### Book

* Advanced Linux Programming Ch1.4 Debugging with GNU Debugger (GDB)
