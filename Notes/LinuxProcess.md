# Linux Process

## Overview

What is process?

An instance of an executing program.

What is thread?

An execution flow of the process.

* Process vs. Thread
  * Similarity
    * Threads share CPU like process. (and only one thread active (running) at a time)
    * If one thread is blocked, another thread can run
  * Difference
    * Threads are *not independent* of one another
    * Threads are *design to assist one other*
        > Process might not because it may originate from different users
    * All threads can access every *address in the task*

### Process in Linux

* [linux/sched.h](https://github.com/torvalds/linux/blob/master/include/linux/sched.h)
  * [`task_struct`](#Process-Descriptor): PCB
  * `task_group`

#### 4 Properties of Process

1. Different process may share the same code.
   * Like you open a application multiple times
2. Process has its own *system heap stack space*
   * The "private property"
3. PCB (task_struct in Linux), a data structure to record a state of a process
   * Make process to be a unit for *kernel scheduling*
   * Also recording all the resource occupy by a process
4. Process has independent storage, i.e. special user space.

Without the 4th condition, then call Thread

* Completely without user space => kernel thread
* Share user space => user thread

### Thread in Linux

> From the aspect of kernel, there is no concept of threads in Linux

* No scheduling for threads
* No data structure to represent a thread (unlike `task_struct` for process)

> Linux kernel **does not priovide any special** *scheduling semantics* or *data structure* to represent threads
>
> (A thread is merely a process that shares certain resources with other processes)

#### Light-weight Process

> Allow both parent and the child process to share many per-process kernel data structures

* [Wiki - Light-weight process](https://en.wikipedia.org/wiki/Light-weight_process)

Between lightweight process, they can share some resources

* Address spce
* Opened file
* ...

#### User Thread

* [Wiki - Pthreads](https://en.wikipedia.org/wiki/POSIX_Threads)

POSIX Standard

* [Wiki - POSIX](https://en.wikipedia.org/wiki/POSIX) - Portable Operating System Interface

Implementation of Thread in Linux

* LinuxThreads - Old
  * Create N+1 thread, one of them is to handle resources
  * [Wiki - LinuxThreads](https://en.wikipedia.org/wiki/LinuxThreads)
* NPTL (Native Posix Thread Library) - Current
  * Create a *light-weight process* for each thread
  * [Wiki - Native POSIX Thread Library](https://en.wikipedia.org/wiki/Native_POSIX_Thread_Library)

#### Kernel Thread

* No user space address
* ...

## Process Implementation in Linux

### Process Descriptor

PCB Process Control Block: Contain all the information related to a single process

* opened files
* the process's address space
* pending signals
* the process's state
* ...

`task_struct`

![task_struct](https://gityuan.com/images/linux/process/task_struct.jpg)

A process descriptor keep track of:

* Process attributes
* Process relationships
* Process memory space
* FIle management
* Signal management
* Process credentials
* Resource limits
* Scheduling related fields

Kernel Stack

...

* [asm/thread_info.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/thread_info.h)
  * `struct thread_info`
  * `THREAD_SIZE` (define in [asm/page_64_types.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/page_64_types.h))

#### Identifying a Process

`current` MACRO:

> * get the process descriptor pointer of the currently running on CPU
> * different implement on different architectures

* `#define current get_current()` (in [asm/current.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/current.h))

PID:

* Global ID: The only one ID for kernel itself and initial name space
  * `SYSCALL_DEFINE0` (in [kernel/sys.c](https://github.com/torvalds/linux/blob/master/kernel/sys.c))
  * `struct pid` (in [linux/pid.h](https://github.com/torvalds/linux/blob/master/include/linux/pid.h))
* Local ID: The ID which belongs to a specific name space
    > Container (docker): Namespace and Cgroups
  * `struct upid`: pid in namespace

Process Group and Session

* `enum pid_type` (in [linux/pid.h](https://github.com/torvalds/linux/blob/master/include/linux/pid.h))

#### Process State

![linux process state](https://4.bp.blogspot.com/-k9KDTfkN8Gs/UxILiXK0sOI/AAAAAAAAA1c/7I_LlZ9xiwI/s1600/03fig03.gif)

* [linux/sched.h](https://github.com/torvalds/linux/blob/master/include/linux/sched.h)
  * `TASK_RUNNING`
    * in Ready Queue
    * in CPU
  * `TASK_INTERRUPTIBLE`
    * suspended (sleeping)
    * until some *condition becomes true* or it *receives a signal* (back to `TASK_RUNNING`)
  * `TASK_UNINTERRUPTIBLE`
    * suspended (sleeping) (like `TASK_INTERRUPTIBLE`)
    * but only signal can wake it up
  * `__TASK_STOPPED`
  * `__TASK_TRACED`
    * stopped by a debugger
  * `EXIT_DEAD`
    * final state
  * `EXIT_ZOMBIE`
    * process exit but haven't cleaned all the state structure

#### Process Family Tree

* All process are descendents of the **init process** (PID = 1)
  * Statically allocated as `init_task`
  * Kernel starts init process in the last step of the boot process
* Every process P has
  * One parent: `struct task_struct *parent`
    * Real_parent: process created P or init process (process 1), if the parent process no longer exists
    * Parent: current parent of P
  * Zero or more children: `struct list_head children`
* Siblings: parent's direct children: `struct list_head sibling`

> The relationships stored in the process descriptor

```sh
$ pstree
```

### Process Creation

In traditional UNIX

* `fork()`: create a child process that is a copy of the current task
  * Resources owne by the parent process are duplicated in the child process
  * Difference --> Process creation very slow and inefficient => solved in modern UNIX
    * PID
    * parent PID
    * certain resources
    * statistics (e.g. pending signals)

In modern UNIX

* **Copy-On-Write (COW)** technique
  * if call `exec()` right after `fork()`, then we won't even need to copy at all
* [Lightweight processes](#Light-weight-Process)
* `vfork()`
  * creates a process that shares the memory address space of its parent

* [Stackoverflow - The difference between fork(), vfork(), exec() and clone()](https://stackoverflow.com/questions/4856255/the-difference-between-fork-vfork-exec-and-clone)

![fork()](http://gityuan.com/images/linux/process/do_fork.jpg)

#### Process Creation System Call (fork(), vfork(), clone())

[![Process Creation System Call](https://flylib.com/books/4/454/1/html/2/images/03fig09.jpg)](https://flylib.com/books/en/4.454.1.42/1/)

* `fork()`
  * Copy all the resources (use COW) but not sharing
  * `clone(SIGCHLD, 0)`
* `vfork()`
  * Share the code before calling the `exec()` or `exit()`
  * `clone(CLONE_VFORK | CLONE_VM | SIGCHLD, 0)`

> `sys_fork()`, `sys_vfork()` and `sys_clone()` all call `do_fork()` (defined in [`kernel/fork.c`](https://github.com/torvalds/linux/blob/master/kernel/fork.c))
>
> `do_fork()` call `copy_process()`

The `copy_process()` function sets up the *process descriptor* and any other *kernel data structure* required for a child's execution.

```sh
# Ubuntu docker
$ ps aux
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  1.0  0.1  18504  3188 pts/0    Ss   06:30   0:00 /bin/bash
root        13  0.0  0.1  34396  2744 pts/0    R+   06:30   0:00 ps aux
```

> In `man ps`, LEGACY DESCRIPTION
>
>      The biggest change is in the interpretation of the -u option, which now displays processes belonging to the specified username(s).  Thus, "ps -aux" will fail
     (unless you want to know about user "x").  As a convenience, however, "ps aux" still works as it did in Tiger.

```sh
$ sleep 10 &
[1] 14180

# PPID = Parent PID
$ ps -f
  UID   PID  PPID   C STIME   TTY           TIME CMD
  502   523   522   0  2:05PM ttys003    0:01.22 -zsh
  502 14180   523   0 10:34AM ttys003    0:00.01 sleep 10

[1]  + 14180 done       sleep 10
```

#### Create Thread

> It's similar with create normal process (call `clone()`)

Thread Group

* `pthread_create` ([pthread_create](http://man7.org/linux/man-pages/man3/pthread_create.3.html))
* `CLONE_THREAD` ([clone](http://man7.org/linux/man-pages/man2/clone.2.html))
* All the subprocess has same TGID

#### Create Kernel Thread

* kernel_thread
* kthread_create
  * kthreadd --> create_kthread -> kernel_thread
* kthread_run
  * call `kthread_create()` and wake immediately

### Process Switch

* Process Context
  * User space
    * code
    * data
    * user stack and heap
  * Control info
  * Hardware context: Registers
    * in `thread_struct` (in `task_struct`)
      * other info.............

**switch_to() MACRO**: for process swithing

* `#define switch_to(prev, next, last)` (in [asm/switch_to.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/switch_to.h))
  * prev: the current process
  * next: the process to be scheduled
  * last: the process which we're going to switch back

> To understand **why we need the third parameter `last`**
>
> * [會一棍子打暈進程的宏：宏switch_to分析](http://www.maxwellxxx.com/switchto)

### Process Termination

System calls that terminate a User Mode application

* `exit_group()`
  * terminates a full *thread group* (i.e. a whold multithreaded application)
  * `do_group_exit()`
  * should be invoked by the `exit()` C library funciton
* `exit()`
  * terminates a *single process*
  * `do_exit()`
  * e.g. invoked by the `pthread_exit()`

Destroying Process

`wait()`

Orphan process: the process which lose its parent process

#### A Lifetime of a Process

1. `fork()`
2. `exec()`
3. `exit()` or `return`
4. `wait()`

## Namespace

Virtualization

* Virtual machine
  * independent kernel/user
  * complete environment/user
* Container based on Namespace
  * multiple namespace for each kernel
  * application level virtualization

### Linux Containers (LXC)

* Namespace
* Cgroups: as a file system under `/sys/fs/cgroup`

Structure order

* Hierarchy
* Flat

Support

* Activate while compiling
* Default namespace => compatibility

Create new process: share namespace with parent or create new namespace

### UTS Namespace

> UTS stands for UNIX Timesharing System

* [What's a UTS namespace?](https://unix.stackexchange.com/questions/183717/whats-a-uts-namespace)

uts_namespace

* [Wiki - Linux namespaces](https://en.wikipedia.org/wiki/Linux_namespaces)

## Resources

### Book

Understanding Linux Kernel - Ch3 Process

* Ch3.1 Process Descriptor
  * Ch3.1.1 Process State
  * Ch3.1.2 Identifying a Process
    * Ch3.1.2.3 The current macro
* Ch3.2 Process Switching
  * Ch3.2.1 Hardware Context
  * Ch3.2.3 The switch_to Macro
* Ch3.3 Creating Processes
  * Ch3.3.1 The clone(), fork(), and vfork() System Calls
  * Ch3.3.2 Kernel Threads
* Ch3.4 Destroying Process

### Article

* [Supercharged Computing Notes - Linux Processes](https://superchargedcomputing.com/2018/03/03/linux-processes/)
* [Linux進程管理(一)](http://gityuan.com/2017/07/30/linux-process/)
* [Linux進程管理(二)--fork](http://gityuan.com/2017/08/05/linux-process-fork/)
* [Linux ps command](https://www.computerhope.com/unix/ups.htm)
* [CSDN - Linux下進程的創建過程分析(_do_fork/do_fork詳解)--Linux進程的管理與調度（八）](https://blog.csdn.net/gatieme/article/details/51569932)
