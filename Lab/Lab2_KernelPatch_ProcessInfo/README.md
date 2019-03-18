# Lab2

## Add Kernel Patch

Goal: Add patch to Linux 4.19.23 Kernel

> **Patch** is a document which store the difference between different version.
> Using patch to update source code, then you won't need to download the whole source code.
> i.e. update based on the basis of local source code

* [KernelNewbies](https://kernelnewbies.org/LinuxVersions)
  * [Linux_4.20 - 10. Networking](https://kernelnewbies.org/Linux_4.20#Networking) - This Experiment
    * [Update commit / patch](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=a337531b942bd8a03e7052444d7e36972aac2d92)
    * TCP: Previously TCP initial receive buffer is ~87KB by default and the initial receive window is ~29KB (20 MSS). This patch changes the two numbers to 128KB and ~64KB (rounding down to the multiples of MSS) respectively commit.

## Read Process Info Module

Goal:

* Implement a kernel module which create `/proc/tasklist` document
* Read all the process information of system, and show its pid, state and name
