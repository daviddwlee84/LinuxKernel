# Synchronization Primitives Study - RCU

[RCU Course Note](../../Notes/KernelSynchronization.md#RCU)

## Overview

> **Introduction in Linux Documentation**
>
> RCU is a synchronization mechanism that was added to the Linux kernel
during the 2.5 development effort that is optimized for read-mostly
situations.
>
> The basic idea behind RCU (read-copy update) is to split destructive
> operations into two parts, one that prevents anyone from seeing the data
> item being destroyed, and one that actually carries out the destruction.
> A "grace period" must elapse between the two parts, and this grace period
> must be long enough that any readers accessing the item being deleted have
> since dropped their references.  For example, an RCU-protected deletion
> from a linked list would first remove the item from the list, wait for
> a grace period to elapse, then free the element.

The misleading (misnomer):

The RCU does stand for Read, Copy and Update.
But Read, Copy, Update are actually a very small part of RCU mechanism.

Thus somtimes it's called "publishing protocol".

> "Copy-on-write" in Java

The publishing protocol (Read, Copy and Update) doesn't mek your concurrency synchronization scheme and RCU.

(You can use publishing protocol with *atomic shared pointers* or *hazard pointers*)

* RCU is distinguished by the specific way of memory reclamation.
* RCU also implies that instead of changing data in place, the writer publishes a new copy of the data
  * Read old data, copy it and update it
  * Then delete the old data with "the RCU way"

### Cost of Multithreading

* Context Switch
  * that's the advantage of using spinlock
* Atomic Operation
  * the operation offered specially by ISA is more expensive (doing multiple things in one operator)
* Disable Interrupt
* Lock
  * Semaphore, Condition Variable
    * sleeping/blocking and wakeup a thread need additional costs

## Background

In the concurrency, if all the data is read-only, then high performance is easy.
Because if there are no writer, then there is *no race condition*.
Even doing a massy computation is really fine.
The operations per second is going to be direct ratio with number of threads.

In real life situation:

1. read-only data does happen (often in scientific computation)
2. more often, data is "almost read-only" - changes happen but infrequently
   * update routing table
   * equipment state table maintenance
   * delay release of data structure
   * multipath I/O device maintenance
3. many reader threads, one writer thread => the following scenario

Consider a scenario: a data science computation

1. Many reader: doing heavy computing
2. Single writer: writing down the result

Idealy we want readers treats the data as *read-only data*.
That is no concurrency, no lock, no atomic operation => cost is very low

So we need a mechanism that can approximate a *read-only list*

### Attemption - The almost Read-Only data structure which avoids race condition

> The order of increasing number of thread and get the improvement on operations per second.
>
> 1. Read-only
> 2. Read-lock => cheapest atomic operation
> 3. Read/write-lock
> 4. Lock-free
>    * wrong choice when updates are rare

## Principle

### General RCU

* Data is accessible through "root pointer" (not always be a "pointer")
  * could be index into an array
  * must be atomic
* Reader
  1. acquire "root pointer" atomically
  2. access the data
* Writer
  1. **read** current data
  2. **copy** to new data
  3. **update** new data
  4. publish it

> Some readers see old data, other readers see new data (normal in concurrent system)

Then writer delete the old data. (In special way.) (The main part of RCU)

> The writer should not delete the reference block as long as at least one reader can access it

1. Hide the old data for the readers who are not reading
2. Wait until all the readers who are holding the RCU to finish their reading
3. Then delete the old data savely
   * In linked list just like the normal case => redirect the pointer

[![RCU delete node in linked list](https://www.ibm.com/developerworks/cn/linux/l-rcu/images/image004.jpg)](https://www.ibm.com/developerworks/cn/linux/l-rcu/index.html)

> This kind of memory reclamation (i.e. make memory unreachable) is called "garbage collection (GC)"

[Example of using std Library](#Using-std-Library)

### RCU Memory Reclamation

> Not the original Read, Copy, Update approach

* RCU use *cooperative protocol* to track when it is save to reclaim memory (when no reader can access it)

> That is, RCU implies that you're not *changing data in-place*. You're using the publishing protocol.

[Example of using cooperative handshake protocol](#Using-cooperative-handshake-protocol)

#### Reader

(must follow the steps to *access shared data*)

1. Call `rcu_read_lock()` to request access
2. Get the *root pointer* (not use the old copy)
3. Call `rcu_read_unlock()` to announce end of access

Readers may access shared data only between the calls to `rcu_read_lock()` and `rcu_read_unlock()`

Reader only in two state

1. reader-side critical section
2. quiescent state
   * reader in quiescent state don't read shared data

#### Writer

(must follow the step to *modify shared data*)

1. Make old shared data inaccessible from the root
2. Call `synchronize_rcu()` to wait for all readers who called `rcu_read_lock()` befor step 1 to call `rcu_read_unlock()`
3. Delete old data and reclaim the memory

We don't need to wait for all readers to exit critical section, only those who *acquired the old root pointer*.

### RCU in Linux Kernel

The advantage of Kernel RCU:

* Kernel knows when context switches happen (readers leave critical section)
* Kernel knows how many threads are there, which ones are running, etc
* Kernel RCU does not need (extra) memory barriers

> The basic idea is the same but the implementation may be differed from the User-space RCU. (There are some implicit assumptions like the advantage mentioned)

## Implementation

### Using std Library

```cpp
std::atomic<node*> head;
```

Reader - use atomic read

```cpp
node* p = head.load(std::memory_order_acquire);
do_search(p);
```

Writer

```cpp
node* new_node = new node(...);
node* next = head.load(std::memory_order_relaxed)->next; // "Read" current data
new_node->next = next; // "Copy" it to new data
head.store(new_node, std::memory_order_release); // "Update" current data
```

### Using cooperative handshake protocol

RCU API communications between the reader, updater, and reclaimer

![Wiki RCU API](https://upload.wikimedia.org/wikipedia/commons/thumb/6/6b/Rcu_api.jpg/440px-Rcu_api.jpg)

* `rcu_read_lock()` (alias in other system `rcu_enter()`)
* `rcu_read_unlock()` (alias in other system `rcu_leave()`)
* `synchronize_rcu()` (alias in other system `wait_for_readers_to_leave()`)
* `call_rcu()`: The differ deletion callback
* `rcu_assign_pointer()`
* `rcu_dereference()`

### Linux Kernel Source Code

* [include/linux](https://github.com/torvalds/linux/tree/master/include/linux)
  * rcu_node_tree.h
  * rcu_segcblist.h
  * rcu_sync.h
  * rculist.h
  * rculist_bl.h
  * rculist_nulls.h
  * rcupdate.h
  * rcupdate_wait.h
  * rcutiny.h
  * rcutree.h
  * rcuwait.h
* [kernel/rcu](https://github.com/torvalds/linux/tree/master/kernel/rcu)
  * rcu.h
  * ...

## Example in Linux Kernel

### Scheduler

In [`kernel/sched/core.c`](https://github.com/torvalds/linux/blob/master/kernel/sched/core.c#L4470)

```c
static int
do_sched_setscheduler(pid_t pid, int policy, struct sched_param __user *param)
{
	struct sched_param lparam;
	struct task_struct *p;
	int retval;

	if (!param || pid < 0)
		return -EINVAL;
	if (copy_from_user(&lparam, param, sizeof(struct sched_param)))
		return -EFAULT;

	rcu_read_lock();
	retval = -ESRCH;
	p = find_process_by_pid(pid); // RCU-iterates over list of processes
	if (p != NULL)
		retval = sched_setscheduler(p, policy, &lparam); // Writes back to RCU-protected structure
	rcu_read_unlock();

	return retval;
}
```

## Conclusion

RCU is really good when there are *rare updates* and okay with *inconsistent data*.
But if the requirement needs *perfect consistency* with *frequent updates* then you should try other mechanism.

## Appendix

### RCU APIs

#### List API

* list - Circular doubly linked list
* hlist Linear doubly linked list

### Patches with RCU

1. RCU was accepted into the Linux 2.5.43 kernel. Patches to RCU were applied to the 2.5.44 and 2.5.45 kernels. RCU was thus fully functional in 2.5.45 and later Linux kernels, just in time for the Halloween functionality freeze. ;-)
2. Patch to the System V IPC implementation using RCU was accepted into the Linux 2.5.46 kernel.
3. Patch providing a lock-free IPv4 route cache was accepted into the Linux 2.5.53 kernel.
4. Patch providing lock-free handler traversal for IPMI handling, added to the Linux 2.5.58 kernel.
5. Patch providing lock-free lookup of directory entries in the dcache subsystem, added to the Linux 2.5.62 kernel.
6. Patches to replace many uses of brlock with RCU in the 2.5.69 kernel, with brlock being entirely eliminated in the 2.5.70 kernel.
7. NMI handling for oprofile uses RCU in the 2.5.73 kernel.
8. Fix ppc64 {pte,pmd}_free vs. hash_page race with RCU in the 2.6.2 kernel.
9. Additional patches to the Linux kernel apply RCU to FD-set management, task-list traversal, and i_shared_sem contention reduction.
10. Yet more patches change RCU's API to conserve memory and stack space.
11. Another patch to monitor RCU grace period.
12. Another patch to apply RCU to fasync_lock, perhaps for the 2.7 timeframe.
13. Another set of patches apply modifications to the RCU infrastructure to make it safe for soft-realtime use (0/2, 1/2, 2/2).
14. The Reiser4 filesystem uses RCU to defer freeing of jnodes.
15. An auditing patch uses RCU to guard the lists of auditing rules.
16. An SELinux scalability patch uses RCU to guard the audit vector cache, with 500x improvement in write() throughput on 32 CPUs, and about 50% improvement on 2 CPUs.

## Resources

* [Linux 2.6內核中新的鎖機制--RCU](https://www.ibm.com/developerworks/cn/linux/l-rcu/index.html)
* [torvalds/linux - Documentation/RCU](https://github.com/torvalds/linux/tree/master/Documentation/RCU)
  * [**whatisRCU.txt**](https://github.com/torvalds/linux/blob/master/Documentation/RCU/whatisRCU.txt) - Introduction
  * [rcu.txt](https://github.com/torvalds/linux/blob/master/Documentation/RCU/rcu.txt) - RCU Concepts
  * [listRCU.txt](https://github.com/torvalds/linux/blob/master/Documentation/RCU/listRCU.txt) - Using RCU to Protect Read-Mostly Linked Lists
* [Wiki - Read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)
* [Wiki - Garbage collection (computer science)](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))
* [Introduction to RCU](http://www2.rdrop.com/users/paulmck/RCU/)
* [What is RCU, Fundamentally?](https://lwn.net/Articles/262464/)
* [RCU Concepts - rcu.txt](https://www.kernel.org/doc/Documentation/RCU/rcu.txt)
  * [Using RCU to Protect Read-Mostly Linked Lists - listRCU.txt](https://www.kernel.org/doc/Documentation/RCU/listRCU.txt)
* [What is RCU? Part 2: Usage](https://lwn.net/Articles/263130/)

### Video

* [CppCon 2017: Fedor Pikus “Read, Copy, Update, then what? RCU for non-kernel programmers”](https://youtu.be/rxQ5K9lo034)
  * [CppCon2017 Github](https://github.com/CppCon/CppCon2017) - [Slides](https://github.com/CppCon/CppCon2017/blob/master/Presentations/Read%2C%20Copy%2C%20Update...%20Then%20What/Read%2C%20Copy%2C%20Update...%20Then%20What%20-%20Fedor%20Pikus%20-%20CppCon%202017.pdf)
* [12 what is RCU 2013 Paul McKenny at IISc](https://youtu.be/obDzjElRj9c)

### Slides

* [SlideShare - Yet another introduction to Linux RCU](https://www.slideshare.net/vh21/yet-another-introduction-of-linux-rcu)
* Washington University CSE 522S - Advanced Operating Systems
  * [Read-Copy-Update Synchronization in the Linux Kernel](https://slideplayer.com/slide/10074374/)
