# Lab3: Observe Memory Mapping and Find Physical Address

## 1. Observe Memory Mapping

### Segments in Process

* [Memory Layout of C Programs](https://www.geeksforgeeks.org/memory-layout-of-c-program/)

![Memory Layout](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/memoryLayoutC.jpg)

* **BSS Segment** (BSS stands for Block Started by Symbol)

    Generally used to store "uninitialized global variable"

* **Data Segment**

    Generally used to store "intialized global variable"

* **Code/Text Segment**

    Generally used to store "executable code".

    This segment is confirmed before execution. And this segment is usually read-only. (but in some architecture is editable).

    In this segment is possible to contain some read-only variable, such as `const char* string`

* **Heap**

    This segment is used to store "dynamic allocate memory". The size is not fixed.

    * malloc
    * free

* **Stack**

    This segment is used to store "temporary local variable". When a function is been invoked, the parameter is push into the process stack. Until the invocation end, the return value will also be put into the stack.

    Because of the LIFO feature of stack, it is very suit to reserve/resume status.

### Segments in Program (ARM)

* [ROM and RAM in ARM](https://stackoverflow.com/questions/5430284/rom-and-ram-in-arm)

```txt
+-----------+ 0x1000 8000 \
|  Unused   |             |
+-----------+             |
|  ZI data  | <--(clear)  | RAM
+-----------+             |
|  RW data  | <--(copy)---|---+
+-----------+ 0x1000 0000 /   |
                              |
                              |
+-----------+ 0x0008 0000 \   |
|  Unused   |             |   |
+-----------+             |   |
|  RW init  |-------------|---+
+-----------+             |
|  RO data  |             | ROM (Flash)
+-----------+             |
| User code |             |
+-----------+             |
| Boot code |             |
+-----------+             |
|  Vectors  |             |
+-----------+ 0x0000 0000 /
```

* **RO**: Read only region
  * **Code**: Compiled machine code
  * **RO_data**: ReadOnly data, global variable
    * e.g. `const` global variable
* **RW_data**: ReadWrite data
  * The initial value is not 0.
  * e.g. global variable, static variable.
* **ZI_data**: ZeroInitialized data
  * The read-write data which initial value is 0.
  * Including uninitialized and initial-zero global variable.

When program store in ROM. The size is **Code** + **RO_data** + **RW_data**.

When program is executing, **RW_data** and **ZI_data** is in the RAM. **RO_data** and **Code** is in either ROM or RAM depending the CPU architecture (51, ARM, x86). Among them, **ZI_data** corresponds to **BSS Segment**, **RW_data** and **RO_data** corresponds to **Data Segment**, **Code** corresponds to **Code/Text Segment**.

![Program Segment](ProgramSegment.png)

### Additional Experiment

Observe the program segments

In `test_program_segment_size.sh` will compile 5 different program. And test the setion size using `size` command in Ubuntu.

Here is the result:

```txt
$ ./test_program_segment_size.sh
====================
The code:

#include <stdio.h>
  
int main(void)
{
    return 0;
}

The size of the program:
   text    data     bss     dec     hex filename
   1033     276       4    1313     521 a.out
====================
The code:

#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    return 0;
}

The size of the program:
   text    data     bss     dec     hex filename
   1033     276       8    1317     525 a.out
====================
The code:

#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    static int i; /* Uninitialized static variable stored in bss */
    return 0;
}

The size of the program:
   text    data     bss     dec     hex filename
   1033     276      12    1321     529 a.out
====================
The code:

#include <stdio.h>
  
int global; /* Uninitialized variable stored in bss*/
  
int main(void)
{
    static int i = 100; /* Initialized static variable stored in DS*/
    return 0;
}

The size of the program:
   text    data     bss     dec     hex filename
   1033     280       8    1321     529 a.out
====================
The code:

#include <stdio.h>

int global = 10; /* initialized global variable stored in DS*/

int main(void)
{
    static int i = 100; /* Initialized static variable stored in DS*/
    return 0;
}

The size of the program:
   text    data     bss     dec     hex filename
   1033     284       4    1321     529 a.out
```

### Maps

Address space attribute

* r: readable
* w: writable
* x: executable
* p/s: private/shared
* -: no limit

| **vm_area_struct** entry of kernel process | **/proc/pid/maps** entry            | Detail                                                                                                                                      |
| ------------------------------------------ | ----------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| vm_start                                   | number before "-"                   | Starting address                                                                                                                            |
| vm_end                                     | number after "-"                    | Ending address                                                                                                                              |
| vm_flag                                    | The second column e.g. r-xp         | Attribute of virtual address space                                                                                                          |
| vm_pgoff                                   | The third column e.g. 00000000      | For named mapping, offset of starting address in virtual memory space (unit: page); For anonymous mapping, equal to 0 or vm_start/PAGE_SIZE |
| vm_file->f_dentry->d_inode->i_sb->s_dev    | The forth column e.g. fd:00         | Equipment number of mapping file. (00:00 means none)                                                                                        |
| vm_file->f_dentry->d_inode->i_ino          | The fifth column e.g. 9176473       | Node number of mapping file. (00:00 means none)                                                                                             |
| -                                          | The sixth column e.g. /lib/ld-25.so | For named mapping, it's mapping file name; For anonymous mapping, this is character of the segment e.g. [stack], [heap] or None             |

### [Experiment](Memory_Mapping)

Program output

> The result will differ each time.

```txt
$ ./lkpmem
variable globalvar1 	 location: 0x22edf018
variable globalvar2 	 location: 0x22edf010
variable localvar1 	 location: 0x15446be0
variable libvar 	 location: 0x15446bc4
variable functionvar 	 location: 0x15446bc4
```

```txt
$ PID=`ps aux | grep ./lkpmem | grep -v grep | awk '{print $2}'`
$ cat /proc/$PID/maps
558d22cde000-558d22cdf000 r-xp 00000000 08:01 678244                     /home/daviddwlee84/Desktop/observe_mem_mapping/src/lkpmem
558d22ede000-558d22edf000 r--p 00000000 08:01 678244                     /home/daviddwlee84/Desktop/observe_mem_mapping/src/lkpmem
558d22edf000-558d22ee0000 rw-p 00001000 08:01 678244                     /home/daviddwlee84/Desktop/observe_mem_mapping/src/lkpmem
558d24042000-558d24063000 rw-p 00000000 00:00 0                          [heap]
7f2c985d1000-7f2c987b8000 r-xp 00000000 08:01 269411                     /lib/x86_64-linux-gnu/libc-2.27.so
7f2c987b8000-7f2c989b8000 ---p 001e7000 08:01 269411                     /lib/x86_64-linux-gnu/libc-2.27.so
7f2c989b8000-7f2c989bc000 r--p 001e7000 08:01 269411                     /lib/x86_64-linux-gnu/libc-2.27.so
7f2c989bc000-7f2c989be000 rw-p 001eb000 08:01 269411                     /lib/x86_64-linux-gnu/libc-2.27.so
7f2c989be000-7f2c989c2000 rw-p 00000000 00:00 0 
7f2c989c2000-7f2c989c3000 r-xp 00000000 08:01 1399                       /usr/lib/liblkpsinglefoo.so
7f2c989c3000-7f2c98bc2000 ---p 00001000 08:01 1399                       /usr/lib/liblkpsinglefoo.so
7f2c98bc2000-7f2c98bc3000 r--p 00000000 08:01 1399                       /usr/lib/liblkpsinglefoo.so
7f2c98bc3000-7f2c98bc4000 rw-p 00001000 08:01 1399                       /usr/lib/liblkpsinglefoo.so
7f2c98bc4000-7f2c98beb000 r-xp 00000000 08:01 269383                     /lib/x86_64-linux-gnu/ld-2.27.so
7f2c98dd1000-7f2c98dd6000 rw-p 00000000 00:00 0 
7f2c98deb000-7f2c98dec000 r--p 00027000 08:01 269383                     /lib/x86_64-linux-gnu/ld-2.27.so
7f2c98dec000-7f2c98ded000 rw-p 00028000 08:01 269383                     /lib/x86_64-linux-gnu/ld-2.27.so
7f2c98ded000-7f2c98dee000 rw-p 00000000 00:00 0 
7ffd15428000-7ffd15449000 rw-p 00000000 00:00 0                          [stack]
7ffd1556c000-7ffd1556f000 r--p 00000000 00:00 0                          [vvar]
7ffd1556f000-7ffd15571000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
```

* 558d22XXXXXX /path/to/lkpmem
* 558d24XXXXXX heap
* 7f2c98XXXXXX linked library
* 7ffd15XXXXXX stack, vvar, vdso

## 2. Find Physical Address

### Background

Multi-Processor Model

* SMP (Shared Memory Multi-Processors)
  * UMA (Uniform Memory Access)
    * e.g. IA32
  * NUMA (NonUniform Memory Access)
    * e.g. ARM

### Linux Physical Memory Management

There are three level in Linux physical memory management

* Node 內存節點
* Zone 內存區域
* Page 物理頁框

#### [Linux Paging Mechanism](../../Notes/MemoryManagement.md#Paging-in-Linux)

Linux use 4-level paging model for both 32-bit and 64-bit system at the same time.

* Page Global Directory
* Page Upper Directory
* Page Middle Directory
* Page Table

### Principle

#### Node

Node is a way Linux describe the physical memory.

Linux use `pg_data_t` to represent Node in the header file `<linux/mmzone.h>`

#### Zone

Single Node is split into several Zone.

Linux use `struct zone` to describe zone in the header file `<linux/mmzone.h>`

Zone Type

* ZONE_DMA
  * compatible for old device
* ZONE_NORMAL
* ZONE_HIGHMEM

#### Page

When using paging mechanism, physical memory is represented by a fixed size physical page frame.

In the logical point of view, memory using fixed size page to represent.

#### Example of IA32

![Segment and Paging](https://i3.wp.com/www.cs.rutgers.edu/~pxk/416/notes/images/09a-ia32-seg_paging.png)

![IA-32e Paging with 4 KB pages](http://www.cs.rutgers.edu/~pxk/416/notes/images/09a-ia32e-paging-4K.png)

The variable address in C Language is a *virtual address*. More precisely, it is a *linear address* transform by the *segment* mechanism

### [Experiment](Physical_Address)

```txt
$ bash compile.sh
tmp value is: 12345678
tmp address is: 7FFEA9512230
the pid is: 3276
the read length is 711 and the buf content is:
PAGE_OFFSET = 0xffff966600000000
PGDIR_SHIFT = 39
P4D_SHIFT = 39
PUD_SHIFT = 30
PMD_SHIFT = 21
PAGE_SHIFT = 12
PTRS_PER_PGD = 512
PTRS_PER_P4D = 1
PTRS_PER_PUD = 512
PTRS_PER_PMD = 512
PTRS_PER_PTE = 512
PAGE_MASK = 0xfffffffffffff000
pgd_tmp = 0x00000000a1825621
pgd_val(*pgd_tmp) = 0x8000000076ee8067
p4d_tmp = 0x00000000a1825621
p4d_val(*pud_tmp) = 0x8000000076ee8067
pud_tmp = 0x00000000a3d6814e
pud_val(*pud_tmp) = 0x75841067
pmd_tmp = 0x000000009a8cd1a1
pmd_val(*pmd_tmp) = 0x7595d067
pte_tmp = 0x00000000ea3364f0
pte_val(*pte_tmp) = 0x800000004170b867
frame_addr = 800000004170b000
frame_offset = 230
the input logic address is = 0x7FFEA9512230
the corresponding physical address is= 0x800000004170B230
```

> If I open the `/proc/logadd2phyadd` directly
>
> ```txt
> $ cat /proc/logadd2phyadd
> PAGE_OFFSET = 0xffff966600000000
> PGDIR_SHIFT = 39
> P4D_SHIFT = 39
> PUD_SHIFT = 30
> PMD_SHIFT = 21
> PAGE_SHIFT = 12
> PTRS_PER_PGD = 512
> PTRS_PER_P4D = 1
> PTRS_PER_PUD = 512
> PTRS_PER_PMD = 512
> PTRS_PER_PTE = 512
> PAGE_MASK = 0xfffffffffffff000
> ```

## 3. Homework: Tracing Linux Code

> In the second experiment, in the `logadd2phyadd.c`. Try to understand how the `vaddr2paddr()` work.

Research `pgd_offset`, `p4d_offset`, `pud_offset`, `pte_offset_kernel` of x86 in the Linux 4.19.23 source code.

* [Elixir Cross Referencer for Linux 4.19.23](https://elixir.bootlin.com/linux/v4.19.23/source)

### pgd_offset

* [`arch/x86/include/asm/pgtable_64_types.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable_64_types.h#L74)

```c
/*
 * PGDIR_SHIFT determines what a top-level page table entry can map
 */
#define PGDIR_SHIFT		39
#define PTRS_PER_PGD		512
```

* [`/arch/x86/include/asm/pgtable.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable.h#999)

```c
/*
 * the pgd page can be thought of an array like this: pgd_t[PTRS_PER_PGD]
 *
 * this macro returns the index of the entry in the pgd page which would
 * control the given virtual address
 */
#define pgd_index(address) (((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

/*
 * pgd_offset() returns a (pgd_t *)
 * pgd_index() is used get the offset into the pgd page's array of pgd_t's;
 */
#define pgd_offset_pgd(pgd, address) (pgd + pgd_index((address)))
/*
 * a shortcut to get a pgd_t in a given mm
 */
#define pgd_offset(mm, address) pgd_offset_pgd((mm)->pgd, (address))
/*
 * a shortcut which implies the use of the kernel's pgd, instead
 * of a process's
 */
#define pgd_offset_k(address) pgd_offset(&init_mm, (address))
```

### p4d_offset

* [`/arch/x86/include/asm/pgtable_64_types.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable_64_types.h#L61)

```c
/*
 * 4th level page in 5-level paging case
 */
#define P4D_SHIFT		39
#define PTRS_PER_P4D		ptrs_per_p4d
```

* [`/arch/x86/boot/compressed/kaslr.c`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/boot/compressed/kaslr.c#L822) - Kernel Address Space Layout Randomization (KASLR)

```c
#ifdef CONFIG_X86_5LEVEL
	if (__read_cr4() & X86_CR4_LA57) {
		__pgtable_l5_enabled = 1;
		pgdir_shift = 48;
		ptrs_per_p4d = 512;
	}
#endif
```

* [`/arch/x86/kernel/head64.c`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/kernel/head64.c#L51)

```c
#ifdef CONFIG_X86_5LEVEL
unsigned int __pgtable_l5_enabled __ro_after_init;
unsigned int pgdir_shift __ro_after_init = 39;
EXPORT_SYMBOL(pgdir_shift);
unsigned int ptrs_per_p4d __ro_after_init = 1;
EXPORT_SYMBOL(ptrs_per_p4d);
#endif
```

* [`/arch/x86/include/asm/pgtable.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable.h#L947)

```c
/* to find an entry in a page-table-directory. */
static inline p4d_t *p4d_offset(pgd_t *pgd, unsigned long address)
{
	if (!pgtable_l5_enabled())
		return (p4d_t *)pgd;
	return (p4d_t *)pgd_page_vaddr(*pgd) + p4d_index(address);
}
```

### pud_offset

* [`/arch/x86/include/asm/pgtable_64_types.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable_64_types.h#L83)

```c
/*
 * 3rd level page
 */
#define PUD_SHIFT	30
#define PTRS_PER_PUD	512
```

* [`/arch/x86/include/asm/pgtable.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable.h#L906)

```c
/* Find an entry in the third-level page table.. */
static inline pud_t *pud_offset(p4d_t *p4d, unsigned long address)
{
	return (pud_t *)p4d_page_vaddr(*p4d) + pud_index(address);
}
```

### pte_offset_kernel

* [`/arch/x86/include/asm/pgtable.h`](https://elixir.bootlin.com/linux/v4.19.23/source/arch/x86/include/asm/pgtable.h#L818)

```c
static inline pte_t *pte_offset_kernel(pmd_t *pmd, unsigned long address)
{
	return (pte_t *)pmd_page_vaddr(*pmd) + pte_index(address);
}
```
