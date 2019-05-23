# Final Project: Manipulating Raspberry Pi GPIO with Direct Register Access in Kernel Space

## Overview

### 7-Segment Display

* Common Cathode 共陰極 (in this experiment)
* Common Anode 共陽極

![Common cathode 4-digit 7-seg pinouts](https://raspi.tv/wp-content/uploads/2015/11/7seg-pinout-annotated_700.jpg)

### GPIO Registers of Raspberry Pi

* [**Low Level Programming of the Raspberry Pi in C**](https://www.pieter-jan.com/node/15)
* [**RPi GPIO Code Samples**](https://elinux.org/RPi_GPIO_Code_Samples#Direct_register_access)
* [**SlideShare - 深入淺出Raspberry Pi GPIO**](https://www.slideshare.net/raspberrypi-tw/raspberry-pigpiolookinside)
  * [LED example](https://github.com/raspberrypi-tw/tutorial/tree/master/gpio/led/c)
  * [Raspberry Pi硬體暫存器的設定方式](http://blog.ittraining.com.tw/2015/05/raspberry-pi_23.html) - Others notes
* [Memory Mapped GPIO](http://merkles.com/Using_GPIOs/Memory_Mapped_GPIO.html)
* [**Raspberry Pi And The IoT In C - Memory Mapped GPIO**](https://www.iot-programmer.com/index.php/books/22-raspberry-pi-and-the-iot-in-c/chapters-raspberry-pi-and-the-iot-in-c/59-raspberry-pi-and-the-iot-in-c-memory-mapped-gpio)

#### BCM2835

> * [bcm2835 datasheet](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)
>   * [BCM2835 datasheet errata](https://elinux.org/BCM2835_datasheet_errata)
> * [bcm2835 library manual](http://www.airspayce.com/mikem/bcm2835/)

* 1.2 Address map
  * 1.2.1 Diagrammatic overview:
    ![BCM2835 Addressing](figure/BCM2835Addressing.png)
  * 1.2.2 ARM virtual addresses (standard Linux kernel only)
    * Addressing: virtual address -> physical address -> bus address
  * 1.2.3 ARM physical addresses
    * Physical address: 0x20000000 ~ 0x20FFFFFF
  * 1.2.4 Bus addresses
    * Bus address: 0x7E000000 ~
* 6.1 Register view
  | Address    | Register Name | Description            | Size | Read/Write |
  | ---------- | ------------- | ---------------------- | ---- | ---------- |
  | 0x7E200000 | GPFSEL0       | GPIO Function Select 0 | 32   | R/W        |
  * Table 6-1 GPIO Function Select Registers (GPFSELn)
    * FSEL29: 29~27 bits
      * 000 = GPIO Pin is an input
      * 001 = GPIO Pin is an output
      * 100 = GPIO Pin takes alternate function 0
      * 101 = GPIO Pin takes alternate function 1
      * 110 = GPIO Pin takes alternate function 2
      * 111 = GPIO Pin takes alternate function 3
      * 011 = GPIO Pin takes alternate function 4
      * 010 = GPIO Pin takes alternate function 5

BCM2835 Pin to Phisical Pin

### Raspberry Pi Memory

* `/proc/iomem`
* `/proc/device-tree/soc/ranges`
* `/sys/class/gpio/`
* `/dev/mem`

```sh
# to discover where devices are in memory
$ sudo cat /proc/iomem
00000000-3b3fffff : System RAM
  00008000-00afffff : Kernel code
  00c00000-00d468af : Kernel data
3f006000-3f006fff : dwc_otg
3f007000-3f007eff : /soc/dma@7e007000
3f00b840-3f00b84e : /soc/vchiq
3f00b880-3f00b8bf : /soc/mailbox@7e00b880
3f100000-3f100027 : /soc/watchdog@7e100000
3f101000-3f102fff : /soc/cprman@7e101000
3f200000-3f2000b3 : /soc/gpio@7e200000
3f201000-3f201fff : /soc/serial@7e201000
  3f201000-3f201fff : /soc/serial@7e201000
3f202000-3f2020ff : /soc/mmc@7e202000
3f206000-3f2060ff : /soc/pixelvalve@7e206000
3f207000-3f2070ff : /soc/pixelvalve@7e207000
3f212000-3f212007 : /soc/thermal@7e212000
3f215000-3f215007 : /soc/aux@0x7e215000
3f215040-3f21507f : /soc/serial@7e215040
3f300000-3f3000ff : /soc/mmc@7e300000
3f400000-3f405fff : /soc/hvs@7e400000
3f805000-3f805fff : /soc/i2c@7e805000
3f806000-3f806fff : /soc/vec@7e806000
3f807000-3f8070ff : /soc/pixelvalve@7e807000
3f808000-3f8080ff : /soc/hdmi@7e902000
3f902000-3f9025ff : /soc/hdmi@7e902000
3f980000-3f98ffff : dwc_otg
3fc00000-3fc00fff : /soc/v3d@7ec00000
```

## Linux Library

* [`include/linux/module.h`](https://github.com/torvalds/linux/blob/master/include/linux/module.h)

### Linux Kernel Thread

* [`include/linux/kthread.h`](https://github.com/torvalds/linux/blob/master/include/linux/kthread.h)

* Playing with Systems Blog
  * [Kernel Threads](https://sysplay.in/blog/linux-kernel-internals/2015/04/kernel-threads/)
  * [Kernel Threads Continued](https://sysplay.in/blog/linux-kernel-internals/2015/05/kernel-threads-continued/)

### Linux GPIO

> But I decide to manipulate the BCM2835's registers by myself. Just take some notes.

* [`include/linux/gpio.h`](https://github.com/torvalds/linux/blob/master/include/linux/gpio.h)

    ```c
    /*
    * <linux/gpio.h>
    *
    * This is the LEGACY GPIO bulk include file, including legacy APIs. It is
    * used for GPIO drivers still referencing the global GPIO numberspace,
    * and should not be included in new code.
    *
    * If you're implementing a GPIO driver, only include <linux/gpio/driver.h>
    * If you're implementing a GPIO consumer, only include <linux/gpio/consumer.h>
    */
    ```

## Compiling Raspberry Kernel Module

* [How compile a loadable kernel module without recompiling kernel](https://raspberrypi.stackexchange.com/questions/39845/how-compile-a-loadable-kernel-module-without-recompiling-kernel)

Originally in the Raspbian image, you won't found the directory `/lib/modules/$(shell uname -r)/build`

Just install with the following commands

```sh
# make sure the system is the latest
sudo apt-get update -y
sudo apt-get upgrade -y

sudo rpi-update # not sure if this is necessary

# Install the kernel headers
sudo apt-get install raspberrypi-kernel-headers
```

After update, you'll found the directory under `/lib/modules` is upgraded (if you're previous system is not the latest). But calling the `uname -r` will still return the previous version. So just `sudo reboot`.

Finally in this project, it will be using the `4.19.42-v7+` Raspberry Pi kernel.

> Try with my [HelloWorld example](../Week1_BuildBasicKernel/HelloWorld) code to test the environment.

## Trouble Shooting

### ttyUSB0 not found (PL2303TA USB to TTL)

> Already connect GND, TX, RX on the Raspberry Pi

OS X doesn't create a /dev entry for raw access to USB devices, and there's no way to access them as TTY devices. ([Stackoverflow - Mac OS analog to /dev/ttyUSBxx](https://stackoverflow.com/questions/11784248/mac-os-analog-to-dev-ttyusbxx))

```sh
# Before plugin device
$ lsusb
Bus 020 Device 003: ID 0a5c:4500 Broadcom Corp. BRCM20702 Hub
Bus 020 Device 004: ID 05ac:828f Apple Inc. Bluetooth USB Host Controller
Bus 000 Device 001: ID 1d6b:IWPT Linux Foundation USB 3.0 Bus
$ ioreg -p IOUSB
+-o Root  <class IORegistryEntry, id 0x100000100, retain 15>
  +-o AppleUSBXHCI Root Hub Simulation@14000000  <class AppleUSBRootHubDevice, id 0x10000033c, registered, matched, activ$
    +-o BRCM20702 Hub@14300000  <class AppleUSBDevice, id 0x100025273, registered, matched, active, busy 0 (15706 ms), re$
      +-o Bluetooth USB Host Controller@14330000  <class AppleUSBDevice, id 0x100025287, registered, matched, active, bus$

# After plugin device
$ lsusb
Bus 020 Device 003: ID 0a5c:4500 Broadcom Corp. BRCM20702 Hub
Bus 020 Device 004: ID 05ac:828f Apple Inc. Bluetooth USB Host Controller
Bus 020 Device 009: ID 067b:2303 Prolific Technology, Inc. Composite Device
Bus 000 Device 001: ID 1d6b:IWPT Linux Foundation USB 3.0 Bus
$ ioreg -p IOUSB
+-o Root  <class IORegistryEntry, id 0x100000100, retain 15>
  +-o AppleUSBXHCI Root Hub Simulation@14000000  <class AppleUSBRootHubDevice, id 0x10000033c, registered, matched, activ$
    +-o BRCM20702 Hub@14300000  <class AppleUSBDevice, id 0x100025273, registered, matched, active, busy 0 (15706 ms), re$
    | +-o Bluetooth USB Host Controller@14330000  <class AppleUSBDevice, id 0x100025287, registered, matched, active, bus$
    +-o IOUSBHostDevice@14100000  <class AppleUSBDevice, id 0x100032509, registered, matched, active, busy 0 (15 ms), ret$
```

> `ioreg -r -c IOUSBHostDevice -l` It will print all the USB host devices, and it will print out all properties from the subtree of each device.

The device is this "Bus 020 Device 009: ID 067b:2303 Prolific Technology, Inc. Composite Device"

We can see that the device ID is

* VID: 067B
* PID: 2303

And I found a [PL2303 Mac OS X Driver](http://www.prolific.com.tw/us/showproduct.aspx?p_id=229&pcid=41). In its `readme.txt` it shows the same device ID like the following.

```txt
Release Note, PL-2303 Mac OS X driver v1.6.1
For Mac OS X 10.9 (Mavericks), 10.10 (Yosemite), 10.11 (El Capitan), 10.12 (Sierra), 10.13 (High Sierra)
Prolific Edition
=================================================================================

Supported device ID:
====================  
    VID:067B
    PID:2303

Kernel extension filename:
==========================  
    ProlificUsbSerial.kext

Device base name:
===================  
    usbserial

Device descriptions:
======================  
    USB-Serial Controller

Driver version:
====================  
    1.6.1
```

Just execute the `PL2303_MacOSX_1.6.1_20160309.pkg` and it will need to restart the computer.

After that you should be able to find `/dev/tty.usbserial` and `/dev/cu.usbserial`

* [**Adafruit's Raspberry Pi Lesson 5. Using a Console Cable Learn how to Connect to your Raspberry Pi**](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-5-using-a-console-cable/overview)
  * [the cable](https://www.adafruit.com/product/954)

Connect the device: (Raspberry Pi default UART baud rate 115200)

* Use `screen`: `screen /dev/cu.usbserial 115200`

#### Concept of USB ID

* [ttyUSB not created when plugging in USB devices](https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=160400&sid=b9c2ecd4e9bad794e7291f3dcc4142ec#p1040390)
* [The USB ID Repository](http://www.linux-usb.org/usb-ids.html)

## Resources

* [How Many Inputs? (GPIO/I₂C)](https://www.raspberrypi.org/forums/viewtopic.php?t=161950)
  * There are 28 GPIO routed to pins on the expansion header. 2 GPIO are reserved for system use. The remaining pins are connected to the 5V, 3V3, and ground rails.
* [RPi Low-level peripherals](https://elinux.org/RPi_Low-level_peripherals)

### Raspberry Pi Kernel Project

* [The Beginner’s Guide to Linux Kernel Module, Raspberry Pi and LED Matrix](https://blog.fazibear.me/the-beginners-guide-to-linux-kernel-module-raspberry-pi-and-led-matrix-790e8236e8e9)
  * [GitHub Gist -  fazibear/pix_mod.c](https://gist.github.com/fazibear/f1fe97c9799501e0e0955859e12ef4f8) - The Beginner’s Guide to Linux Kernel Module, Raspberry Pi and LED Matrix
* [Embedded Linux: Raspberry Pi B+ LED Blink thru GPIO using Kernel Driver or Linux Module](https://youtu.be/RlNPtBEZRkY)
  * [Embedded Linux: Raspberry Pi B+ LED Blink thru GPIO in C Programming](https://youtu.be/ayxYBZbAWAo)
  * [Embedded Linux: GPIO/LED control on Raspberry Pi through sysfs](https://youtu.be/Si1R2gaqE64)

### Raspberry Pi

* [Raspberry Pi Documentation GPIO](https://www.raspberrypi.org/documentation/usage/gpio/)
* [RPi Serial Connection](https://elinux.org/RPi_Serial_Connection)
* [Raspberry Pi Linux Kernel](https://github.com/raspberrypi/linux)

#### Pinout

* [**Raspberry Pi Pinout**](https://pinout.xyz/)
  * `pinout` command
* [Raspberry Pi B+ Pinout Leaf](https://github.com/splitbrain/rpibplusleaf)
* [sparkfun - Raspberry gPIo](https://learn.sparkfun.com/tutorials/raspberry-gpio/gpio-pinout)
* [Pin Numbering - Raspberry Pi 3 Model B](https://pi4j.com/1.2/pins/model-3b-rev1.html)
* [GPIO Pin Numbering](https://www.raspberrypi.org/forums/viewtopic.php?t=196696)

![GPIO-B+.png](https://www.raspberrypi.org/forums/download/file.php?id=20374&sid=e6fc7c4a3bf34d9d6c889a9405847cad)

#### Beginner Tutorial

* [The Pi Hut](https://thepihut.com/)
  * [An introduction to Raspberry Pi GPIO](https://thepihut.com/blogs/raspberry-pi-tutorials/an-introduction-to-raspberry-pi-gpio) - Python
  * [Turning on an LED with your Raspberry Pi's GPIO Pins](https://thepihut.com/blogs/raspberry-pi-tutorials/27968772-turning-on-an-led-with-your-raspberry-pis-gpio-pins) - Scratch
* [Physical Computing with Python](https://projects.raspberrypi.org/en/projects/physical-computing)
* [How to drive a 7 segment display directly on Raspberry Pi in Python](https://raspi.tv/2015/how-to-drive-a-7-segment-display-directly-on-raspberry-pi-in-python)

Video

* [Raspberry Pi GPIO Tutorial: The Basics Explained](https://www.youtube.com/watch?v=6PuK9fh3aL8)
* [Raspberry Pi3 Pinout Explained](https://www.youtube.com/watch?v=eQBcgYyB1vk)

### Library/Module/Package

#### C

* [**WiringPi**](http://wiringpi.com/) - [github](https://github.com/WiringPi/WiringPi)
  * [wiringPi Pin numbering](https://projects.drogon.net/wiringpi-pin-numbering/)
  * [WiringPi: GPIO Pin Numbering Tables](http://wiringpi.com/wp-content/uploads/2013/03/pins.pdf)
  * `gpio readall` command
* [RaspberryPi-GPIO](https://github.com/alanbarr/RaspberryPi-GPIO)

#### Python

* [**GPIO Zero**](https://gpiozero.readthedocs.io/en/stable/) - [github](https://github.com/RPi-Distro/python-gpiozero/)
  * [Raspberry Pi Documentation GPIO in Python](https://www.raspberrypi.org/documentation/usage/gpio/python/README.md)
  * Raspbian Install - GPIO Zero is installed by default in the Raspbian desktop image
    * `sudo apt install python-gpiozero python3-gpiozero`
  * Other OS
    * `sudo pip install gpiozero`
* **RPi.GPIO** - [SourceForge](https://sourceforge.net/p/raspberry-gpio-python/wiki/Home/) - [PyPi](https://pypi.org/project/RPi.GPIO/)
  * `sudo apt-get install python-rpi.gpio python3-rpi.gpio` - The RPi.GPIO module is installed by default in Raspbian.
  * `import RPi.GPIO as GPIO`

#### Node.js

* **onoff** - [github](https://github.com/fivdi/onoff)
  * [w3schools - Node.js Raspberry Pi - GPIO Introduction](https://www.w3schools.com/nodejs/nodejs_raspberrypi_gpio_intro.asp)

#### Java

* [Pi4J](https://pi4j.com/1.2/)
  * `curl -sSL https://pi4j.com/install | sudo bash`

### Linux

* `linux/gpio.h`
* [GPIO in the kernel: an introduction](https://lwn.net/Articles/532714/)
* [Xilinx Wiki/Linux Drivers - Linux GPIO Driver](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver)
