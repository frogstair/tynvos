#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <stddef.h>
#include <stdint.h>

void memory_initialize();
uint32_t memory_getsize();

extern struct mmap_table_entry_t* TABLE_ENTRIES;

#endif