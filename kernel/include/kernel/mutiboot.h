#ifndef _KERNEL_MULTIBOOT_H
#define _KERNEL_MULTIBOOT_H 1

#include <stdint.h>

void multiboot_initialize();

extern uint8_t *MULTIBOOT_INFO_ADDRESS;

#endif