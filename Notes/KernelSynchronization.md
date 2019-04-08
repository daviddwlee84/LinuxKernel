# Linux Kernel Synchronization Mechanism

> We can consider each mechanism in the following scenario.
>
> * single vs. multiple CPU
> * preemptive vs. non-preemptive environmant

## Overview

Synchronization Mechanism:

* Per-CPU variable
  * [UMD CMSC412 - Per-CPU variables](https://www.cs.umd.edu/class/fall2015/cmsc412/percpu.pdf)
  * [0xAX/linux-insides/Concepts/linux-cpu-1.md Per-CPU variables](https://github.com/0xAX/linux-insides/blob/master/Concepts/linux-cpu-1.md)
* Atomic operations
* spinlock => 全自動
* Rwlock => 微半自動
* seqlock => 半自動
* RCU (Read-Copy Update) => 非常半自動
  * [Wiki - Read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)
  * [Linux 2.6內核中新的鎖機制--RCU](https://www.ibm.com/developerworks/cn/linux/l-rcu/index.html)
* semaphore 信號量
  * [Wiki - Semaphore (programming)](https://en.wikipedia.org/wiki/Semaphore_(programming))
* rw_semaphore
* mutex 互斥量 => simplified semaphore
* BKL (Big Kernel Lock)
* Brlokc (Big Reader Lock) (only in 2.4 Kernel)
* Completions 完成變量 (Condition Variable)
* Memory Barriers 內存屏障
* Local Interrupt Disabling
* Disabling and Enabling Derrable Function

> If you want the tool to be convenient/handy => slow (if you can avoid then don't use it)
>
> If you want the tool to be fast => harder to handle

## per-CPU variables

> logically split the data across the CPUs

* provide protection against concurrent accesses from *several CPUs*
* don't provide protection against accesses from *asynchronous functions*

> a kernel control path should access a per-CPU variable with **kernel preemption disabled** (prone race condition)

### Functions and Macros for the per-CPU variable

| Macro or Function name                 | Description |
| -------------------------------------- | ----------- |
| `DEFINE_PER_CPU(type, name)`           | -           |
| `per_cpu(name, cpu)`                   | -           |
| `__get_cpu_var(name)get_cpu_var(name)` | -           |
| `put_cpu_var(name)`                    | TBD         |
| `alloc_percpu(type)`                   | -           |
| `free_percpu(pointer)`                 | -           |
| `per_cpu_ptr(pointer, cpu)`            | -           |

## Atomic operations

> Make sure the instruction be executed atomically. (can't be interrupt)
>
> The atomic operation will slow down the system!

[`include/linux/types.h`](https://github.com/torvalds/linux/blob/master/include/linux/types.h#L176)

```c
typedef struct {
	int counter;
} atomic_t;
```

* Atomic operate function only accept with type `atomic_t`
* Compiler won't optimize the corresponding value

### Atomic Integer Operation

```c
// Define
atomic_t u; // define u
atomic_t v = ATOMIC_INIT(0); // define v and initialize it with 0

// Operation
atomic_set(&v, 4); // v = 4
atomic_add(2, &v); // v = v + 2
// as counter
atomic_inc(&v);    // v++
atomic_dec(&v);    // v--

// Other
// transfer atomic_t to int => atomic_read()
printk("%d\n", atomic_read(&v));
// v-- and check if v == 0 return True else False
int atomic_dec_and_test(atomic_t *v);
```

| Atomic Integer Operation | Description |
| ------------------------ | ----------- |
| TBD                      | TBD         |

### Atomic Bit Operation

TBD

| Atomic Bit Operation | Description |
| -------------------- | ----------- |
| TBD                  | TBD         |

## Spin Lock

> The most common lock

* Can only be hold by only one *executable thread*
  * Can prevent from *kernel preemption* and *SMP* (Symmetric MultiProcessing)
* Atomic test-and-set
* Busy loop -> spin
  * Can't prevent from *interrupt* => [solution](#Dealing-with-Interrupt)
* Must be *short-term*

[`include/linux/spinlock_types.h`](https://github.com/torvalds/linux/blob/master/include/linux/spinlock_types.h#L61)

```c
typedef struct spinlock {
	union {
		struct raw_spinlock rlock;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define LOCK_PADSIZE (offsetof(struct raw_spinlock, dep_map))
		struct {
			u8 __padding[LOCK_PADSIZE];
			struct lockdep_map dep_map;
		};
#endif
	};
} spinlock_t;
```

> To support cross-platform (line 12)
>
> ```c
> #if defined(CONFIG_SMP)
> # include <asm/spinlock_types.h>
> #else
> # include <linux/spinlock_types_up.h>
> #endif
> ```

### Dealing with Interrupt

```c
DEFINE_SPINLOCK(mr_lock);
unsigned long flags;

spin_lock_irqsave(&mr_lock, flags);
/* Critical Region */
spin_unlock_irqrestore(&mr_lock, flags);
```

### Close Bottom Half

`spin_lock_bh()`

> Interrupt may preempt the bottom-half. So we need to protect the data in the bottom-half.

#### ISR Top-half handler vs. Bottom-half handler

> ISR interrupt service routine aka. interrupt handler

* [The Top & Bottom Half's in Linux ISR](https://linuxforthenew.blogspot.com/2011/07/top-bottom-halfs-in-linux-isr.html)
* [Tasklets and Bottom-Half Processing](https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch09s05.html)

The big difference between the top-half handler and the bottom half is that all interrupts are enabled during execution of the bottom half—that’s why it runs at a safer time. In the typical scenario, the top half saves device data to a device-specific buffer, schedules its bottom half, and exits: this is very fast. The bottom half then performs whatever other work is required, such as awakening processes, starting up another I/O operation, and so on. This setup permits the top half to service a new interrupt while the bottom half is still working.

### Use Spin Lock to make global shared version per-CPU counter

* [`include/linux/percpu_counter.h`](https://github.com/torvalds/linux/blob/master/include/linux/percpu_counter.h#L20)

```c
struct percpu_counter {
	raw_spinlock_t lock;
	s64 count;
#ifdef CONFIG_HOTPLUG_CPU
	struct list_head list;	/* All percpu_counters are on a list */
#endif
	s32 __percpu *counters;
};
```

## Read-Write Spin Lock

> Improve the *parallelism of kernel*

| -          | Read    | Write   |
| ---------- | ------- | ------- |
| Read Lock  | TBD     | **TBD** |
| Write Lock | **TBD** | **TBD** |

[`include/linux/rwlock_types.h`](https://github.com/torvalds/linux/blob/master/include/linux/rwlock_types.h#L11)

```c
typedef struct {
	arch_rwlock_t raw_lock;
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned int magic, owner_cpu;
	void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} rwlock_t;
```

## Sequence (read-write) Lock (seqlock)

* We have to check if the data is valid

| -          | Read | Write   |
| ---------- | ---- | ------- |
| Read Lock  | TBD  | TBD     |
| Write Lock | TBD  | **TBD** |

[`include/linux/seqlock.h`](https://github.com/torvalds/linux/blob/master/include/linux/seqlock.h#L404)

```c
typedef struct {
	struct seqcount seqcount;
	spinlock_t lock;
} seqlock_t;
```

### Write and Read Operation

#### write

> line 446

```c
static inline void write_seqlock(seqlock_t *sl)
{
	spin_lock(&sl->lock);
	write_seqcount_begin(&sl->seqcount);
}

static inline void write_sequnlock(seqlock_t *sl)
{
	write_seqcount_end(&sl->seqcount);
	spin_unlock(&sl->lock);
}
```

#### read

> line 21

```c
// Expected non-blocking reader usage:
do {
    seq = read_seqbegin(&mr_seq_lock); // into Critical Region
    /* Read operation */
} while (read_seqretry(&mr_seq_lock, seq));
```

> line 436

```c
static inline unsigned read_seqretry(const seqlock_t *sl, unsigned start)
{
	return read_seqcount_retry(&sl->seqcount, start);
}
```

> line 203
>
> `__read_seqcount_retry` is like `read_seqcount_retry`, but has no `smp_rmb()` barrier

```c
static inline int __read_seqcount_retry(const seqcount_t *s, unsigned start)
{
	return unlikely(s->sequence != start);
}
```

### Example of Jiffies by using seqlock

> not sure

[`include/linux/jiffies.h`](https://github.com/torvalds/linux/blob/master/include/linux/jiffies.h#L86)

## RCU

| -          | Read | Write |
| ---------- | ---- | ----- |
| Read Lock  | TBD  | TBD   |
| Write Lock | TBD  | TBD   |

* No lock -> better performance
* Limit
  * Only protect *dynamic-allocated* and *referenced-with-pointer* data structure
  * In critical region not allow sleeping

> * Start: `rcu_read_lock()` = `preempt_disable()`
> * End: `rcu_read_unlock()` = `preempt_enable()`

### Writer

`rcu_assign_pointer(ptr, newptr)`

`synchronize_rcu()`

`call_rcu(struct rcu_head *head, void (*func) struct rcu_head *rcu)`

### Operation on Linked List

[`include/linux/rculist.h`](https://github.com/torvalds/linux/blob/master/include/linux/rculist.h#L170)

#### Example

[`net/hsr/hsr_framereg.c`](https://github.com/torvalds/linux/blob/master/net/hsr/hsr_framereg.c#L64)

### RCU for Read-Mostly Arrays

#### Hash Table

## Semaphore

> A kind of sleeping/blocking lock

[`include/linux/semaphore.h`](https://github.com/torvalds/linux/blob/master/include/linux/semaphore.h#L16)

```c
struct semaphore {
	raw_spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
};
```

[`kernel/locking/semaphore.c`](https://github.com/torvalds/linux/blob/master/kernel/locking/semaphore.c)

| Semaphore Operation | Description |
| ------------------- | ----------- |
| TBD                 | TBD         |

## Mutex

[`include/linux/mutex.h`](https://github.com/torvalds/linux/blob/master/include/linux/mutex.h#L53)

```c
struct mutex {
	atomic_long_t		owner;
	spinlock_t		wait_lock;
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
	struct optimistic_spin_queue osq; /* Spinner MCS lock */
#endif
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_MUTEXES
	void			*magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};
```

### Real-time Mutex

[`include/linux/rtmutex.h`](https://github.com/torvalds/linux/blob/master/include/linux/rtmutex.h#L30)

```c
struct rt_mutex {
	raw_spinlock_t		wait_lock;
	struct rb_root_cached   waiters;
	struct task_struct	*owner;
#ifdef CONFIG_DEBUG_RT_MUTEXES
	int			save_state;
	const char		*name, *file;
	int			line;
	void			*magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};
```

## Completions (Condition Variable)

> e.g. implementation of `vfork()`

[`include/linux/completion.h`](https://github.com/torvalds/linux/blob/master/include/linux/completion.h#L26)

```c
struct completion {
	unsigned int done;
	wait_queue_head_t wait;
};
```

### Example of Kernel Initialization

* `init/main.c`
  * [calling `kernel_thread`](https://github.com/torvalds/linux/blob/master/init/main.c#L416)
  * [`kernel_init`](https://github.com/torvalds/linux/blob/master/init/main.c#L1064)

## Barrier

| Barrier Operation | Description |
| ----------------- | ----------- |
| TBD               | TBD         |

## Forbid Preemption

* `preempt_enable()`
* `preempt_disable()`

## Conclusion

### Semaphore vs. Spinlock
