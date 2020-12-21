#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/paging.h>
#include <stdio.h>

void print_logo();

void kernel_early() {
	terminal_initialize();
	paging_initialize();

	int protected = 0;
	asm("cli\n\t"
		"mov %%cr0, %0\n\t"
		:"=a"(protected)
		:"a"(protected));

	protected &= 1;
	int color = 4 - protected * 2;
	char* prot = protected ? "on" : "off";

	printf("Protected mode: %C%s%C", color, prot, 7);
}

void kernel_main() {
	print_logo();
	printf("Welcome to %Ctynvos%C kernel (v 0.0.0.1)\n", 3, 7);
	puts("Made by frogstair");

	while(1);
}

void print_logo() {
	printf("%C", 3);
	puts("");
	puts("            ######            ");
	puts("          ######              ");
	puts("        ######                ");
	puts("      ######                  ");
	puts("    #############             ");
	puts("  ################        ####");
	puts(" ######       ######    ######");
	puts("#####           ####  ######  ");
	puts("####             #########    ");
	puts("####             #######      ");
	puts("#####           ######        ");
	puts(" ######       ######          ");
	puts("  ################            ");
	puts("    ############              ");
	puts("       ######                 ");
	puts("");
	printf("%C", 7);
}