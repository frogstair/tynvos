#include <stdio.h>
#include <stdlib.h>

#include <kernel/mutiboot.h>
#include <kernel/memory.h>
#include <screen/screen.h>
#include <screen/logo/logo.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")

void wait() {
	for(int i = 0; i < 100000; i++) {
		(void)0;
	}
}

#pragma GCC pop_options

void print_logo();

static int protected = 0;

void kernel_early() {
	multiboot_initialize();
	memory_initialize();
	screen_initialize();

	asm("mov %%cr0, %0\n\t"
		:"=a"(protected));
		
	protected &= 1;
	int color = protected ? 0x5bc232 : 0xc23232;
	char* prot = protected ? "on" : "off";
	printf("Protected mode: %C%s%C\n", color, prot, 0xffffff);

	printf("Detected resolution: %dx%d\n\n", SCREEN_WIDTH, SCREEN_HEIGHT);

	int memorysize = (memory_getsize() - memory_getsize()%1000000) /1000000;
	printf("Detected memory: %uGB\n", (uint32_t)memorysize);
	puts("\n=========== KERNEL INITIALIZED ===========\n");
}

void kernel_main() {
	screen_drawbmp(&logo, 64, 64, 5);
	printf("\nWelcome to the %Ctynvos%C kernel (v 0.0.0.1)\n", 0x00ffb7, 0xffffff);
	puts("Made by frogstair");
}