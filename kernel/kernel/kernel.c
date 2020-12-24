#include <stdio.h>
#include <stdlib.h>

#include <screen/screen.h>
#include <screen/logo/logo.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")

void wait() {
	for(int i = 0; i < 100000; i++) {
		((void)0);
	}
}

#pragma GCC pop_options

void print_logo();

static int protected = 0;

void kernel_early() {
	screen_initialize();
	asm("mov %%cr0, %0\n\t"
		:"=a"(protected));
		
	protected &= 1;
	int color = protected ? 0x00ff00 : 0xff0000;
	char* prot = protected ? "on" : "off";
	printf("Protected mode: %C%s%C\n\n", color, prot, 0xffffff);
}

void kernel_main() {
	screen_drawbmp(&logo, 64, 64, 5);
	printf("\nWelcome to the %Ctynvos%C kernel (v 0.0.0.1)\n", 0x00ffb7, 0xffffff);
	puts("Made by frogstair");
}