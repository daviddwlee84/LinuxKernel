# Kernel Hello World

* hello.c
* Makefile

## Build Kernel

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

## Install Kernel

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

## Remove Kernel

```sh
# This will execute module_exit

sudo rmmod hello
```

```sh
$ dmesg | tail
[   354.954788] Hello world
[   404.508377] Goodbye world
```
