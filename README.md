# Linux Kernel

Linux Kernel Programming and Driver Design. PKU course notes and example.

## Schedule

|  Title | Subject                                                                                               |
| -----: | ----------------------------------------------------------------------------------------------------- |
| Week 0 | [Compile Linux 4.19.23 kernel on Ubuntu](Subject/Week0_CompileLinuxKernel/CompileLinuxKernel.md)      |
| Week 1 | [Build Basic Linux Kernel](Subject/Week1_BuildBasicKernel/BuildBasicKernel.md)                        |
|  Lab 1 | [Kernel Debug](Lab/Lab1_KernelDebug/KernelDebug.md)                                                   |
| Week 3 | [Linux Process](Subject/Week3_LinuxProcess)                                                           |
|  Lab 2 | [Add Kernel Patch and Read Process Info Module](Lab/Lab2_KernelPatch_ProcessInfo/)                    |
| Week 5 | No homework ʕ•͡ᴥ•ʔ                                                                                    |
| Week 6 | No homework maybe.. ʕ•͡ᴥ•ʔ ??                                                                         |
| Week 7 | [Synchronization Mechanism - RCU](Subject/Week7_SynchronizationMechanism/SynchronizationMechanism.md) |
| Week 8 | No homework (*￣▽￣)/‧☆"'-.,_,.-'"-.,_☆                                                                 |
| Week 9 | 5/1 Holiday                                                                                           |
|  Lab 3 | [Observe Memory Mapping and Find Physical Address](Lab/Lab3_Memory)                                   |

* [Experiment Collection](Notes/ExperimentCollection.md)

## Final Project: Manipulating Raspberry Pi GPIO in Kernel Space with 7-segment Display

> And also Direct Register Access in user space

* [Project Directory](Subject/Final)
  * [Experiment](Subject/Final/Experiment.md)
    * [for environment detail](Subject/Final/Experiment.md#Experiment-Envirounmant)
    * [for environment setting](Subject/Final/Experiment.md#Experiment-Steps)
  * [Analysis](Subject/Final/Analysis.md)
  * [slides](Subject/Final/slides)

```sh
# Quick start on Raspberry Pi (make sure you've set up the environment as mine)
# Copy the file to the home dir of raspberry pi (you can replace raspberrypi.local with the IP address)
scp -r Subject/Final pi@raspberrypi.local:~/KernelGPIO
# connect to raspberry pi
ssh pi@raspberrypi.local

# compile and install the rpi_7seg module
cd KernelGPIO/Code_KernelDelayThread_UseGPIO_H
./smart_install.sh

# play a dice!
cd ../TestScriptsForKernelSysfs
./dice.py 6
```

![compile demo](Subject/Final/figure/compile.gif)

![dice demo](Subject/Final/figure/dice.gif)

## Notes

### Course Notes

* [Linux Kernel Overview](Notes/LinuxKernelOverview.md)
* [Linux Kernel Programming](Notes/KernelProgramming.md)
* [Linux Process](Notes/LinuxProcess.md)
* [Linux Process Scheduling](Notes/LinuxProcessScheduling.md)
* [Linux Kernel Synchronization](Notes/KernelSynchronization.md)
* [Linux Interrupt, Exception and System Call](Notes/KernelInterrupt.md) - Skip system call now (TBD)
* [Linux Memory Management](Notes/MemoryManagement.md) (TBD)
* Virtual Memory (Maybe)
* [Linux File System](Notes/LinuxFileSystem.md)

### Linux Programming

* [**GNU Debugger**](Notes/GDB.md)

### Others

* [Virtual Machine](Notes/VirtualMachine.md)

## Resources

* [PKU Course Website](http://59.56.74.25:4455/)
* [Linux From Scratch](http://www.linuxfromscratch.org/)
* [Supercharged Computing Notes - Category: Kernel](https://superchargedcomputing.com/category/kernel/)

### Book

* Linux Kernel Development 3ed.
  * [Shichao's Notes LKD](https://notes.shichao.io/lkd/)
* Understanding the Linux Kernel (O'Reilly) (深入理解Linux内核)
* [Linux Device Drivers 3ed. (O'Reilly)](https://lwn.net/Kernel/LDD3/)
* Linux Kernel in a Nutshell (O'Reilly)
* Is Parallel Programming Hard (深入理解並行編程)
* Understanding Linux Network Internals (Linux網路技術內幕)
* 奔跑吧Linux內核
  * Github - [figozhang](https://github.com/figozhang)/[Running-LinuxKernel](https://github.com/figozhang/Running-LinuxKernel)

#### E-Book

* [**Introduction to Computer Organization: ARM Assembly Language Using the Raspberry Pi**](https://bob.cs.sonoma.edu/IntroCompOrg-RPi/intro-co-rpi.html)
* [**linux-insides**](https://0xax.gitbooks.io/linux-insides/content/index.html) - A book-in-progress about the linux kernel and its insides
  * [github](https://github.com/0xAX/linux-insides)
  * [chinese - Linux 内核揭秘](https://xinqiu.gitbooks.io/linux-insides-cn/content/index.html)
    * [github](https://github.com/MintCN/linux-insides-zh)
* [The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/index.html)
