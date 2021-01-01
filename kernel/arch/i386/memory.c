#include <kernel/memory.h>
#include <kernel/mutiboot.h>

#include <stddef.h>

struct memory_sizes_t {
    uint32_t mem_lower;
    uint32_t mem_higher;
}* memory_sizes;

uint32_t TOPMOST_ADDRESS;

struct mmap_table_t {
    uint32_t mmap_length;
    uint32_t mmap_addr;
}* mmap_table;

struct mmap_table_entry_t {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
}* mmap_table_entry;

struct mmap_table_entry_t* TABLE_ENTRIES;

void memory_initialize() {
    if(!(*MULTIBOOT_INFO_ADDRESS & 1)) {
        return;
    }

    memory_sizes = (struct memory_sizes_t*)(MULTIBOOT_INFO_ADDRESS + 4);
    TOPMOST_ADDRESS = memory_getsize();

    if(!(*MULTIBOOT_INFO_ADDRESS & 0x40)) {
        return;
    }
    
    mmap_table = (struct mmap_table_t*)(MULTIBOOT_INFO_ADDRESS + 44);
    TABLE_ENTRIES = (struct mmap_table_entry_t*)(mmap_table->mmap_addr);
}

uint32_t memory_getsize() {
    if(memory_sizes->mem_higher) {
        return memory_sizes->mem_higher;
    } else {
        return memory_sizes->mem_lower;
    }
}

struct mmap_table_entry_t* memory_gettableindex(uint32_t index) {
    struct mmap_table_entry_t* entry;
    //TODO
    return entry;
}