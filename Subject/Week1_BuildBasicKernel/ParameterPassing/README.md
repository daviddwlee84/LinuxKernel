# Passing Command Line Arguments

## Module Programming

### Passing parameter when installing module

Example:

```sh
insmod hello.ko test=2
```

### Macro module_param

`module_param(arg1, arg2, arg3)`

* arg1 : The name of the variable.
* arg2 : Its type.
  * Support datatype: byte, short, ushort, int, uint, long, ulong, bool, charp, Array, ... etc.
* arg3 : Permissions for the corresponding file in sysfs.

### Licensing and Module Description

* `MODULE_LICENSE()`
* `MODULE_DESCRIPTION()`
* `MODULE_AUTHOR()`

- [Licensing and Module Documentation](https://www.tldp.org/LDP/lkmpg/2.6/html/x279.html)

## Steps

### Build Module

```sh
make
```

### Install and Pass Parameter

```sh
sudo insmod parampassing.ko input_param=87
```

Check the result

```sh
$ lsmod
Module                  Size  Used by
parampassing           16384  0

$ dmesg
...
[  500.310960] Hello world, Parameter has been passed: 87
```

## Links

* [Passing Command Line Arguments to a Module](https://www.tldp.org/LDP/lkmpg/2.6/html/x323.html)
