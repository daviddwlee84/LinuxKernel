# Kernel Programming

## Kernel Compiling

### Makefile, Kconfig, .config files

* [淺談內核的Makefile、Kconfig和.config文件](https://blog.csdn.net/thisway_diy/article/details/76981113)

## Kernel Debugging Technique

### Printk

### kprobe

### SystemTap

* [SystemTap Official Website](https://sourceware.org/systemtap/)

SystemTap provides free software (GPL) infrastructure to simplify the gathering of information about the running Linux system. This assists diagnosis of a performance or functional problem. SystemTap eliminates the need for the developer to go through the tedious and disruptive instrument, recompile, install, and reboot sequence that may be otherwise required to collect data.

### strace

### Kernel Oops Messages

### The magic SysRq key

### Debug Tools

#### KDB

#### KGDB

## Module Programming

LKM (Loadable Kernel Module)

* [The Linux Kernel Module Programming Guide](https://www.tldp.org/LDP/lkmpg/2.6/html/index.html)

### Difference between kernel module programming and C application programming

### Command

#### Install Module

```
insmod
```

#### Remove Module

```
rmmod
```

#### List Module

Show *current running* module in this system.

```
lsmod
```

This program function is simply reading information from `/proc/modules` file system.

So it's equivalent to `cat /proc/modules`

#### modprobe

This is similar with **insmod**, be used to load module into kernel.

```
modprobe
```

## Others

### Difference between Linux 2.4 and Linux 2.6 up while programming
