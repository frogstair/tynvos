#include "kernel/mutiboot.h"

#include <stdint.h>

uint8_t *MULTIBOOT_INFO_ADDRESS;

void multiboot_initialize()
{
    asm("mov %%ebx, %0"
        : "=a"(MULTIBOOT_INFO_ADDRESS));
}