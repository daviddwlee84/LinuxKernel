# Final Project

## Overview

### 7-Segment Display

* Common Cathode 共陰極 (in this experiment)
* Common Anode 共陽極

![Common cathode 4-digit 7-seg pinouts](https://raspi.tv/wp-content/uploads/2015/11/7seg-pinout-annotated_700.jpg)

### Gerenral Purpose I/O Port

#### GPIO Registers of Raspberry Pi

## Data Structure

## Algorithm

## Result

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

### Raspberry Pi Kernel Project

* [The Beginner’s Guide to Linux Kernel Module, Raspberry Pi and LED Matrix](https://blog.fazibear.me/the-beginners-guide-to-linux-kernel-module-raspberry-pi-and-led-matrix-790e8236e8e9)
  * [GitHub Gist -  fazibear/pix_mod.c](https://gist.github.com/fazibear/f1fe97c9799501e0e0955859e12ef4f8) - The Beginner’s Guide to Linux Kernel Module, Raspberry Pi and LED Matrix
* [Embedded Linux: Raspberry Pi B+ LED Blink thru GPIO using Kernel Driver or Linux Module](https://youtu.be/RlNPtBEZRkY)
  * [Embedded Linux: Raspberry Pi B+ LED Blink thru GPIO in C Programming](https://youtu.be/ayxYBZbAWAo)
  * [Embedded Linux: GPIO/LED control on Raspberry Pi through sysfs](https://youtu.be/Si1R2gaqE64)

### Raspberry Pi

* [Raspberry Pi Documentation GPIO](https://www.raspberrypi.org/documentation/usage/gpio/)
* [RPi Serial Connection](https://elinux.org/RPi_Serial_Connection)

#### Pinout

* [**Raspberry Pi Pinout**](https://pinout.xyz/)
  * `pinout` command
* [Raspberry Pi B+ Pinout Leaf](https://github.com/splitbrain/rpibplusleaf)
* [sparkfun - Raspberry gPIo](https://learn.sparkfun.com/tutorials/raspberry-gpio/gpio-pinout)

#### Beginner Tutorial

* [The Pi Hut](https://thepihut.com/)
  * [An introduction to Raspberry Pi GPIO](https://thepihut.com/blogs/raspberry-pi-tutorials/an-introduction-to-raspberry-pi-gpio) - Python
  * [Turning on an LED with your Raspberry Pi's GPIO Pins](https://thepihut.com/blogs/raspberry-pi-tutorials/27968772-turning-on-an-led-with-your-raspberry-pis-gpio-pins) - Scratch
* [Physical Computing with Python](https://projects.raspberrypi.org/en/projects/physical-computing)
* [How to drive a 7 segment display directly on Raspberry Pi in Python](https://raspi.tv/2015/how-to-drive-a-7-segment-display-directly-on-raspberry-pi-in-python)

### Library/Module/Package

#### C

* [**WiringPi**](http://wiringpi.com/) - [github](https://github.com/WiringPi/WiringPi)
  * [wiringPi Pin numbering](https://projects.drogon.net/wiringpi-pin-numbering/)
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

### Linux

* `linux/gpio.h`
* [GPIO in the kernel: an introduction](https://lwn.net/Articles/532714/)
* [Xilinx Wiki/Linux Drivers - Linux GPIO Driver](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver)
