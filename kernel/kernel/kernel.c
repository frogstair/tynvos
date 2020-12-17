#include <stdio.h>

#include <kernel/tty.h>
#include <stdio.h>

void print_logo();

void kernel_main(void) {
	terminal_initialize();
	print_logo();
	puts("Welcome to tynvos");
	puts("Kernel terminal initialized");
	puts("Made by frogstair");
}

void print_logo() {
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
	puts("");
}