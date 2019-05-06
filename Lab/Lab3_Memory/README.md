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
| vm_flag                                    | The second column e.g. r-xp         | Attribute of virtual address space.                                                                                                         |
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
