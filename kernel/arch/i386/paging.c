#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/paging.h>

uint32_t pg_dir[1024] __attribute__((aligned(4096)));
uint32_t pg_tbl[1024] __attribute__((aligned(4096)));

void paging_initialize() {
    for(int i = 0; i < 1024; i++) {
        pg_dir[i] = 0x00000002;
        pg_tbl[i] = (i * 0x1000) | 3;
    }
    pg_dir[0] = ((unsigned int)pg_tbl) | 3;

    uint32_t* pgptr = pg_dir;

    asm("mov %0, %%eax\n\t"
        "mov %%eax, %%cr3\n\t"

        "mov %%cr0, %%eax\n\t"
        "or 0x80000001, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        :"=a"(pgptr));
}