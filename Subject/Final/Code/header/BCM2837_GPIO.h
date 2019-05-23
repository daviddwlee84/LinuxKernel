#ifndef BCM2837_GPIO_H
#define BCM2837_GPIO_H

#include <sys/mman.h> // PROT_READ, PROT_WRITE, MAP_SHARED, MAP_FAILED
#include <fcntl.h>    // O_RDWR, O_SYNC
#include <unistd.h>   // NULL

#include "seven_seg.h"

#define BCM2837_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000) /* GPIO controller */

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g) *(g_gpio.addr + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(g_gpio.addr + ((g) / 10)) |= (1 << (((g) % 10) * 3))

#define GPIO_SET *(g_gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(g_gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define BLOCK_SIZE (4 * 1024)

// I/O Acces
struct bcm2837_peripheral
{
    unsigned long addr_p;        // pointer to memory address
    int mem_fd;                  // the fd value of /dev/mem when open it
    void *map;                   // return of memory map
    volatile unsigned int *addr; // address to register
};

extern struct bcm2837_peripheral g_gpio;

int map_peripheral(struct bcm2837_peripheral *p);
void unmap_peripheral(struct bcm2837_peripheral *p);

#endif // BCM2837_GPIO_H