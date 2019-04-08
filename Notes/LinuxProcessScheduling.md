# Linux Process Scheduling

## Overvie

### Process Categories

* Interactive Process
* Batch Process
* Real-time Process

#### Real-time Process

* Hard Real-time Process
  * must response in *specific* time (not too fast or too slow)
  * Linux kernel don't support this (but you can customize that)
* Soft Real-time Process
  * response as fast as possible
    * given higher priority...
* Normal Process
  * (any other process, like batch process, interactive process...)

### Scheduling Policy

### Context Switch Frequency

* Neither too long nor too short
  * too short: overhead for process switch
  * too long: processes no longer appear to ........
* Always a compromise

## Linux Scheduling Algorithm

History

* O(n)
* O(1) Scheduler
* [CFS](#Completely-Fair-Scheduler-(CFS))
  * Introducing since version 2.6.23

### Kernel version 2.4

* 1400 lines of code
* `schedule_data` as the only linked list in system
  * defined in `sched.c`

- [Linux 2.4.0](https://github.com/yifengyou/linux-2.4.0)
  - [`kernel/sched.c`](https://github.com/yifengyou/linux-2.4.0/blob/master/linux-2.4.0/kernel/sched.c)
  - [`include/linux/sched.h`](https://github.com/yifengyou/linux-2.4.0/blob/master/linux-2.4.0/include/linux/sched.h)

### Linux 2.6 O(1)

> Prior to Kernel 2.6.23

#### UMA (Uniform Memory Access)

* [Wiki - Uniform memory access](https://en.wikipedia.org/wiki/Uniform_memory_access)

a shared memory architecture used in parallel computers

#### SMP (Symmetric Multiprocessor)

* [Wiki - Symmetric multiprocessing](https://en.wikipedia.org/wiki/Symmetric_multiprocessing)

#### runqueue Data Structure

* [Wiki - Run queue](https://en.wikipedia.org/wiki/Run_queue)

### Completely Fair Scheduler (CFS)

> After Linux 2.6.23

* [Wiki - Completely Fair Scheduler](https://en.wikipedia.org/wiki/Completely_Fair_Scheduler)

* [Youtube - Assembly Language Tutorial 4 Floats & Switch](https://youtu.be/zj-cGdNpYZ8)

## Resources

### Article

* [Shichao's Notes - Chapter 4. Process Scheduling](https://notes.shichao.io/lkd/ch4/)
