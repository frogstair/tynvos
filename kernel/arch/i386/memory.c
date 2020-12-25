#include <kernel/memory.h>
#include <kernel/mutiboot.h>

#include <stddef.h>

struct memory_sizes_t {
    uint32_t mem_lower;
    uint32_t mem_higher;
}* memory_sizes;

uint32_t TOPMOST_ADDRESS;

void memory_initialize() {
    if(!(*MULTIBOOT_INFO_ADDRESS & 8)) {
        return;
    }

    memory_sizes = (struct memory_sizes_t*)(MULTIBOOT_INFO_ADDRESS + 4);
    TOPMOST_ADDRESS = memory_getsize();
}

uint32_t memory_getsize() {
    if(memory_sizes->mem_higher) {
        return memory_sizes->mem_higher;
    } else {
        return memory_sizes->mem_lower;
    }
}