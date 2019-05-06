# Linux Memory Management

* Memory Addressing
  * Segmentation
* Physical and Virtual Memory
* Page Table Management
* Data Structures
* Allocators
  * vmalloc
  * Page Allocation
  * sla/ub
* Process address space

## Memory Addressing

* Logical Address
  * In machine language instruction, used to determine a operator or instruction address
  * Include *segment* and *offset*
* Linear Address (aka. Virtual Address) => "Express Ability"
  * In 32-bit machine => 4GB i.e. 4294967295. From 0x00000000 to 0xFFFFFFFF
* Physical Address => "Actually You Have"

```txt
Logical Address ===Segmentation Unit==> Linear Address ===Paging Unit==> Physical address
                             \                               /
                                Memory Management Unit (MMU)
```

![memory addressing](https://cdn-images-1.medium.com/max/800/0*JdsEg4qS7fxg_vsw.png)

IA-32 Memory Management Facilities:

* Segmentation: from Logical Address to Linear Address
* Paging (demand paging): from Linear Address to Phsical Address

> In Linux, it used these two mechanism at the same time. (because you have to support hardware... but tend to weaker the segmentation)

## Segmentation

### Segment Selectors

![segment selectors](http://www.renyujie.net/images/articles/article_ca_x86_4_3.png)

* Index
  * which is an index into the GDT and thus points to an entry of the GDT called a segment descriptor
* Table Indicator
  * which indicates whether the selector points to a GDT or an LDT
* Requestor Privilege Level (RPL)
  * which is the privilege level of the CPU when the selector is loaded

> Privilege Level in Linux only 0 and 3. One for operating system (kernel) level, another for user space level.

#### Privilege Level

* RPL (Request Privilege Level)
* CPL (Current Privilege Level)
* DPL (Descriptor Privilege Level)

* Access data, gate descripter: max(CPL, RPL) <= DPL
* ...

### Segment Register

* Visible part: Segment Selector
* Hidden part: Base Address, Limit, Access Information

> Linux uses segmentation only when required by the 80x86 architecture!

### Segment Model

#### Basic Flat Model - Linux

> Continuous, non-segment address space

![basic flat model](https://image1.slideserve.com/1765946/basic-flat-model-n.jpg)

#### Protected Flat Model

#### Multi-Segment Model

## Linux Physical Address

### Node

Linux .....

NUMA (Non Uniform Memory Access)

In x86 used UMA

### Zone

Each **node** split into many **zones**

> Because of hardware limitations, the kernel cannot treat all pages as identical

* `ZONE_DMA`
* `ZONE_NORMAL`
* `ZONE_HIGHMEM` (in 32-bit system, there may have some physical address that cannot be expressed by virtual address thus need this zone.)

In X86

* `ZONE_DMA` => 16MB of memory
* `ZONE_NORMAL` => 16MB ~ 896MB
* `ZONE_HIGHMEM` => 896MB ~ End (in 64-bit system it is empty)

## Linux Linear Address Space

...TBD

1GB Kernel Space + 3GB User Space

* All the user share the 1GB Kernel Space
* But for a single thread vision. It can see a 4GB memory space.

### Kernel Address Space

![Kernel Address Space
](https://www.kernel.org/doc/gorman/html/understand/understand-html010.png)

* vmalloc address space
  * Noncontiguous physical memory allocation
* kmap address space
  * Allocation of memory from `ZONE_HITHMEM`
* Fixed mapping
  * Compile-time virtual memory allocation

### 64-bit machine

> Used only 48-bit. => 128TB for user and another half for kernel

### Translating Kernel Virtual Address

* Memory in `ZONE_DMA` and `ZONE_NORMAL` is direct-mapped and all page frames are described by `mem_map` array

Kernel virtual address => physical address

Kernel virtual address => struct page

[arch/x86/include/asm/io.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/io.h#L131) - line 131

```c
/**
 *	virt_to_phys	-	map virtual addresses to physical
 *	@address: address to remap
 *
 *	The returned physical address is the physical (CPU) mapping for
 *	the memory address given. It is only valid to use this function on
 *	addresses directly mapped or allocated via kmalloc.
 *
 *	This function does not give bus mappings for DMA transfers. In
 *	almost all conceivable cases a device driver should not be using
 *	this function
 */

static inline phys_addr_t virt_to_phys(volatile void *address)
{
	return __pa(address);
}
#define virt_to_phys virt_to_phys
```

[arch/x86/include/asm/page.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/page.h#L42) - line 42 and 69

```c
#define __pa(x)		__phys_addr((unsigned long)(x))
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
```

## Page Table Management

### Page and Page Frame

* Page: a block of data, which may be stored in any page frame or on disk! (not necessary be in memory)
* Page Frame: physical addresses in main memory

### Paging in Linux

> 4 level paging model

* PGD Page Global Directory (頁全局目錄)
* PUD Page Upper Directory (頁上級目錄)
* PMD Page Middle Directory (頁中間目錄)
* PT Page Table (頁表)

> Multi-level is good when data is sparse

TBD

### High Memory Mappings

#### Physical Address Extension (PAE)

PDPT Page Directory Pointer Table

## Data Structure

### Node Stucture

include/linux/mmzone.h

* [struct pglist_data](https://github.com/torvalds/linux/blob/master/include/linux/mmzone.h#L635)
* [struct zone](https://github.com/torvalds/linux/blob/master/include/linux/mmzone.h#L362)

mm/memory.c

* [struct page *mem_map](https://github.com/torvalds/linux/blob/master/mm/memory.c#L93)

include/linux/mm_types.h

* [struct page](https://github.com/torvalds/linux/blob/master/include/linux/mm_types.h#L69)

include/linux/page-flags.h

* [enum pageflags](https://github.com/torvalds/linux/blob/master/include/linux/page-flags.h#L99)

### Relationship Between Nodes, Zones and Pages

### Macro

TBD

* [include/asm-generic/page.h](https://github.com/torvalds/linux/blob/master/include/asm-generic/page.h#L16)

```c
#define PAGE_SHIFT	12
#ifdef __ASSEMBLY__
#define PAGE_SIZE	(1 << PAGE_SHIFT)
#else
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#endif
#define PAGE_MASK	(~(PAGE_SIZE-1))
```

```c
typedef struct {
	unsigned long pte;
} pte_t;
typedef struct {
	unsigned long pmd[16];
} pmd_t;
typedef struct {
	unsigned long pgd;
} pgd_t;
typedef struct {
	unsigned long pgprot;
} pgprot_t;
typedef struct page *pgtable_t;
```

```c
#define pte_val(x)	((x).pte)
#define pmd_val(x)	((&x)->pmd[0])
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __pmd(x)	((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )
```

* [arch/x86/include/asm/pgtable-2level_types.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/pgtable-2level_types.h)
* [arch/x86/include/asm/pgtable-3level_types.h](https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/pgtable-3level_types.h)

## Allocator

* Boot Memory Allocator
  * `alloc_bootmem()`
  * `free_bootmem()`
* Physical Page Allocator (buddy system)
  * `alloc_pages()`
  * `__get_free_pages()`
* Slab Allocation
  * `kmalloc()`
  * `kfree()`
* Virtual Memory Allocator
  * `vmalloc()`
  * `vfree()`

## Noncontiguous ....

### Linear Address of Noncontiguous Memory Areas

vmalloc.c

## Physical Page Allocation

Not yet

## Resources

* [Linux Kernel: Memory Addressing](https://medium.com/hungys-blog/linux-kernel-memory-addressing-a0d304283af3)
* [Segmentation in x86](http://www.renyujie.net/articles/article_ca_x86_4.php)
* [Chapter 4  Process Address Space](https://www.kernel.org/doc/gorman/html/understand/understand007.html)
* [Shichao's Notes - Chapter 12. Memory Management](https://notes.shichao.io/lkd/ch12/)
* [Microprocessor system architectures – IA32 segmentation](https://www.slideserve.com/arty/microprocessor-system-architectures-ia32-segmentation)
* [pdf - Intel’s View of Memory Management](https://web.cs.wpi.edu/~cs3013/c07/lectures/Section09.1-Intel.pdf)
* [pdf - Memory Management: Paging](http://www.ic.unicamp.br/~celio/mc404-2013/arm-manuals/Paging%20Systems.pdf)
