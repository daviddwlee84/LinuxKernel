# Kernel Hello World

* hello.c
* Makefile
  * make all
  * make clean

## Module Programming

### Init/Exit Macros

```c
module_init(hello_init);
module_exit(hello_exit);
```

* Decalring *module initialization* and *module cleanup*

Definition: [`<linux/module.h>`](https://github.com/torvalds/linux/blob/master/include/linux/module.h)

* [module_init](https://www.fsl.cs.sunysb.edu/kernel-api/re01.html): driver initialization entry point - function to be run at kernel boot time or module insertion
* [module_exit](https://www.fsl.cs.sunysb.edu/kernel-api/re02.html): driver exit entry point - function to be run when driver is removed
* [Need of using MACROS like module_init and module_exit while writing Loadable Kernel Modules](https://stackoverflow.com/questions/27469502/need-of-using-macros-like-module-init-and-module-exit-while-writing-loadable-ker)

### Function

```c
static int __init hello_init(void);
static void __init hello_exit(void):
```

* static: There is no other meaning exist except this file
* __init: Only be used in initialization. After the module is loaded, this function will be release from RAM.
* __exit: Only be used in removing module.

Definition: [`<linux/init.h>`](https://github.com/torvalds/linux/blob/master/include/linux/init.h)

* [The __init and __exit Macros](https://www.tldp.org/LDP/lkmpg/2.6/html/x245.html)

## Steps

### Build Kernel

```sh
$ make
```

Generate files

* hello.mod.c
* hello.mod.o
* hello.o
* hello.ko
* modules.order
* Module.symvers

### Install Kernel

```sh
# This will execute module_init()

$ sudo insmod hello.ko
```

Check

```sh
$ lsmod | grep hello
hello           16384   0

$ dmesg | tail
[   354.954788] Hello world
```

### Remove Kernel

```sh
# This will execute module_exit()

sudo rmmod hello
```

> `sudo rmmod hello.ko` may also work

```sh
$ dmesg | tail
[   354.954788] Hello world
[   404.508377] Goodbye world
```
